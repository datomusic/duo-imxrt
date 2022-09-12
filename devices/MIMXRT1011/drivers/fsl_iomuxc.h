/*
 * Copyright 2016 Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_IOMUXC_H_
#define _FSL_IOMUXC_H_

#include "fsl_common.h"

/*!
 * @addtogroup iomuxc_driver
 * @{
 */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.iomuxc"
#endif

/*! @name Driver version */
/*@{*/
/*! @brief IOMUXC driver version 2.0.3. */
#define FSL_IOMUXC_DRIVER_VERSION (MAKE_VERSION(2, 0, 4))
/*@}*/

/*!
 * @name Pin function ID
 * The pin function ID is a tuple of \<muxRegister muxMode inputRegister inputDaisy configRegister\>
 *
 * @{
 */
#define IOMUXC_GPIO_AD_14_LPI2C1_SCL 0x401F8010U, 0x0U, 0x401F81C0U, 0x0U, 0x401F80C0U
#define IOMUXC_GPIO_AD_14_LPUART3_CTS_B 0x401F8010U, 0x1U, 0, 0, 0x401F80C0U
#define IOMUXC_GPIO_AD_14_KPP_COL00 0x401F8010U, 0x2U, 0x401F819CU, 0x0U, 0x401F80C0U
#define IOMUXC_GPIO_AD_14_LPUART4_CTS_B 0x401F8010U, 0x3U, 0, 0, 0x401F80C0U
#define IOMUXC_GPIO_AD_14_FLEXIO1_IO26 0x401F8010U, 0x4U, 0, 0, 0x401F80C0U
#define IOMUXC_GPIO_AD_14_GPIOMUX_IO28 0x401F8010U, 0x5U, 0, 0, 0x401F80C0U
#define IOMUXC_GPIO_AD_14_REF_CLK_24M 0x401F8010U, 0x6U, 0, 0, 0x401F80C0U
#define IOMUXC_GPIO_AD_14_XBAR1_INOUT02 0x401F8010U, 0x7U, 0, 0, 0x401F80C0U

#define IOMUXC_GPIO_AD_13_LPI2C1_SDA 0x401F8014U, 0x0U, 0x401F81C4U, 0x0U, 0x401F80C4U
#define IOMUXC_GPIO_AD_13_LPUART3_RTS_B 0x401F8014U, 0x1U, 0, 0, 0x401F80C4U
#define IOMUXC_GPIO_AD_13_KPP_ROW00 0x401F8014U, 0x2U, 0x401F81ACU, 0x0U, 0x401F80C4U
#define IOMUXC_GPIO_AD_13_LPUART4_RTS_B 0x401F8014U, 0x3U, 0, 0, 0x401F80C4U
#define IOMUXC_GPIO_AD_13_FLEXIO1_IO25 0x401F8014U, 0x4U, 0, 0, 0x401F80C4U
#define IOMUXC_GPIO_AD_13_GPIOMUX_IO27 0x401F8014U, 0x5U, 0, 0, 0x401F80C4U
#define IOMUXC_GPIO_AD_13_ARM_NMI 0x401F8014U, 0x6U, 0x401F8210U, 0x0U, 0x401F80C4U
#define IOMUXC_GPIO_AD_13_JTAG_TMS 0x401F8014U, 0x7U, 0, 0, 0x401F80C4U

#define IOMUXC_GPIO_AD_12_LPSPI2_SCK 0x401F8018U, 0x0U, 0x401F81E4U, 0x0U, 0x401F80C8U
#define IOMUXC_GPIO_AD_12_FLEXPWM1_PWM0_X 0x401F8018U, 0x1U, 0, 0, 0x401F80C8U
#define IOMUXC_GPIO_AD_12_KPP_COL01 0x401F8018U, 0x2U, 0x401F81A0U, 0x0U, 0x401F80C8U
#define IOMUXC_GPIO_AD_12_PIT_TRIGGER01 0x401F8018U, 0x3U, 0, 0, 0x401F80C8U
#define IOMUXC_GPIO_AD_12_FLEXIO1_IO24 0x401F8018U, 0x4U, 0, 0, 0x401F80C8U
#define IOMUXC_GPIO_AD_12_GPIOMUX_IO26 0x401F8018U, 0x5U, 0, 0, 0x401F80C8U
#define IOMUXC_GPIO_AD_12_USB_OTG1_PWR 0x401F8018U, 0x6U, 0, 0, 0x401F80C8U
#define IOMUXC_GPIO_AD_12_JTAG_TCK 0x401F8018U, 0x7U, 0, 0, 0x401F80C8U

#define IOMUXC_GPIO_AD_11_LPSPI2_PCS0 0x401F801CU, 0x0U, 0x401F81E0U, 0x0U, 0x401F80CCU
#define IOMUXC_GPIO_AD_11_FLEXPWM1_PWM1_X 0x401F801CU, 0x1U, 0, 0, 0x401F80CCU
#define IOMUXC_GPIO_AD_11_KPP_ROW01 0x401F801CU, 0x2U, 0x401F81B0U, 0x0U, 0x401F80CCU
#define IOMUXC_GPIO_AD_11_PIT_TRIGGER02 0x401F801CU, 0x3U, 0, 0, 0x401F80CCU
#define IOMUXC_GPIO_AD_11_FLEXIO1_IO23 0x401F801CU, 0x4U, 0, 0, 0x401F80CCU
#define IOMUXC_GPIO_AD_11_GPIOMUX_IO25 0x401F801CU, 0x5U, 0, 0, 0x401F80CCU
#define IOMUXC_GPIO_AD_11_WDOG1_B 0x401F801CU, 0x6U, 0, 0, 0x401F80CCU
#define IOMUXC_GPIO_AD_11_JTAG_MOD 0x401F801CU, 0x7U, 0, 0, 0x401F80CCU

#define IOMUXC_GPIO_AD_10_LPSPI2_SDO 0x401F8020U, 0x0U, 0x401F81ECU, 0x0U, 0x401F80D0U
#define IOMUXC_GPIO_AD_10_FLEXPWM1_PWM2_X 0x401F8020U, 0x1U, 0, 0, 0x401F80D0U
#define IOMUXC_GPIO_AD_10_KPP_COL02 0x401F8020U, 0x2U, 0x401F81A4U, 0x0U, 0x401F80D0U
#define IOMUXC_GPIO_AD_10_PIT_TRIGGER03 0x401F8020U, 0x3U, 0, 0, 0x401F80D0U
#define IOMUXC_GPIO_AD_10_FLEXIO1_IO22 0x401F8020U, 0x4U, 0, 0, 0x401F80D0U
#define IOMUXC_GPIO_AD_10_GPIOMUX_IO24 0x401F8020U, 0x5U, 0, 0, 0x401F80D0U
#define IOMUXC_GPIO_AD_10_USB_OTG1_ID 0x401F8020U, 0x6U, 0x401F8170U, 0x0U, 0x401F80D0U
#define IOMUXC_GPIO_AD_10_JTAG_TDI 0x401F8020U, 0x7U, 0, 0, 0x401F80D0U

