#!/usr/bin/env python3

import argparse
import sys
import time
import rtmidi
from rtmidi.midiutil import open_midioutput
from os.path import basename, dirname, abspath, exists
from spsdk.sdp import SDP
from spsdk.sdp.interfaces.usb import SdpUSBInterface
from spsdk.mboot.interfaces.usb import MbootUSBInterface
from spsdk.mboot import McuBoot
from firmware_info import print_firmware_info

# --- Device / memory constants -------------------------------------------

SDP_VID_PID = "0x1FC9,0x0145"
MBOOT_VID_PID = "0x15A2,0x0073"

FLASHLOADER_ADDR = 0x20205800
BINARY_START_ADDR = 0x60000400
CONFIG_BLOCK_ADDR = 0x20202000
CONFIG_BLOCK_TAG = 0xC0000007
FLEXSPI_NOR_MEM_ID = 9

RESET_SYX = [0xF0, 0x7D, 0x64, 0x0B, 0xF7]

POLL_INTERVAL = 0.5
MBOOT_TIMEOUT = 10.0

# Wait time (seconds) to confirm board is unplugged.
REMOVAL_SETTLE = 4.0

# Wait time to confirm unplugged after boot succeeds.
CONFIRMED_REMOVAL_SETTLE = 1.0

# Max wait time for board to boot into MIDI mode.
BOOT_TIMEOUT = 8.0

# --- Terminal helpers -----------------------------------------------------

SPINNER = "|/-\\"
USE_ANSI = sys.stdout.isatty()

GREEN, RED, YELLOW, DIM, RESET = "\033[32m", "\033[31m", "\033[33m", "\033[2m", "\033[0m"


def colour(code, text):
    return f"{code}{text}{RESET}" if USE_ANSI else text


def status(text):
    """Draw a status line in the terminal."""
    if USE_ANSI:
        print(f"\r\033[K{text}", end="", flush=True)


def clear_status():
    if USE_ANSI:
        print("\r\033[K", end="", flush=True)


# --- Device discovery -----------------------------------------------------

def find_duo_midi_port(quiet=False):
    for (index, name) in enumerate(rtmidi.MidiOut().get_ports()):
        if "duo" in name.lower():
            if not quiet:
                print(f"Found {name} connected to MIDI")
            return index
    if not quiet:
        print("Could not detect DUO midi port.")
    return None


def find_sdp_interface(quiet=False):
    match SdpUSBInterface.scan(SDP_VID_PID):
        case [interface]:
            if not quiet:
                print(f"Found {interface.device.product_name}")
            return interface
        case _:
            if not quiet:
                print("No DUO connected in SDP host mode")
            return None


def find_mboot_interface(quiet=False):
    match MbootUSBInterface.scan(MBOOT_VID_PID):
        case [interface]:
            return interface
        case _:
            if not quiet:
                print("No DUO connected in MBOOT mode")
            return None


def enter_bootloader():
    """Ask a running DUO to reset into SDP via MIDI sysex."""
    duo_port = find_duo_midi_port()
    if duo_port is None:
        return False

    print_firmware_info()
    midiout, portname = open_midioutput(duo_port, use_virtual=False)
    print(f"Sending reset signal to {portname} at port {duo_port}")
    midiout.send_message(RESET_SYX)
    return True


def wait_for_mboot(timeout=MBOOT_TIMEOUT):
    """Wait for the flashloader to enumerate."""
    deadline = time.monotonic() + timeout
    tick = 0
    while time.monotonic() < deadline:
        interface = find_mboot_interface(quiet=True)
        if interface:
            clear_status()
            return interface
        status(f"  {SPINNER[tick % 4]} waiting for flashloader to enumerate...")
        tick += 1
        time.sleep(0.25)
    clear_status()
    return None


def duo_present():
    """True if a DUO is visible in SDP, MBOOT, or normal firmware (MIDI) mode."""
    return bool(
        find_sdp_interface(quiet=True)
        or find_mboot_interface(quiet=True)
        or find_duo_midi_port(quiet=True) is not None
    )


