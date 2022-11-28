#!/usr/bin/env sh

amidi -p hw:2,0,0 -s ./DUO-bootloader.syx -i 10

sleep 1

# Write NXP's Flashloader to RAM using the built in USB bootloader
sdphost -u 0x1FC9,0x0145 -- write-file 0x20205800 ./ivt_flashloader.bin
# Then jump to the address where the Flashloader was placed
sdphost -u 0x1FC9,0x0145 -- jump-address 0x20205800

# The MCU will now reconnect as a USB composite device
sleep 1
blhost -u 0x15A2,0x0073 -j -- get-property 1 0 

## Configure how the bootloader should address the flash memory
blhost -u 0x15A2,0x0073 -j -- fill-memory 0x20202000 4 0xC0000007 word
blhost -u 0x15A2,0x0073 -j -- fill-memory 0x20202004 4 0 word
blhost -u 0x15A2,0x0073 -j -- configure-memory 9 0x20202000
# blhost -u 0x15A2,0x0073 -j -- write-memory 0x60000400 ./fcb_w25q.bin,512

echo "erasing flash"
blhost -u 0x15A2,0x0073 -- flash-erase-region 0x60000400 128000
echo "done"

echo "writing binary"
blhost -u 0x15A2,0x0073 -- write-memory 0x60000400 ../src/demos/leds/build/flexspi_nor_debug/out.bin
echo "done"

blhost -u 0x15A2,0x0073 -- reset