#define IOMUXC_GPIO_AD_09_LPSPI2_SDI 0x401F8024U, 0x0U, 0x401F81E8U, 0x0U, 0x401F80D4U
#define IOMUXC_GPIO_AD_09_FLEXPWM1_PWM3_X 0x401F8024U, 0x1U, 0, 0, 0x401F80D4U
#define IOMUXC_GPIO_AD_09_KPP_ROW02 0x401F8024U, 0x2U, 0x401F81B4U, 0x0U, 0x401F80D4U
#define IOMUXC_GPIO_AD_09_ARM_TRACE_SWO 0x401F8024U, 0x3U, 0, 0, 0x401F80D4U
#define IOMUXC_GPIO_AD_09_FLEXIO1_IO21 0x401F8024U, 0x4U, 0, 0, 0x401F80D4U
#define IOMUXC_GPIO_AD_09_GPIOMUX_IO23 0x401F8024U, 0x5U, 0, 0, 0x401F80D4U
#define IOMUXC_GPIO_AD_09_REF_CLK_32K 0x401F8024U, 0x6U, 0, 0, 0x401F80D4U
#define IOMUXC_GPIO_AD_09_JTAG_TDO 0x401F8024U, 0x7U, 0, 0, 0x401F80D4U

#define IOMUXC_GPIO_AD_08_LPI2C2_SCL 0x401F8028U, 0x0U, 0x401F81C8U, 0x0U, 0x401F80D8U
#define IOMUXC_GPIO_AD_08_LPUART3_TXD 0x401F8028U, 0x1U, 0x401F8204U, 0x0U, 0x401F80D8U
#define IOMUXC_GPIO_AD_08_ARM_CM7_TXEV 0x401F8028U, 0x2U, 0, 0, 0x401F80D8U
#define IOMUXC_GPIO_AD_08_LPUART2_CTS_B 0x401F8028U, 0x3U, 0, 0, 0x401F80D8U
#define IOMUXC_GPIO_AD_08_GPT2_COMPARE3 0x401F8028U, 0x4U, 0, 0, 0x401F80D8U
#define IOMUXC_GPIO_AD_08_GPIOMUX_IO22 0x401F8028U, 0x5U, 0, 0, 0x401F80D8U
#define IOMUXC_GPIO_AD_08_EWM_OUT_B 0x401F8028U, 0x6U, 0, 0, 0x401F80D8U
#define IOMUXC_GPIO_AD_08_JTAG_TRSTB 0x401F8028U, 0x7U, 0, 0, 0x401F80D8U

#define IOMUXC_GPIO_AD_07_LPI2C2_SDA 0x401F802CU, 0x0U, 0x401F81CCU, 0x0U, 0x401F80DCU
#define IOMUXC_GPIO_AD_07_LPUART3_RXD 0x401F802CU, 0x1U, 0x401F8200U, 0x0U, 0x401F80DCU
#define IOMUXC_GPIO_AD_07_ARM_CM7_RXEV 0x401F802CU, 0x2U, 0x401F8220U, 0x0U, 0x401F80DCU
#define IOMUXC_GPIO_AD_07_LPUART2_RTS_B 0x401F802CU, 0x3U, 0, 0, 0x401F80DCU
#define IOMUXC_GPIO_AD_07_GPT2_CAPTURE2 0x401F802CU, 0x4U, 0, 0, 0x401F80DCU
#define IOMUXC_GPIO_AD_07_GPIOMUX_IO21 0x401F802CU, 0x5U, 0, 0, 0x401F80DCU
#define IOMUXC_GPIO_AD_07_OCOTP_FUSE_LATCHED 0x401F802CU, 0x6U, 0, 0, 0x401F80DCU
#define IOMUXC_GPIO_AD_07_XBAR1_INOUT03 0x401F802CU, 0x7U, 0, 0, 0x401F80DCU

#define IOMUXC_GPIO_AD_06_LPSPI1_SCK 0x401F8030U, 0x0U, 0x401F81D4U, 0x0U, 0x401F80E0U
#define IOMUXC_GPIO_AD_06_PIT_TRIGGER00 0x401F8030U, 0x1U, 0, 0, 0x401F80E0U
#define IOMUXC_GPIO_AD_06_FLEXPWM1_PWM3_A 0x401F8030U, 0x2U, 0x401F8180U, 0x0U, 0x401F80E0U
#define IOMUXC_GPIO_AD_06_KPP_COL01 0x401F8030U, 0x3U, 0x401F81A0U, 0x1U, 0x401F80E0U
#define IOMUXC_GPIO_AD_06_GPT2_COMPARE2 0x401F8030U, 0x4U, 0, 0, 0x401F80E0U
#define IOMUXC_GPIO_AD_06_GPIOMUX_IO20 0x401F8030U, 0x5U, 0, 0, 0x401F80E0U
#define IOMUXC_GPIO_AD_06_LPI2C1_HREQ 0x401F8030U, 0x6U, 0x401F81BCU, 0x0U, 0x401F80E0U

#define IOMUXC_GPIO_AD_05_LPSPI1_PCS0 0x401F8034U, 0x0U, 0x401F81D0U, 0x0U, 0x401F80E4U
#define IOMUXC_GPIO_AD_05_PIT_TRIGGER01 0x401F8034U, 0x1U, 0, 0, 0x401F80E4U
#define IOMUXC_GPIO_AD_05_FLEXPWM1_PWM3_B 0x401F8034U, 0x2U, 0x401F8190U, 0x0U, 0x401F80E4U
#define IOMUXC_GPIO_AD_05_KPP_ROW01 0x401F8034U, 0x3U, 0x401F81B0U, 0x1U, 0x401F80E4U
#define IOMUXC_GPIO_AD_05_GPT2_CAPTURE1 0x401F8034U, 0x4U, 0, 0, 0x401F80E4U
#define IOMUXC_GPIO_AD_05_GPIOMUX_IO19 0x401F8034U, 0x5U, 0, 0, 0x401F80E4U

#define IOMUXC_GPIO_AD_04_LPSPI1_SDO 0x401F8038U, 0x0U, 0x401F81DCU, 0x0U, 0x401F80E8U
#define IOMUXC_GPIO_AD_04_PIT_TRIGGER02 0x401F8038U, 0x1U, 0, 0, 0x401F80E8U
#define IOMUXC_GPIO_AD_04_FLEXPWM1_PWM2_A 0x401F8038U, 0x2U, 0x401F817CU, 0x0U, 0x401F80E8U
#define IOMUXC_GPIO_AD_04_KPP_COL02 0x401F8038U, 0x3U, 0x401F81A4U, 0x1U, 0x401F80E8U
#define IOMUXC_GPIO_AD_04_GPT2_COMPARE1 0x401F8038U, 0x4U, 0, 0, 0x401F80E8U
#define IOMUXC_GPIO_AD_04_GPIOMUX_IO18 0x401F8038U, 0x5U, 0, 0, 0x401F80E8U
#define IOMUXC_GPIO_AD_04_SNVS_VIO_5_CTL 0x401F8038U, 0x6U, 0, 0, 0x401F80E8U

