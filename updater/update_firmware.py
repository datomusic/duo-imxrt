#!/usr/bin/env python3

import sys
import time
import rtmidi
from rtmidi.midiutil import open_midioutput
from os.path import basename
from spsdk.sdp import SDP
import spsdk.sdp.interfaces.usb as sdp_usb
import spsdk.mboot.interfaces.usb as mboot_usb
from spsdk.mboot import McuBoot


def send_sysex_file(filename, midiout, delay=50):
    """Send contents of sysex file to given MIDI output.
    Reads file given by filename and sends all consecutive sysex messages found
    in it to given midiout after prompt.
    """

    SYSTEM_EXCLUSIVE = b'\xF0'
    END_OF_EXCLUSIVE = b'\xF7'
    bn = basename(filename)

    with open(filename, 'rb') as sysex_file:
        data = sysex_file.read()

        if data.startswith(SYSTEM_EXCLUSIVE):
            sox = 0
            i = 0

            while sox >= 0:
                sox = data.find(SYSTEM_EXCLUSIVE, sox)

                if sox >= 0:
                    eox = data.find(END_OF_EXCLUSIVE, sox)

                    if eox >= 0:
                        sysex_msg = data[sox:eox + 1]
                        # Python 2: convert data into list of integers
                        if isinstance(sysex_msg, str):
                            sysex_msg = [ord(c) for c in sysex_msg]

                        midiout.send_message(sysex_msg)
                        time.sleep(0.001 * delay)

                        i += 1
                    else:
                        break

                    sox = eox + 1


def enter_bootloader():
    midiout = rtmidi.MidiOut()
    available_ports = midiout.get_ports()

    
    if(len(available_ports) == 0):
        print("No MIDI ports found. Is the DUO connected?")
    elif(len(available_ports) > 1):
        print("Multiple MIDI ports found. Select one of:")
        print(available_ports)
    else:
        portname = available_ports[0]
    
        midiout, portname = open_midioutput(
            portname, interactive=True, use_virtual=True)

        send_sysex_file("./data/DUO-bootloader.syx", midiout)


def find_sdp_interface():
    match sdp_usb.scan_usb("0x1FC9,0x0145"):
        case [interface]:
            return interface
        case _:
            print("No DUO connected")


def find_mboot_interface():
    match mboot_usb.scan_usb("0x15A2,0x0073"):
        case [interface]:
            return interface
        case _:
            print("No DUO connected")


def update_firmware(firmware_path):
    with open(firmware_path, "rb") as firmware:
        firmware_bytes = firmware.read()

    try:
        enter_bootloader()
    except rtmidi._rtmidi.InvalidPortError:
        pass

    time.sleep(1)
    interface = find_sdp_interface()

    print("Sending flashloader")
    with SDP(interface) as s:
        flashloader_bytes = open("./data/ivt_flashloader.bin", "rb").read()
        flashloader_addr = 0x20205800
        s.write_file(flashloader_addr, flashloader_bytes)
        s.jump_and_run(flashloader_addr)

    print("Sent flashloader. Rebooting.")
    time.sleep(1)

    boot_interface = find_mboot_interface()

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
    firmware_path = "./duo_firmware.bin"
    if len(sys.argv) > 1:
        firmware_path = sys.argv[1]

    update_firmware(firmware_path)


if __name__ == "__main__":
    main()
