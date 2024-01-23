This repository contains the firmware for the Dato DUO synthesizer serial numbers 5000 and up.

## Updating the firmware

Grab one of the binaries from https://github.com/datomusic/duo-imxrt/releases
Make sure you have Python 3 installed

in the commandline, go to the updater directory and enter
`python3 -m pip install -r requirements.txt`

use `python3 update_firmware.py`

if on Mac you get an error `usb.core.NoBackendError: No backend available`, please install libusb by running `brew install libusb`

## DUO Brains revisions
The main pcb of the Dato DUO is dubbed Brains. Up until mid 2022 the Brains 1 were based on the NXP K20DX256 microcontroller. Due to ongoing problems with availability of that chip family later Brains 2 are based on NXP iMX RT1010

Source code for the two different platforms can be found in the platforms directory.

## Credits
- Teensy Audio Library: https://github.com/PaulStoffregen/cores https://github.com/PaulStoffregen/Audio
- Seeeduino Arduino core for imxrt: https://github.com/Seeed-Studio/ArduinoCore-imxrt
- WS2812-Flexio driver: https://github.com/Finomnis/ws2812-flexio/