#define IOMUXC_GPIO_AD_03_LPSPI1_SDI 0x401F803CU, 0x0U, 0x401F81D8U, 0x0U, 0x401F80ECU
#define IOMUXC_GPIO_AD_03_PIT_TRIGGER03 0x401F803CU, 0x1U, 0, 0, 0x401F80ECU
#define IOMUXC_GPIO_AD_03_FLEXPWM1_PWM2_B 0x401F803CU, 0x2U, 0x401F818CU, 0x0U, 0x401F80ECU
#define IOMUXC_GPIO_AD_03_KPP_ROW02 0x401F803CU, 0x3U, 0x401F81B4U, 0x1U, 0x401F80ECU
#define IOMUXC_GPIO_AD_03_GPT2_CLK 0x401F803CU, 0x4U, 0, 0, 0x401F80ECU
#define IOMUXC_GPIO_AD_03_GPIOMUX_IO17 0x401F803CU, 0x5U, 0, 0, 0x401F80ECU
#define IOMUXC_GPIO_AD_03_SNVS_VIO_5_B 0x401F803CU, 0x6U, 0, 0, 0x401F80ECU
#define IOMUXC_GPIO_AD_03_JTAG_DE_B 0x401F803CU, 0x7U, 0, 0, 0x401F80ECU

#define IOMUXC_GPIO_AD_02_LPUART4_TXD 0x401F8040U, 0x0U, 0x401F820CU, 0x0U, 0x401F80F0U
#define IOMUXC_GPIO_AD_02_LPSPI1_PCS1 0x401F8040U, 0x1U, 0, 0, 0x401F80F0U
#define IOMUXC_GPIO_AD_02_WDOG2_B 0x401F8040U, 0x2U, 0, 0, 0x401F80F0U
#define IOMUXC_GPIO_AD_02_LPI2C2_SCL 0x401F8040U, 0x3U, 0x401F81C8U, 0x1U, 0x401F80F0U
#define IOMUXC_GPIO_AD_02_MQS_RIGHT 0x401F8040U, 0x4U, 0, 0, 0x401F80F0U
#define IOMUXC_GPIO_AD_02_GPIOMUX_IO16 0x401F8040U, 0x5U, 0, 0, 0x401F80F0U
#define IOMUXC_GPIO_AD_02_ARM_TRACE_CLK 0x401F8040U, 0x7U, 0, 0, 0x401F80F0U

#define IOMUXC_GPIO_AD_01_LPUART4_RXD 0x401F8044U, 0x0U, 0x401F8208U, 0x0U, 0x401F80F4U
#define IOMUXC_GPIO_AD_01_LPSPI2_PCS1 0x401F8044U, 0x1U, 0, 0, 0x401F80F4U
#define IOMUXC_GPIO_AD_01_WDOG1_ANY 0x401F8044U, 0x2U, 0, 0, 0x401F80F4U
#define IOMUXC_GPIO_AD_01_LPI2C2_SDA 0x401F8044U, 0x3U, 0x401F81CCU, 0x1U, 0x401F80F4U
#define IOMUXC_GPIO_AD_01_MQS_LEFT 0x401F8044U, 0x4U, 0, 0, 0x401F80F4U
#define IOMUXC_GPIO_AD_01_GPIOMUX_IO15 0x401F8044U, 0x5U, 0, 0, 0x401F80F4U
#define IOMUXC_GPIO_AD_01_USB_OTG1_OC 0x401F8044U, 0x6U, 0x401F821CU, 0x0U, 0x401F80F4U
#define IOMUXC_GPIO_AD_01_ARM_TRACE_SWO 0x401F8044U, 0x7U, 0, 0, 0x401F80F4U

#define IOMUXC_GPIO_AD_00_LPUART2_TXD 0x401F8048U, 0x0U, 0x401F81FCU, 0x0U, 0x401F80F8U
#define IOMUXC_GPIO_AD_00_LPSPI1_PCS2 0x401F8048U, 0x1U, 0, 0, 0x401F80F8U
#define IOMUXC_GPIO_AD_00_KPP_COL03 0x401F8048U, 0x2U, 0x401F81A8U, 0x0U, 0x401F80F8U
#define IOMUXC_GPIO_AD_00_USB_OTG1_PWR 0x401F8048U, 0x3U, 0, 0, 0x401F80F8U
#define IOMUXC_GPIO_AD_00_FLEXIO1_IO20 0x401F8048U, 0x4U, 0, 0, 0x401F80F8U
#define IOMUXC_GPIO_AD_00_GPIOMUX_IO14 0x401F8048U, 0x5U, 0, 0, 0x401F80F8U
#define IOMUXC_GPIO_AD_00_ARM_NMI 0x401F8048U, 0x6U, 0x401F8210U, 0x1U, 0x401F80F8U
#define IOMUXC_GPIO_AD_00_ARM_TRACE0 0x401F8048U, 0x7U, 0, 0, 0x401F80F8U

#define IOMUXC_GPIO_SD_14_FLEXSPI_A_DQS 0x401F804CU, 0x0U, 0x401F8194U, 0x0U, 0x401F80FCU
#define IOMUXC_GPIO_SD_14_FLEXSPI_B_DQS 0x401F804CU, 0x1U, 0x401F8198U, 0x0U, 0x401F80FCU

#define IOMUXC_GPIO_SD_13_FLEXSPI_B_SCLK 0x401F8050U, 0x0U, 0, 0, 0x401F8100U
#define IOMUXC_GPIO_SD_13_SAI3_RX_BCLK 0x401F8050U, 0x1U, 0, 0, 0x401F8100U
#define IOMUXC_GPIO_SD_13_ARM_CM7_TXEV 0x401F8050U, 0x2U, 0, 0, 0x401F8100U
#define IOMUXC_GPIO_SD_13_CCM_PMIC_RDY 0x401F8050U, 0x3U, 0, 0, 0x401F8100U
#define IOMUXC_GPIO_SD_13_FLEXIO1_IO19 0x401F8050U, 0x4U, 0, 0, 0x401F8100U
#define IOMUXC_GPIO_SD_13_GPIO2_IO13 0x401F8050U, 0x5U, 0, 0, 0x401F8100U
#define IOMUXC_GPIO_SD_13_SRC_BT_CFG03 0x401F8050U, 0x6U, 0, 0, 0x401F8100U

#define IOMUXC_GPIO_SD_12_FLEXSPI_A_DQS 0x401F8054U, 0x0U, 0x401F8194U, 0x1U, 0x401F8104U
#define IOMUXC_GPIO_SD_12_LPSPI2_PCS0 0x401F8054U, 0x1U, 0x401F81E0U, 0x1U, 0x401F8104U
#define IOMUXC_GPIO_SD_12_LPUART1_TXD 0x401F8054U, 0x2U, 0x401F81F4U, 0x0U, 0x401F8104U
#define IOMUXC_GPIO_SD_12_FLEXIO1_IO18 0x401F8054U, 0x4U, 0, 0, 0x401F8104U
#define IOMUXC_GPIO_SD_12_GPIO2_IO12 0x401F8054U, 0x5U, 0, 0, 0x401F8104U
#define IOMUXC_GPIO_SD_12_WDOG2_RST_B_DEB 0x401F8054U, 0x6U, 0, 0, 0x401F8104U

