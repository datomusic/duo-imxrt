/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "duobrains2_flexspi_nor_config.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.xip_board"
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(XIP_BOOT_HEADER_ENABLE) && (XIP_BOOT_HEADER_ENABLE == 1)
#if defined(__CC_ARM) || defined(__ARMCC_VERSION) || defined(__GNUC__)
__attribute__((section(".boot_hdr.conf"), used))
#elif defined(__ICCARM__)
#pragma location = ".boot_hdr.conf"
#endif

#define SEQUENCE(first, second, third, fourth) first, second, third, fourth
#define TWO_EMPTY_STEPS 0x00000000
#define EMPTY_SEQUENCE SEQUENCE(TWO_EMPTY_STEPS, TWO_EMPTY_STEPS, TWO_EMPTY_STEPS, TWO_EMPTY_STEPS)

// SPI Flash config for Winbond W25Q16
// Change .sflashA1Size 
const flexspi_nor_config_t qspiflash_config = {
    .memConfig =
        {
            .tag              = FLEXSPI_CFG_BLK_TAG,
            .version          = FLEXSPI_CFG_BLK_VERSION,
            .readSampleClkSrc = kFlexSPIReadSampleClk_LoopbackInternally,
            .csHoldTime       = 1u,
            .csSetupTime      = 2u,
            .deviceType       = kFlexSpiDeviceType_SerialNOR,
            .sflashPadType    = kSerialFlash_4Pads,
            .serialClkFreq    = kFlexSpiSerialClk_133MHz,
            .sflashA1Size     = 16u * 1024u * 1024u,
            .lookupTable =
                {
            // FLEXSPI_LUT_SEQ(cmd0, pad0, op0, cmd1, pad1, op1)
            // The high 16 bits is command 1 and the low are command 0.
            // Within a command, the top 6 bits are the opcode, the next two are the number
            // of pads and then last byte is the operand. The operand's meaning changes
            // per opcode.

            // Indices with ROM should always have the same function because the ROM
            // bootloader uses it.

            // 0: ROM: Read LUTs
            // Quad version
            SEQUENCE(FLEXSPI_LUT_SEQ(CMD_SDR,   FLEXSPI_1PAD, 0xEB /* the command to send */,
                                     RADDR_SDR, FLEXSPI_4PAD, 24 /* bits to transmit */),
                     FLEXSPI_LUT_SEQ(DUMMY_SDR, FLEXSPI_4PAD, 6 /* 6 dummy cycles, 2 for M7-0 and 4 dummy */,
                                     READ_SDR,  FLEXSPI_4PAD, 0x04),
            // Single fast read version, good for debugging.
            // FLEXSPI_LUT_SEQ(CMD_SDR,   FLEXSPI_1PAD, 0x0B /* the command to send */,
            //                 RADDR_SDR, FLEXSPI_1PAD, 24  /* bits to transmit */),
            // FLEXSPI_LUT_SEQ(DUMMY_SDR, FLEXSPI_1PAD, 8 /* 8 dummy clocks */,
            //                 READ_SDR,  FLEXSPI_1PAD, 0x04),
                     TWO_EMPTY_STEPS,
                     TWO_EMPTY_STEPS),

            // 1: ROM: Read status
            SEQUENCE(FLEXSPI_LUT_SEQ(CMD_SDR,  FLEXSPI_1PAD, 0x05  /* the command to send */,
                                     READ_SDR, FLEXSPI_1PAD, 0x02),
                     TWO_EMPTY_STEPS,
                     TWO_EMPTY_STEPS,
                     TWO_EMPTY_STEPS),

            // 2: Empty
            SEQUENCE(FLEXSPI_LUT_SEQ(CMD_SDR,   FLEXSPI_1PAD, 0x35 /* the command to send */,
                DUMMY_SDR, FLEXSPI_1PAD, 8),
                TWO_EMPTY_STEPS,
                TWO_EMPTY_STEPS,
                TWO_EMPTY_STEPS),

            // 3: ROM: Write Enable
            SEQUENCE(FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x06  /* the command to send */,
                                     STOP,    FLEXSPI_1PAD, 0x00),
                     TWO_EMPTY_STEPS,
                     TWO_EMPTY_STEPS,
                     TWO_EMPTY_STEPS),

            // 4: Config: Write Status
            SEQUENCE(FLEXSPI_LUT_SEQ(CMD_SDR,   FLEXSPI_1PAD, 0x01  /* the command to send */,
                                     WRITE_SDR, FLEXSPI_1PAD, 0x01),
                     TWO_EMPTY_STEPS,
                     TWO_EMPTY_STEPS,
                     TWO_EMPTY_STEPS),

            // 5: ROM: Erase Sector
            SEQUENCE(FLEXSPI_LUT_SEQ(CMD_SDR,   FLEXSPI_1PAD, 0x20  /* the command to send */,
                                     RADDR_SDR, FLEXSPI_1PAD, 24 /* bits to transmit */),
                     TWO_EMPTY_STEPS,
                     TWO_EMPTY_STEPS,
                     TWO_EMPTY_STEPS),

            // 6: Empty
            EMPTY_SEQUENCE,

            // 7: Empty
            EMPTY_SEQUENCE,

            // 8: Block Erase
            SEQUENCE(FLEXSPI_LUT_SEQ(CMD_SDR,   FLEXSPI_1PAD, 0xD8  /* the command to send */,
                                     RADDR_SDR, FLEXSPI_1PAD, 24 /* bits to transmit */),
                     TWO_EMPTY_STEPS,
                     TWO_EMPTY_STEPS,
                     TWO_EMPTY_STEPS),

            // 9: ROM: Page program
            SEQUENCE(FLEXSPI_LUT_SEQ(CMD_SDR,   FLEXSPI_1PAD, 0x02  /* the command to send */,
                                     RADDR_SDR, FLEXSPI_1PAD, 24 /* bits to transmit */),

                     FLEXSPI_LUT_SEQ(WRITE_SDR, FLEXSPI_1PAD, 0x04  /* data out */,
                                     STOP,      FLEXSPI_1PAD, 0),
                     TWO_EMPTY_STEPS,
                     TWO_EMPTY_STEPS),

            // 10: Empty
            EMPTY_SEQUENCE,

            // 11: ROM: Chip erase
            SEQUENCE(FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x60  /* the command to send */,
                                     STOP,    FLEXSPI_1PAD, 0),
                     TWO_EMPTY_STEPS,
                     TWO_EMPTY_STEPS,
                     TWO_EMPTY_STEPS),

            // 12: Empty
            EMPTY_SEQUENCE,

            // 13: ROM: Read SFDP
            EMPTY_SEQUENCE,

            // 14: ROM: Restore no cmd
            EMPTY_SEQUENCE,

            // 15: ROM: Dummy
            EMPTY_SEQUENCE
                },
        },
    .pageSize           = 256u,
    .sectorSize         = 4u * 1024u,
    .blockSize          = 64u * 1024u,
    .isUniformBlockSize = false,
};
#endif /* XIP_BOOT_HEADER_ENABLE */