def wait_for_boot(timeout=BOOT_TIMEOUT):
    """Wait for board to boot into MIDI mode."""
    deadline = time.monotonic() + timeout
    tick = 0
    while time.monotonic() < deadline:
        if find_duo_midi_port(quiet=True) is not None:
            clear_status()
            return True
        status(colour(DIM, f"  {SPINNER[tick % 4]} waiting for the board to boot..."))
        tick += 1
        time.sleep(0.25)
    clear_status()
    return False


def wait_for_removal(settle=REMOVAL_SETTLE):
    """Wait until the board is unplugged."""
    tick = 0
    absent_since = None
    while True:
        if duo_present():
            absent_since = None
        elif absent_since is None:
            absent_since = time.monotonic()
        elif time.monotonic() - absent_since >= settle:
            break
        if absent_since is None:
            message = "unplug the board to continue..."
        else:
            remaining = settle - (time.monotonic() - absent_since)
            message = f"board removed, ready in {remaining:.0f}s"
        status(colour(DIM, f"  {SPINNER[tick % 4]} {message}"))
        tick += 1
        time.sleep(POLL_INTERVAL)
    clear_status()


# --- Flashing -------------------------------------------------------------

def load_firmware(firmware_path):
    if not exists(firmware_path):
        print(f"Firmware file not found: {firmware_path}")
        print("Please specify a file location.")
        return None
    with open(firmware_path, "rb") as firmware:
        return firmware.read()


def flash(interface, firmware_bytes, firmware_path, data_path):
    """Flash a board in SDP mode. Returns True on success."""
    flashloader_path = f"{data_path}/ivt_flashloader.bin"
    if not exists(flashloader_path):
        print(f"Flashloader not found: {flashloader_path}")
        return False
    with open(flashloader_path, "rb") as flashloader:
        flashloader_bytes = flashloader.read()

    print("Sending flashloader ... ", end="", flush=True)
    with SDP(interface) as s:
        s.write_file(FLASHLOADER_ADDR, flashloader_bytes)
        s.jump_and_run(FLASHLOADER_ADDR)
    print("done")

    boot_interface = wait_for_mboot()
    if not boot_interface:
        print(f"MBoot interface did not appear within {MBOOT_TIMEOUT:.0f}s.")
        return False

    with McuBoot(boot_interface) as mboot:
        if mboot.get_property(1, 0) is None:
            print(f"Could not query bootloader: {mboot.status_string}")
            return False

        print("Configuring memory ... ", end="", flush=True)
        configured = (
            mboot.fill_memory(CONFIG_BLOCK_ADDR, 4, CONFIG_BLOCK_TAG)
            and mboot.fill_memory(CONFIG_BLOCK_ADDR + 4, 4, 0)
            and mboot.configure_memory(CONFIG_BLOCK_ADDR, FLEXSPI_NOR_MEM_ID)
        )
        if not configured:
            print(f"FAILED ({mboot.status_string})")
            return False
        print("done")

        print("Erasing flash ... ", end="", flush=True)
        if not mboot.flash_erase_region(BINARY_START_ADDR, len(firmware_bytes)):
            print(f"FAILED ({mboot.status_string})")
            return False
        print("done")

        print(f"Writing {basename(firmware_path)} "
              f"({len(firmware_bytes):,} bytes) ... ", end="", flush=True)
        if not mboot.write_memory(BINARY_START_ADDR, firmware_bytes):
            print(f"FAILED ({mboot.status_string})")
            return False
        print("done")

        print("Resetting")
        mboot.reset(reopen=False)

    return True


def flash_once(firmware_bytes, firmware_path, data_path,
               skip_enter_bootloader=False, interactive=True):
    """Reset board via MIDI (optional) and flash via SDP."""
    if skip_enter_bootloader:
        print("Skipping MIDI bootloader entry.")
    else:
        if not enter_bootloader() and interactive:
            input("Please enter bootloader manually, then press Enter.")
        time.sleep(1)

    interface = find_sdp_interface()
    if not interface:
        return False

    time.sleep(1)
    return flash(interface, firmware_bytes, firmware_path, data_path)