#define IOMUXC_GPIO_SD_11_FLEXSPI_A_DATA3 0x401F8058U, 0x0U, 0, 0, 0x401F8108U
#define IOMUXC_GPIO_SD_11_LPSPI2_SCK 0x401F8058U, 0x1U, 0x401F81E4U, 0x1U, 0x401F8108U
#define IOMUXC_GPIO_SD_11_LPUART1_RXD 0x401F8058U, 0x2U, 0x401F81F0U, 0x0U, 0x401F8108U
#define IOMUXC_GPIO_SD_11_FLEXIO1_IO17 0x401F8058U, 0x4U, 0, 0, 0x401F8108U
#define IOMUXC_GPIO_SD_11_GPIO2_IO11 0x401F8058U, 0x5U, 0, 0, 0x401F8108U
#define IOMUXC_GPIO_SD_11_WDOG1_RST_B_DEB 0x401F8058U, 0x6U, 0, 0, 0x401F8108U

#define IOMUXC_GPIO_SD_10_FLEXSPI_A_SCLK 0x401F805CU, 0x0U, 0, 0, 0x401F810CU
#define IOMUXC_GPIO_SD_10_LPSPI2_SDO 0x401F805CU, 0x1U, 0x401F81ECU, 0x1U, 0x401F810CU
#define IOMUXC_GPIO_SD_10_LPUART2_TXD 0x401F805CU, 0x2U, 0x401F81FCU, 0x1U, 0x401F810CU
#define IOMUXC_GPIO_SD_10_FLEXIO1_IO16 0x401F805CU, 0x4U, 0, 0, 0x401F810CU
#define IOMUXC_GPIO_SD_10_GPIO2_IO10 0x401F805CU, 0x5U, 0, 0, 0x401F810CU

#define IOMUXC_GPIO_SD_09_FLEXSPI_A_DATA0 0x401F8060U, 0x0U, 0, 0, 0x401F8110U
#define IOMUXC_GPIO_SD_09_LPSPI2_SDI 0x401F8060U, 0x1U, 0x401F81E8U, 0x1U, 0x401F8110U
#define IOMUXC_GPIO_SD_09_LPUART2_RXD 0x401F8060U, 0x2U, 0x401F81F8U, 0x0U, 0x401F8110U
#define IOMUXC_GPIO_SD_09_FLEXIO1_IO15 0x401F8060U, 0x4U, 0, 0, 0x401F8110U
#define IOMUXC_GPIO_SD_09_GPIO2_IO09 0x401F8060U, 0x5U, 0, 0, 0x401F8110U

#define IOMUXC_GPIO_SD_08_FLEXSPI_A_DATA2 0x401F8064U, 0x0U, 0, 0, 0x401F8114U
#define IOMUXC_GPIO_SD_08_LPI2C2_SCL 0x401F8064U, 0x1U, 0x401F81C8U, 0x2U, 0x401F8114U
#define IOMUXC_GPIO_SD_08_LPSPI1_SCK 0x401F8064U, 0x2U, 0x401F81D4U, 0x1U, 0x401F8114U
#define IOMUXC_GPIO_SD_08_FLEXIO1_IO14 0x401F8064U, 0x4U, 0, 0, 0x401F8114U
#define IOMUXC_GPIO_SD_08_GPIO2_IO08 0x401F8064U, 0x5U, 0, 0, 0x401F8114U

#define IOMUXC_GPIO_SD_07_FLEXSPI_A_DATA1 0x401F8068U, 0x0U, 0, 0, 0x401F8118U
#define IOMUXC_GPIO_SD_07_LPI2C2_SDA 0x401F8068U, 0x1U, 0x401F81CCU, 0x2U, 0x401F8118U
#define IOMUXC_GPIO_SD_07_LPSPI1_PCS0 0x401F8068U, 0x2U, 0x401F81D0U, 0x1U, 0x401F8118U
#define IOMUXC_GPIO_SD_07_FLEXIO1_IO13 0x401F8068U, 0x4U, 0, 0, 0x401F8118U
#define IOMUXC_GPIO_SD_07_GPIO2_IO07 0x401F8068U, 0x5U, 0, 0, 0x401F8118U

#define IOMUXC_GPIO_SD_06_FLEXSPI_A_SS0_B 0x401F806CU, 0x0U, 0, 0, 0x401F811CU
#define IOMUXC_GPIO_SD_06_LPI2C1_SCL 0x401F806CU, 0x1U, 0x401F81C0U, 0x1U, 0x401F811CU
#define IOMUXC_GPIO_SD_06_LPSPI1_SDO 0x401F806CU, 0x2U, 0x401F81DCU, 0x1U, 0x401F811CU
#define IOMUXC_GPIO_SD_06_FLEXIO1_IO12 0x401F806CU, 0x4U, 0, 0, 0x401F811CU
#define IOMUXC_GPIO_SD_06_GPIO2_IO06 0x401F806CU, 0x5U, 0, 0, 0x401F811CU

#define IOMUXC_GPIO_SD_05_FLEXSPI_A_SS1_B 0x401F8070U, 0x0U, 0, 0, 0x401F8120U
#define IOMUXC_GPIO_SD_05_LPI2C1_SDA 0x401F8070U, 0x1U, 0x401F81C4U, 0x1U, 0x401F8120U
#define IOMUXC_GPIO_SD_05_LPSPI1_SDI 0x401F8070U, 0x2U, 0x401F81D8U, 0x1U, 0x401F8120U
#define IOMUXC_GPIO_SD_05_FLEXIO1_IO11 0x401F8070U, 0x4U, 0, 0, 0x401F8120U
#define IOMUXC_GPIO_SD_05_GPIO2_IO05 0x401F8070U, 0x5U, 0, 0, 0x401F8120U

#define IOMUXC_GPIO_SD_04_FLEXPWM1_PWM1_A 0x401F8074U, 0x2U, 0x401F8178U, 0x0U, 0x401F8124U
#define IOMUXC_GPIO_SD_04_CCM_WAIT 0x401F8074U, 0x3U, 0, 0, 0x401F8124U
#define IOMUXC_GPIO_SD_04_FLEXIO1_IO10 0x401F8074U, 0x4U, 0, 0, 0x401F8124U
#define IOMUXC_GPIO_SD_04_GPIO2_IO04 0x401F8074U, 0x5U, 0, 0, 0x401F8124U
#define IOMUXC_GPIO_SD_04_SRC_BOOT_MODE00 0x401F8074U, 0x6U, 0, 0, 0x401F8124U
#define IOMUXC_GPIO_SD_04_FLEXSPI_B_DATA03 0x401F8074U, 0x0U, 0, 0, 0x401F8124U
#define IOMUXC_GPIO_SD_04_SAI3_RX_SYNC 0x401F8074U, 0x1U, 0, 0, 0x401F8124U

