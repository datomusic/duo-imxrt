#!/usr/bin/env sh

#
# Installation instructions for sdphost and blhost are here:
# https://spsdk.readthedocs.io/en/latest/usage/installation.html
#
# Write NXP's Flashloader to RAM using the built in USB bootloader
sdphost -u 0x1FC9,0x0145 -- write-file 0x20205800 ./ivt_flashloader.bin
# Then jump to the address where the Flashloader was placed
sdphost -u 0x1FC9,0x0145 -- jump-address 0x20205800

# The MCU will now reconnect as a USB composite device
sleep 1
blhost -t 50000 -u 0x15A2,0x0073 -j -- get-property 1 0 
#blhost -u 0x15A2,0x0073 -j -- read-memory 1075791572 4 ./readReg.dat 0

# 
# Configure how the bootloader should address the flash memory
# The format of the configuration block is described in the MCU Bootloader Reference Manual document.
#
# blhost -u 0x15A2,0x0073 -j -- fill-memory 538976256 4 3221225479 word
blhost -u 0x15A2,0x0073 -j -- fill-memory 0x20202000 4 0xC0000007 word
# last bit 7 = 120MHz. Needs to be verified.

#blhost -u 0x15A2,0x0073 -j -- fill-memory 538976260 4 0 word
blhost -u 0x15A2,0x0073 -j -- fill-memory 0x20202004 4 0 word
#blhost -u 0x15A2,0x0073 -j -- configure-memory 9 538976256
blhost -u 0x15A2,0x0073 -j -- configure-memory 9 0x20202000
# blhost -u 0x15A2,0x0073 -j -- read-memory 1610613760 512 ./fcb_w25q.bin 9
# blhost -u 0x15A2,0x0073 -j -- flash-erase-all 9 

#
# Write the flash configuration block to flash
#
# blhost -u 0x15A2,0x0073 -j -- write-memory 1610613760 ./fcb_w25q.bin,512
blhost -u 0x15A2,0x0073 -j -- write-memory 0x60000400 ./fcb_w25q.bin,512
