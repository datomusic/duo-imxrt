#!/bin/bash
arm-none-eabi-objcopy -vv -O binary \
    -j.flash_config \
    -j.ivt \
    -j.interrupts \
    -j.text \
    -j.ARM.extab \
    -j.ARM \
    -j.ctors \
    -j.dtors \
    -j.preinit_array \
    -j.init_array \
    -j.fini_array \
    -j.interrupts_ram \
    -j.data \
    -j.ram_function \
    -j.ncache.init \
    -j.ncache\
    -j.bss\
    -j.bss.dma\
    -j.heap\
    -j.stack\
    -j.ARM.attributes\
    *.elf ./duo_firmware.bin