#define IOMUXC_GPIO_SD_03_FLEXSPI_B_DATA00 0x401F8078U, 0x0U, 0, 0, 0x401F8128U
#define IOMUXC_GPIO_SD_03_SAI3_RX_DATA 0x401F8078U, 0x1U, 0, 0, 0x401F8128U
#define IOMUXC_GPIO_SD_03_FLEXPWM1_PWM1_B 0x401F8078U, 0x2U, 0x401F8188U, 0x0U, 0x401F8128U
#define IOMUXC_GPIO_SD_03_CCM_REF_EN_B 0x401F8078U, 0x3U, 0, 0, 0x401F8128U
#define IOMUXC_GPIO_SD_03_FLEXIO1_IO09 0x401F8078U, 0x4U, 0, 0, 0x401F8128U
#define IOMUXC_GPIO_SD_03_GPIO2_IO03 0x401F8078U, 0x5U, 0, 0, 0x401F8128U
#define IOMUXC_GPIO_SD_03_SRC_BOOT_MODE01 0x401F8078U, 0x6U, 0, 0, 0x401F8128U

#define IOMUXC_GPIO_SD_02_FLEXSPI_B_DATA02 0x401F807CU, 0x0U, 0, 0, 0x401F812CU
#define IOMUXC_GPIO_SD_02_SAI3_TX_DATA 0x401F807CU, 0x1U, 0, 0, 0x401F812CU
#define IOMUXC_GPIO_SD_02_FLEXPWM1_PWM0_A 0x401F807CU, 0x2U, 0x401F8174U, 0x0U, 0x401F812CU
#define IOMUXC_GPIO_SD_02_CCM_CLKO1 0x401F807CU, 0x3U, 0, 0, 0x401F812CU
#define IOMUXC_GPIO_SD_02_FLEXIO1_IO08 0x401F807CU, 0x4U, 0, 0, 0x401F812CU
#define IOMUXC_GPIO_SD_02_GPIO2_IO02 0x401F807CU, 0x5U, 0, 0, 0x401F812CU
#define IOMUXC_GPIO_SD_02_SRC_BT_CFG00 0x401F807CU, 0x6U, 0, 0, 0x401F812CU

#define IOMUXC_GPIO_SD_01_FLEXSPI_B_DATA01 0x401F8080U, 0x0U, 0, 0, 0x401F8130U
#define IOMUXC_GPIO_SD_01_SAI3_TX_BCLK 0x401F8080U, 0x1U, 0, 0, 0x401F8130U
#define IOMUXC_GPIO_SD_01_FLEXPWM1_PWM0_B 0x401F8080U, 0x2U, 0x401F8184U, 0x0U, 0x401F8130U
#define IOMUXC_GPIO_SD_01_CCM_CLKO2 0x401F8080U, 0x3U, 0, 0, 0x401F8130U
#define IOMUXC_GPIO_SD_01_FLEXIO1_IO07 0x401F8080U, 0x4U, 0, 0, 0x401F8130U
#define IOMUXC_GPIO_SD_01_GPIO2_IO01 0x401F8080U, 0x5U, 0, 0, 0x401F8130U
#define IOMUXC_GPIO_SD_01_SRC_BT_CFG01 0x401F8080U, 0x6U, 0, 0, 0x401F8130U

#define IOMUXC_GPIO_SD_00_FLEXSPI_B_SS0_B 0x401F8084U, 0x0U, 0, 0, 0x401F8134U
#define IOMUXC_GPIO_SD_00_SAI3_TX_SYNC 0x401F8084U, 0x1U, 0, 0, 0x401F8134U
#define IOMUXC_GPIO_SD_00_ARM_CM7_RXEV 0x401F8084U, 0x2U, 0x401F8220U, 0x1U, 0x401F8134U
#define IOMUXC_GPIO_SD_00_CCM_STOP 0x401F8084U, 0x3U, 0, 0, 0x401F8134U
#define IOMUXC_GPIO_SD_00_FLEXIO1_IO06 0x401F8084U, 0x4U, 0, 0, 0x401F8134U
#define IOMUXC_GPIO_SD_00_GPIO2_IO00 0x401F8084U, 0x5U, 0, 0, 0x401F8134U
#define IOMUXC_GPIO_SD_00_SRC_BT_CFG02 0x401F8084U, 0x6U, 0, 0, 0x401F8134U

#define IOMUXC_GPIO_13_LPUART2_RXD 0x401F8088U, 0x0U, 0x401F81F8U, 0x1U, 0x401F8138U
#define IOMUXC_GPIO_13_LPSPI2_PCS2 0x401F8088U, 0x1U, 0, 0, 0x401F8138U
#define IOMUXC_GPIO_13_KPP_ROW03 0x401F8088U, 0x2U, 0x401F81B8U, 0x0U, 0x401F8138U
#define IOMUXC_GPIO_13_USB_OTG1_ID 0x401F8088U, 0x3U, 0x401F8170U, 0x1U, 0x401F8138U
#define IOMUXC_GPIO_13_FLEXIO1_IO05 0x401F8088U, 0x4U, 0, 0, 0x401F8138U
#define IOMUXC_GPIO_13_GPIOMUX_IO13 0x401F8088U, 0x5U, 0, 0, 0x401F8138U
#define IOMUXC_GPIO_13_SPDIF_LOCK 0x401F8088U, 0x6U, 0, 0, 0x401F8138U
#define IOMUXC_GPIO_13_ARM_TRACE1 0x401F8088U, 0x7U, 0, 0, 0x401F8138U

#define IOMUXC_GPIO_12_LPUART3_TXD 0x401F808CU, 0x0U, 0x401F8204U, 0x1U, 0x401F813CU
#define IOMUXC_GPIO_12_LPI2C1_SCL 0x401F808CU, 0x1U, 0x401F81C0U, 0x2U, 0x401F813CU
#define IOMUXC_GPIO_12_KPP_COL00 0x401F808CU, 0x2U, 0x401F819CU, 0x1U, 0x401F813CU
#define IOMUXC_GPIO_12_USB_OTG1_OC 0x401F808CU, 0x3U, 0x401F821CU, 0x1U, 0x401F813CU
#define IOMUXC_GPIO_12_FLEXIO1_IO04 0x401F808CU, 0x4U, 0, 0, 0x401F813CU
#define IOMUXC_GPIO_12_GPIOMUX_IO12 0x401F808CU, 0x5U, 0, 0, 0x401F813CU
#define IOMUXC_GPIO_12_SPDIF_EXT_CLK 0x401F808CU, 0x6U, 0x401F8218U, 0x0U, 0x401F813CU
#define IOMUXC_GPIO_12_ARM_TRACE2 0x401F808CU, 0x7U, 0, 0, 0x401F813CU

