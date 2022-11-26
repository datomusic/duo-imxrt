import rtmidi
from rtmidi.midiutil import list_output_ports, open_midioutput
from os.path import basename, exists, isdir, join
import argparse
import logging
import os
import sys
import time

log = logging.getLogger("sendsysex")

__program__ = 'sendsysex.py'
__version__ = '1.1'
__author__ = 'Christopher Arndt'
__date__ = '$Date$'

SYSTEM_EXCLUSIVE = b'\xF0'
END_OF_EXCLUSIVE = b'\xF7'

try:
    raw_input
except NameError:
    # Python 3
    raw_input = input

def send_sysex_file(filename, midiout, portname, prompt=True, delay=50):
    """Send contents of sysex file to given MIDI output.
    Reads file given by filename and sends all consecutive sysex messages found
    in it to given midiout after prompt.
    """
    bn = basename(filename)

    with open(filename, 'rb') as sysex_file:
        data = sysex_file.read()

        if data.startswith(SYSTEM_EXCLUSIVE):
            try:
                if prompt:
                    yn = raw_input("Send '%s' to %s (y/N)? " % (bn, portname))
            except (EOFError, KeyboardInterrupt):
                print('')
                raise StopIteration

            if not prompt or yn.lower() in ('y', 'yes'):
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

                            log.info("Sending '%s' message #%03i...", bn, i)
                            midiout.send_message(sysex_msg)
                            time.sleep(0.001 * delay)

                            i += 1
                        else:
                            break

                        sox = eox + 1
        else:
            log.warning("File '%s' does not start with a sysex message.", bn)


def enter_bootloader():
    portname = "Duo MIDI 1"
    midiout, portname = open_midioutput(portname, interactive=False, use_virtual=True)
    print("Sending sysex")
    send_sysex_file("./DUO-bootloader.syx", midiout, "Duo MIDI 1", prompt=False)
    print("done")


def main():
    enter_bootloader()


if __name__ == "__main__":
    main()