# --- Bench loop -----------------------------------------------------------

def run_loop(firmware_bytes, firmware_path, data_path, use_midi_reset):
    """Continuously wait for boards and flash them."""
    flashed = failed = 0
    tick = 0

    if use_midi_reset:
        print("Polling for a running DUO on MIDI. Ctrl-C to quit.")
    else:
        print("Factory mode: plug in a blank board to flash it. Ctrl-C to quit.")

    try:
        while True:
            interface = find_sdp_interface(quiet=True)

            if interface is None and use_midi_reset:
                if find_duo_midi_port(quiet=True) is not None:
                    clear_status()
                    print(f"\n--- board #{flashed + failed + 1} ---")
                    try:
                        enter_bootloader()
                        time.sleep(1)
                        interface = find_sdp_interface(quiet=True)
                    except KeyboardInterrupt:
                        raise
                    except Exception as exc:
                        interface = None
                        print(f"  {type(exc).__name__}: {exc}")

                    if interface is None:
                        failed += 1
                        print(colour(RED, "FAILED: board did not appear in SDP mode"))
                        wait_for_removal()
                        continue
                else:
                    interface = None

            if interface is None:
                tally = colour(DIM, f"[{flashed} ok, {failed} failed]")
                status(f"{SPINNER[tick % 4]} waiting for a board  {tally}")
                tick += 1
                time.sleep(POLL_INTERVAL)
                continue

            clear_status()
            if not use_midi_reset:
                print(f"\n--- board #{flashed + failed + 1} ---")

            try:
                ok = flash(interface, firmware_bytes, firmware_path, data_path)
            except KeyboardInterrupt:
                raise
            except Exception as exc:
                ok = False
                print(f"\n  {type(exc).__name__}: {exc}")

            if ok:
                flashed += 1
                print(colour(GREEN, "OK  flashed successfully"))
                booted = wait_for_boot()
                if booted:
                    print(colour(GREEN, "OK  booted into firmware"))
                else:
                    print(colour(YELLOW, f"note: no MIDI port within "
                                         f"{BOOT_TIMEOUT:.0f}s"))
                wait_for_removal(
                    CONFIRMED_REMOVAL_SETTLE if booted else REMOVAL_SETTLE
                )
            else:
                failed += 1
                print(colour(RED, "FAILED"))
                wait_for_removal()
    except KeyboardInterrupt:
        clear_status()
        print()

    return flashed, failed


# --- Entry point ----------------------------------------------------------

def main():
    script_path = abspath(dirname(sys.argv[0]))
    data_path = f"{script_path}/data"

    parser = argparse.ArgumentParser(prog="DUO firmware updater")
    parser.add_argument(
        'firmware_path', nargs='?', default=f"{script_path}/duo_firmware.bin"
    )
    parser.add_argument(
        '-c', '--continuous', action='store_true',
        help="Keep polling for a running DUO on MIDI, resetting and flashing each one."
    )
    parser.add_argument(
        '-f', '--factory', action='store_true',
        help="Keep polling for boards already in SDP mode (i.e. blank chips), "
             "without attempting MIDI bootloader entry."
    )
    args = parser.parse_args()

    firmware_bytes = load_firmware(args.firmware_path)
    if firmware_bytes is None:
        return 1

    print(f"Firmware: {basename(args.firmware_path)} ({len(firmware_bytes):,} bytes)")

    if args.continuous or args.factory:
        flashed, failed = run_loop(
            firmware_bytes, args.firmware_path, data_path,
            use_midi_reset=args.continuous
        )
        print(f"{flashed} flashed, {failed} failed.")
        return 0

    ok = flash_once(firmware_bytes, args.firmware_path, data_path, interactive=True)
    print(colour(GREEN, "\nOK  flashed successfully") if ok
          else colour(RED, "\nFAILED"))
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