#define IOMUXC_GPIO_11_LPUART3_RXD 0x401F8090U, 0x0U, 0x401F8200U, 0x1U, 0x401F8140U
#define IOMUXC_GPIO_11_LPI2C1_SDA 0x401F8090U, 0x1U, 0x401F81C4U, 0x2U, 0x401F8140U
#define IOMUXC_GPIO_11_KPP_ROW00 0x401F8090U, 0x2U, 0x401F81ACU, 0x1U, 0x401F8140U
#define IOMUXC_GPIO_11_FLEXSPI_B_SS1_B 0x401F8090U, 0x3U, 0, 0, 0x401F8140U
#define IOMUXC_GPIO_11_FLEXIO1_IO03 0x401F8090U, 0x4U, 0, 0, 0x401F8140U
#define IOMUXC_GPIO_11_GPIOMUX_IO11 0x401F8090U, 0x5U, 0, 0, 0x401F8140U
#define IOMUXC_GPIO_11_SPDIF_OUT 0x401F8090U, 0x6U, 0, 0, 0x401F8140U
#define IOMUXC_GPIO_11_ARM_TRACE3 0x401F8090U, 0x7U, 0, 0, 0x401F8140U

#define IOMUXC_GPIO_10_LPUART1_TXD 0x401F8094U, 0x0U, 0x401F81F4U, 0x1U, 0x401F8144U
#define IOMUXC_GPIO_10_LPI2C1_HREQ 0x401F8094U, 0x1U, 0x401F81BCU, 0x1U, 0x401F8144U
#define IOMUXC_GPIO_10_EWM_OUT_B 0x401F8094U, 0x2U, 0, 0, 0x401F8144U
#define IOMUXC_GPIO_10_LPI2C2_SCL 0x401F8094U, 0x3U, 0x401F81C8U, 0x3U, 0x401F8144U
#define IOMUXC_GPIO_10_FLEXIO1_IO02 0x401F8094U, 0x4U, 0, 0, 0x401F8144U
#define IOMUXC_GPIO_10_GPIOMUX_IO10 0x401F8094U, 0x5U, 0, 0, 0x401F8144U
#define IOMUXC_GPIO_10_SPDIF_IN 0x401F8094U, 0x6U, 0x401F8214U, 0x0U, 0x401F8144U

#define IOMUXC_GPIO_09_LPUART1_RXD 0x401F8098U, 0x0U, 0x401F81F0U, 0x1U, 0x401F8148U
#define IOMUXC_GPIO_09_WDOG1_B 0x401F8098U, 0x1U, 0, 0, 0x401F8148U
#define IOMUXC_GPIO_09_FLEXSPI_A_SS1_B 0x401F8098U, 0x2U, 0, 0, 0x401F8148U
#define IOMUXC_GPIO_09_LPI2C2_SDA 0x401F8098U, 0x3U, 0x401F81CCU, 0x3U, 0x401F8148U
#define IOMUXC_GPIO_09_FLEXIO1_IO01 0x401F8098U, 0x4U, 0, 0, 0x401F8148U
#define IOMUXC_GPIO_09_GPIOMUX_IO09 0x401F8098U, 0x5U, 0, 0, 0x401F8148U
#define IOMUXC_GPIO_09_SPDIF_SR_CLK 0x401F8098U, 0x6U, 0, 0, 0x401F8148U

#define IOMUXC_GPIO_08_SAI1_MCLK 0x401F809CU, 0x0U, 0, 0, 0x401F814CU
#define IOMUXC_GPIO_08_GPT1_CLK 0x401F809CU, 0x1U, 0, 0, 0x401F814CU
#define IOMUXC_GPIO_08_FLEXPWM1_PWM3_A 0x401F809CU, 0x2U, 0x401F8180U, 0x1U, 0x401F814CU
#define IOMUXC_GPIO_08_LPUART3_TXD 0x401F809CU, 0x3U, 0x401F8204U, 0x2U, 0x401F814CU
#define IOMUXC_GPIO_08_FLEXIO1_IO00 0x401F809CU, 0x4U, 0, 0, 0x401F814CU
#define IOMUXC_GPIO_08_GPIOMUX_IO08 0x401F809CU, 0x5U, 0, 0, 0x401F814CU
#define IOMUXC_GPIO_08_LPUART1_CTS_B 0x401F809CU, 0x6U, 0, 0, 0x401F814CU

#define IOMUXC_GPIO_07_SAI1_TX_SYNC 0x401F80A0U, 0x0U, 0, 0, 0x401F8150U
#define IOMUXC_GPIO_07_GPT1_COMPARE1 0x401F80A0U, 0x1U, 0, 0, 0x401F8150U
#define IOMUXC_GPIO_07_FLEXPWM1_PWM3_B 0x401F80A0U, 0x2U, 0x401F8190U, 0x1U, 0x401F8150U
#define IOMUXC_GPIO_07_LPUART3_RXD 0x401F80A0U, 0x3U, 0x401F8200U, 0x2U, 0x401F8150U
#define IOMUXC_GPIO_07_SPDIF_LOCK 0x401F80A0U, 0x4U, 0, 0, 0x401F8150U
#define IOMUXC_GPIO_07_GPIOMUX_IO07 0x401F80A0U, 0x5U, 0, 0, 0x401F8150U
#define IOMUXC_GPIO_07_LPUART1_RTS_B 0x401F80A0U, 0x6U, 0, 0, 0x401F8150U

#define IOMUXC_GPIO_06_SAI1_TX_BCLK 0x401F80A4U, 0x0U, 0, 0, 0x401F8154U
#define IOMUXC_GPIO_06_GPT1_CAPTURE1 0x401F80A4U, 0x1U, 0, 0, 0x401F8154U
#define IOMUXC_GPIO_06_FLEXPWM1_PWM2_A 0x401F80A4U, 0x2U, 0x401F817CU, 0x1U, 0x401F8154U
#define IOMUXC_GPIO_06_LPUART4_TXD 0x401F80A4U, 0x3U, 0x401F820CU, 0x1U, 0x401F8154U
#define IOMUXC_GPIO_06_SPDIF_EXT_CLK 0x401F80A4U, 0x4U, 0x401F8218U, 0x1U, 0x401F8154U
#define IOMUXC_GPIO_06_GPIOMUX_IO06 0x401F80A4U, 0x5U, 0, 0, 0x401F8154U

#define IOMUXC_GPIO_05_SAI1_TX_DATA01 0x401F80A8U, 0x0U, 0, 0, 0x401F8158U
#define IOMUXC_GPIO_05_GPT1_COMPARE2 0x401F80A8U, 0x1U, 0, 0, 0x401F8158U
#define IOMUXC_GPIO_05_FLEXPWM1_PWM2_B 0x401F80A8U, 0x2U, 0x401F818CU, 0x1U, 0x401F8158U
#define IOMUXC_GPIO_05_LPUART4_RXD 0x401F80A8U, 0x3U, 0x401F8208U, 0x1U, 0x401F8158U
#define IOMUXC_GPIO_05_SPDIF_OUT 0x401F80A8U, 0x4U, 0, 0, 0x401F8158U
#define IOMUXC_GPIO_05_GPIOMUX_IO05 0x401F80A8U, 0x5U, 0, 0, 0x401F8158U

