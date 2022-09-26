# Flashing a blank microcontroller

## Outline
The microcontroller has a built-in USB bootloader that runs whenever the contents of the flash
are invalid. We're going to use sdphost to talk to the built-in USB bootloader and upload
a temporary flashloader. After that, we will use blhost to program a flash configuration
block that tells the microcontroller how to communicate with the flash chip.

## Requirements
NXP [SPSDK](https://spsdk.readthedocs.io/en/latest/usage/installation.html)

## Steps
Once the spsdk is installed and added to the path, run duobrains2_init.sh. It runs through all the steps needed to configure the microcontroller for use with Winbond W25Q flash chips (models that end in 'IM' are supported)