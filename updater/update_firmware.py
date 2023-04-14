#!/usr/bin/env python3

import argparse
import sys
import time
import rtmidi
from rtmidi.midiutil import open_midioutput
from os.path import basename
from spsdk.sdp import SDP
import spsdk.sdp.interfaces.usb as sdp_usb
import spsdk.mboot.interfaces.usb as mboot_usb
from spsdk.mboot import McuBoot


def find_duo_midi_port():
    for (index, name) in enumerate(rtmidi.MidiOut().get_ports()):
        if "duo" in name.lower():
            return index
    return None


def enter_bootloader():
    duo_port = find_duo_midi_port()

    if not duo_port:
        print("Could not detect DUO midi port.")
        return False
    else:
        midiout, portname = open_midioutput(duo_port, use_virtual=False)
        reset_syx = [0xF0, 0x7d, 0x64, 0x0b, 0xF7]
        midiout.send_message(reset_syx)
        return True


def find_sdp_interface():
    match sdp_usb.scan_usb("0x1FC9,0x0145"):
        case [interface]:
            return interface
        case _:
            print("No DUO connected")
            return None


def find_mboot_interface():
    match mboot_usb.scan_usb("0x15A2,0x0073"):
        case [interface]:
            return interface
        case _:
            print("No DUO connected")
            return None


def update_firmware(firmware_path, data_path, continuous):
    with open(firmware_path, "rb") as firmware:
        firmware_bytes = firmware.read()

    if not enter_bootloader():
        if continuous:
            print("Continuing. [Continuous mode]")
        else:
            input("Please enter bootloader manually, then press Enter.")

    time.sleep(1)
    interface = find_sdp_interface()
    if not interface:
        print("Aborting.")
        return False

    print("Sending flashloader")
    with SDP(interface) as s:
        flashloader_bytes = open(f"{data_path}/ivt_flashloader.bin", "rb").read()
        flashloader_addr = 0x20205800
        s.write_file(flashloader_addr, flashloader_bytes)
        s.jump_and_run(flashloader_addr)

    print("Sent flashloader. Rebooting.")
    time.sleep(1)

    print("Finding boot interface.")
    boot_interface = find_mboot_interface()
    if not boot_interface:
        print("Aborting.")
        return False

    with McuBoot(boot_interface) as mboot:
        mboot.get_property(1, 0)
        mboot.fill_memory(0x20202000, 4, 0xC0000007)
        mboot.fill_memory(0x20202004, 4, 0)
        "Configuring memory"
        mboot.configure_memory(0x20202000, 9)

        binary_start_addr = 0x60000400

        print("Erasing flash")
        mboot.flash_erase_region(binary_start_addr, len(firmware_bytes))
        print("Done")

        print("Writing binary")
        mboot.write_memory(binary_start_addr, firmware_bytes)
        print("Done")

        print("Resetting")
        mboot.reset(reopen=False)

def main():
    from os.path import dirname,abspath
    script_path = abspath(dirname(sys.argv[0]))
    data_path = f"{script_path}/data"

    parser = argparse.ArgumentParser(prog="DUO firmware updater")
    parser.add_argument('firmware_path', nargs='?', default=f"{script_path}/duo_firmware.bin", )
    parser.add_argument(
        '-c', '--continuous', action='store_true', 
        help="Disable user interaction and keep polling after successful or failed updates."
    )

    args = parser.parse_args()

    if args.continuous:
        print("Polling (continuous mode).")
        print()
        while True:
            time.sleep(3)
            update_firmware(args.firmware_path, data_path, True)
    else:
        update_firmware(args.firmware_path, data_path, False)


if __name__ == "__main__":
    main()