#define IOMUXC_GPIO_04_SAI1_TX_DATA00 0x401F80ACU, 0x0U, 0, 0, 0x401F815CU
#define IOMUXC_GPIO_04_GPT1_CAPTURE2 0x401F80ACU, 0x1U, 0, 0, 0x401F815CU
#define IOMUXC_GPIO_04_FLEXPWM1_PWM1_A 0x401F80ACU, 0x2U, 0x401F8178U, 0x1U, 0x401F815CU
#define IOMUXC_GPIO_04_SPDIF_IN 0x401F80ACU, 0x4U, 0x401F8214U, 0x1U, 0x401F815CU
#define IOMUXC_GPIO_04_GPIOMUX_IO04 0x401F80ACU, 0x5U, 0, 0, 0x401F815CU

#define IOMUXC_GPIO_03_SAI1_RX_DATA00 0x401F80B0U, 0x0U, 0, 0, 0x401F8160U
#define IOMUXC_GPIO_03_GPT1_COMPARE3 0x401F80B0U, 0x1U, 0, 0, 0x401F8160U
#define IOMUXC_GPIO_03_FLEXPWM1_PWM1_B 0x401F80B0U, 0x2U, 0x401F8188U, 0x1U, 0x401F8160U
#define IOMUXC_GPIO_03_SPDIF_SR_CLK 0x401F80B0U, 0x4U, 0, 0, 0x401F8160U
#define IOMUXC_GPIO_03_GPIOMUX_IO03 0x401F80B0U, 0x5U, 0, 0, 0x401F8160U

#define IOMUXC_GPIO_02_SAI1_RX_SYNC 0x401F80B4U, 0x0U, 0, 0, 0x401F8164U
#define IOMUXC_GPIO_02_WDOG2_B 0x401F80B4U, 0x1U, 0, 0, 0x401F8164U
#define IOMUXC_GPIO_02_FLEXPWM1_PWM0_A 0x401F80B4U, 0x2U, 0x401F8174U, 0x1U, 0x401F8164U
#define IOMUXC_GPIO_02_LPI2C1_SCL 0x401F80B4U, 0x3U, 0x401F81C0U, 0x3U, 0x401F8164U
#define IOMUXC_GPIO_02_KPP_COL03 0x401F80B4U, 0x4U, 0x401F81A8U, 0x1U, 0x401F8164U
#define IOMUXC_GPIO_02_GPIOMUX_IO02 0x401F80B4U, 0x5U, 0, 0, 0x401F8164U

#define IOMUXC_GPIO_01_SAI1_RX_BCLK 0x401F80B8U, 0x0U, 0, 0, 0x401F8168U
#define IOMUXC_GPIO_01_WDOG1_ANY 0x401F80B8U, 0x1U, 0, 0, 0x401F8168U
#define IOMUXC_GPIO_01_FLEXPWM1_PWM0_B 0x401F80B8U, 0x2U, 0x401F8184U, 0x1U, 0x401F8168U
#define IOMUXC_GPIO_01_LPI2C1_SDA 0x401F80B8U, 0x3U, 0x401F81C4U, 0x3U, 0x401F8168U
#define IOMUXC_GPIO_01_KPP_ROW03 0x401F80B8U, 0x4U, 0x401F81B8U, 0x1U, 0x401F8168U
#define IOMUXC_GPIO_01_GPIOMUX_IO01 0x401F80B8U, 0x5U, 0, 0, 0x401F8168U

#define IOMUXC_GPIO_00_FLEXSPI_B_DQS 0x401F80BCU, 0x0U, 0x401F8198U, 0x1U, 0x401F816CU
#define IOMUXC_GPIO_00_SAI3_MCLK 0x401F80BCU, 0x1U, 0, 0, 0x401F816CU
#define IOMUXC_GPIO_00_LPSPI2_PCS3 0x401F80BCU, 0x2U, 0, 0, 0x401F816CU
#define IOMUXC_GPIO_00_LPSPI1_PCS3 0x401F80BCU, 0x3U, 0, 0, 0x401F816CU
#define IOMUXC_GPIO_00_PIT_TRIGGER00 0x401F80BCU, 0x4U, 0, 0, 0x401F816CU
#define IOMUXC_GPIO_00_GPIOMUX_IO00 0x401F80BCU, 0x5U, 0, 0, 0x401F816CU

#define IOMUXC_SNVS_PMIC_ON_REQ_SNVS_LP_PMIC_ON_REQ 0x400A8000U, 0x0U, 0, 0, 0x400A8010U
#define IOMUXC_SNVS_PMIC_ON_REQ_GPIO5_IO00 0x400A8000U, 0x5U, 0, 0, 0x400A8010U

#define IOMUXC_SNVS_TEST_MODE 0, 0, 0, 0, 0x400A8004U

#define IOMUXC_SNVS_POR_B 0, 0, 0, 0, 0x400A8008U

#define IOMUXC_SNVS_ONOFF 0, 0, 0, 0, 0x400A800CU

/*@}*/

#define IOMUXC_GPR_SAIMCLK_LOWBITMASK (0x7U)
#define IOMUXC_GPR_SAIMCLK_HIGHBITMASK (0x3U)

typedef enum _iomuxc_gpr_mode
{
    kIOMUXC_GPR_GlobalInterruptRequest    = IOMUXC_GPR_GPR1_GINT_MASK,
    kIOMUXC_GPR_SAI1MClkOutputDir         = IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK,
    kIOMUXC_GPR_SAI3MClkOutputDir         = IOMUXC_GPR_GPR1_SAI3_MCLK_DIR_MASK,
    kIOMUXC_GPR_ExcMonitorSlavErrResponse = IOMUXC_GPR_GPR1_EXC_MON_MASK,
    kIOMUXC_GPR_AHBClockEnable            = (int)IOMUXC_GPR_GPR1_CM7_FORCE_HCLK_EN_MASK,
} iomuxc_gpr_mode_t;

typedef enum _iomuxc_gpr_saimclk
{
    kIOMUXC_GPR_SAI1MClk1Sel = IOMUXC_GPR_GPR1_SAI1_MCLK1_SEL_SHIFT,
    kIOMUXC_GPR_SAI1MClk2Sel = IOMUXC_GPR_GPR1_SAI1_MCLK2_SEL_SHIFT,
    kIOMUXC_GPR_SAI1MClk3Sel = IOMUXC_GPR_GPR1_SAI1_MCLK3_SEL_SHIFT,
    kIOMUXC_GPR_SAI3MClk3Sel = IOMUXC_GPR_GPR1_SAI3_MCLK3_SEL_SHIFT,
} iomuxc_gpr_saimclk_t;

typedef enum _iomuxc_mqs_pwm_oversample_rate
{
    kIOMUXC_MqsPwmOverSampleRate32 = 0, /* MQS PWM over sampling rate 32. */
    kIOMUXC_MqsPwmOverSampleRate64 = 1  /* MQS PWM over sampling rate 64. */
} iomuxc_mqs_pwm_oversample_rate_t;

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus */

