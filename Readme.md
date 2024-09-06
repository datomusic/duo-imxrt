This repository contains the firmware for the Dato DUO synthesizer serial numbers 4630 and up.

## Updating the firmware

Grab one of the binaries from https://github.com/datomusic/duo-imxrt/releases
Make sure you have Python 3 installed

in the commandline, go to the `tools/updater` directory and enter
`python3 -m pip install -r requirements.txt`

use `python3 update_firmware.py`

if on Mac you get an error `usb.core.NoBackendError: No backend available`, please install libusb by running `brew install libusb`

## Building the firmware
Make sure you have version 12.3.rel1 of the Arm GNU Toolchain for embedded installed: https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads . It has 'eabi' in the filename.

Git clone this repository including submodules
`git clone --recurse-submodules https://github.com/datomusic/duo-imxrt.git`

Go into the apps directory
`cd duo-imxrt/brains2/apps`

Tell the build script where the compiler is and run the build script
`export TOOLCHAIN_DIR=/Applications/ArmGNUToolchain/12.3.rel1/arm-none-eabi && ../scripts/build.sh duo`

If you get an error saying that `The CMAKE_C_COMPILER: /bin/arm-none-eabi-gcc is not a full path to an existing compiler tool.` make sure the path at `TOOLCHAIN_DIR` is valid and points to the `arm-none-eabi` dir

If you get an error along the lines of `Cannot find source file: /duo-imxrt/brains2/libraries/tinyusb/src/class/cdc/cdc_device.c` make sure all submodules are downloaded. You can make sure all submodules are fetched by running `git submodule update --init --recursive`

If the build is successful you can upload it by running `../script/send_firmware.sh duo`. You will need all correct python libraries as well so make sure you followed the instruction at "Updating the firmware" to install them.

## DUO Brains revisions
The main pcb of the Dato DUO is dubbed Brains. Up until mid 2022 the Brains 1 were based on the NXP K20DX256 microcontroller. Due to ongoing problems with availability of that chip family later Brains 2 are based on NXP iMX RT1010

Source code for the two different platforms can be found in the platforms directory.

## Credits
- Teensy Audio Library: https://github.com/PaulStoffregen/cores https://github.com/PaulStoffregen/Audio
- Seeeduino Arduino core for imxrt: https://github.com/Seeed-Studio/ArduinoCore-imxrt
- WS2812-Flexio driver: https://github.com/Finomnis/ws2812-flexio/
