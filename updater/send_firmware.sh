#!/usr/bin/env sh

amidi -p hw:2,0,0 -s ./DUO-bootloader.syx -i 10

sleep 1

# Write NXP's Flashloader to RAM using the built in USB bootloader
sdphost -u 0x1FC9,0x0145 -- write-file 0x20205800 ./ivt_flashloader.bin
# Then jump to the address where the Flashloader was placed
sdphost -u 0x1FC9,0x0145 -- jump-address 0x20205800

# The MCU will now reconnect as a USB composite device
sleep 1
blhost -u 0x15A2,0x0073  -- get-property 1 0

## Configure how the bootloader should address the flash memory
blhost -u 0x15A2,0x0073  -- fill-memory 0x20202000 4 0xC0000007
blhost -u 0x15A2,0x0073  -- fill-memory 0x20202004 4 0
blhost -u 0x15A2,0x0073  -- configure-memory 9 0x20202000
blhost -u 0x15A2,0x0073  -- write-memory 0x60000400 ./fcb_w25q.bin,512

echo "Erasing flash"
blhost -u 0x15A2,0x0073 -- flash-erase-region 0x60000400 128000
echo "Done"

echo "Writing binary"
blhost -u 0x15A2,0x0073 -- write-memory 0x60000400 ../src/duo-imxrt/build/flexspi_nor_release/out.bin
echo "done"

echo "Resetting"
blhost -u 0x15A2,0x0073 -- reset