/*! @name Configuration */
/*@{*/

/*!
 * @brief Sets the IOMUXC pin mux mode.
 * @note The first five parameters can be filled with the pin function ID macros.
 *
 * This is an example to set the PTA6 as the lpuart0_tx:
 * @code
 * IOMUXC_SetPinMux(IOMUXC_PTA6_LPUART0_TX, 0);
 * @endcode
 *
 * This is an example to set the PTA0 as GPIOA0:
 * @code
 * IOMUXC_SetPinMux(IOMUXC_PTA0_GPIOA0, 0);
 * @endcode
 *
 * @param muxRegister  The pin mux register.
 * @param muxMode      The pin mux mode.
 * @param inputRegister The select input register.
 * @param inputDaisy   The input daisy.
 * @param configRegister  The config register.
 * @param inputOnfield   Software input on field.
 */
static inline void IOMUXC_SetPinMux(uint32_t muxRegister,
                                    uint32_t muxMode,
                                    uint32_t inputRegister,
                                    uint32_t inputDaisy,
                                    uint32_t configRegister,
                                    uint32_t inputOnfield)
{
    *((volatile uint32_t *)muxRegister) =
        IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(muxMode) | IOMUXC_SW_MUX_CTL_PAD_SION(inputOnfield);

    if (inputRegister != 0UL)
    {
        *((volatile uint32_t *)inputRegister) = inputDaisy;
    }
}

/*!
 * @brief Sets the IOMUXC pin configuration.
 * @note The previous five parameters can be filled with the pin function ID macros.
 *
 * This is an example to set pin configuration for IOMUXC_PTA3_LPI2C0_SCLS:
 * @code
 * IOMUXC_SetPinConfig(IOMUXC_PTA3_LPI2C0_SCLS,IOMUXC_SW_PAD_CTL_PAD_PUS_MASK|IOMUXC_SW_PAD_CTL_PAD_PUS(2U))
 * @endcode
 *
 * @param muxRegister  The pin mux register.
 * @param muxMode      The pin mux mode.
 * @param inputRegister The select input register.
 * @param inputDaisy   The input daisy.
 * @param configRegister  The config register.
 * @param configValue   The pin config value.
 */
static inline void IOMUXC_SetPinConfig(uint32_t muxRegister,
                                       uint32_t muxMode,
                                       uint32_t inputRegister,
                                       uint32_t inputDaisy,
                                       uint32_t configRegister,
                                       uint32_t configValue)
{
    if (configRegister != 0UL)
    {
        *((volatile uint32_t *)configRegister) = configValue;
    }
}

/*!
 * @brief Sets IOMUXC general configuration for some mode.
 *
 * @param base     The IOMUXC GPR base address.
 * @param mode     The mode for setting. the mode is the logical OR of "iomuxc_gpr_mode"
 * @param enable   True enable false disable.
 */
static inline void IOMUXC_EnableMode(IOMUXC_GPR_Type *base, uint32_t mode, bool enable)
{
    mode &= ~(IOMUXC_GPR_GPR1_SAI1_MCLK1_SEL_MASK | IOMUXC_GPR_GPR1_SAI1_MCLK2_SEL_MASK |
              IOMUXC_GPR_GPR1_SAI1_MCLK3_SEL_MASK | IOMUXC_GPR_GPR1_SAI3_MCLK3_SEL_MASK);

    if (enable)
    {
        base->GPR1 |= mode;
    }
    else
    {
        base->GPR1 &= ~mode;
    }
}

/*!
 * @brief Sets IOMUXC general configuration for SAI MCLK selection.
 *
 * @param base     The IOMUXC GPR base address.
 * @param mclk     The SAI MCLK.
 * @param clkSrc   The clock source. Take refer to register setting details for the clock source in RM.
 */
static inline void IOMUXC_SetSaiMClkClockSource(IOMUXC_GPR_Type *base, iomuxc_gpr_saimclk_t mclk, uint8_t clkSrc)
{
    uint32_t gpr;

    if (mclk > kIOMUXC_GPR_SAI1MClk2Sel)
    {
        gpr = base->GPR1 & ~((uint32_t)IOMUXC_GPR_SAIMCLK_HIGHBITMASK << (uint32_t)mclk);
        base->GPR1 = (((uint32_t)clkSrc & IOMUXC_GPR_SAIMCLK_HIGHBITMASK) << (uint32_t)mclk) | gpr;
    }
    else
    {
        gpr = base->GPR1 & ~((uint32_t)IOMUXC_GPR_SAIMCLK_LOWBITMASK << (uint32_t)mclk);
        base->GPR1 = (((uint32_t)clkSrc & IOMUXC_GPR_SAIMCLK_LOWBITMASK) << (uint32_t)mclk) | gpr;
    }
}

/*!
 * @brief Enters or exit MQS software reset.
 *
 * @param base     The IOMUXC GPR base address.
 * @param enable   Enter or exit MQS software reset.
 */
static inline void IOMUXC_MQSEnterSoftwareReset(IOMUXC_GPR_Type *base, bool enable)
{
    if (enable)
    {
        base->GPR2 |= IOMUXC_GPR_GPR2_MQS_SW_RST_MASK;
    }
    else
    {
        base->GPR2 &= ~IOMUXC_GPR_GPR2_MQS_SW_RST_MASK;
    }
}

/*!
 * @brief Enables or disables MQS.
 *
 * @param base     The IOMUXC GPR base address.
 * @param enable   Enable or disable the MQS.
 */
static inline void IOMUXC_MQSEnable(IOMUXC_GPR_Type *base, bool enable)
{
    if (enable)
    {
        base->GPR2 |= IOMUXC_GPR_GPR2_MQS_EN_MASK;
    }
    else
    {
        base->GPR2 &= ~IOMUXC_GPR_GPR2_MQS_EN_MASK;
    }
}

/*!
 * @brief Configure MQS PWM oversampling rate compared with mclk and divider ratio control for mclk from hmclk.
 *
 * @param base     The IOMUXC GPR base address.
 * @param rate     The MQS PWM oversampling rate, refer to "iomuxc_mqs_pwm_oversample_rate_t".
 * @param divider  The divider ratio control for mclk from hmclk. mclk freq = 1 /(divider + 1) * hmclk freq.
 */

static inline void IOMUXC_MQSConfig(IOMUXC_GPR_Type *base, iomuxc_mqs_pwm_oversample_rate_t rate, uint8_t divider)
{
    uint32_t gpr = base->GPR2 & ~(IOMUXC_GPR_GPR2_MQS_OVERSAMPLE_MASK | IOMUXC_GPR_GPR2_MQS_CLK_DIV_MASK);
    base->GPR2 = gpr | IOMUXC_GPR_GPR2_MQS_OVERSAMPLE(rate) | IOMUXC_GPR_GPR2_MQS_CLK_DIV(divider);
}

/*@}*/

#if defined(__cplusplus)
}
#endif /*_cplusplus */

/*! @}*/

#endif /* _FSL_IOMUXC_H_ */
