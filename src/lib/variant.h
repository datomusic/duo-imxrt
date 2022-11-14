/*
 * The MIT License (MIT)
 * 
 * Author: Hongtai Liu (lht856@foxmail.com)
 * 
 * Copyright (C) 2019  Seeed Technology Co.,Ltd. 
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef PINS_ARDUINO_DUOBRAINS2
#define PINS_ARDUINO_DUOBRAINS2

#include "WVariant.h"
#include "fsl_lpuart.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

#define USB_VID 0x16D0 // MCS Electronics
#define USB_PID 0x10A7 // DUO Brains 2 VID

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// GPIO_0
#define GPIO_00              0U
#define GPIO_01              1U
#define GPIO_02              2U
#define GPIO_03              3U
#define GPIO_04              4U
#define GPIO_05              5U
#define GPIO_06              6U
#define GPIO_07              7U
#define GPIO_08              8U
#define GPIO_09              9U
#define GPIO_10             10U
#define GPIO_11             11U
#define GPIO_12             12U
#define GPIO_13             13U

// GPIO_AD
#define GPIO_AD_00          14U
#define GPIO_AD_01          15U
#define GPIO_AD_02          16U
#define GPIO_AD_03          17U
#define GPIO_AD_04          18U
#define GPIO_AD_05          19U
#define GPIO_AD_06          20U
#define GPIO_AD_07          21U
#define GPIO_AD_08          22U
#define GPIO_AD_09          23U
#define GPIO_AD_10          24U
#define GPIO_AD_11          25U
#define GPIO_AD_12          26U
#define GPIO_AD_13          27U
#define GPIO_AD_14          28U

// GPIO_SD
#define GPIO_SD_00          29U 
#define GPIO_SD_01          30U 
#define GPIO_SD_02          31U 
#define GPIO_SD_03          32U 
#define GPIO_SD_04          33U 
#define GPIO_SD_05          34U 
#define GPIO_SD_06          35U 
#define GPIO_SD_07          36U 
#define GPIO_SD_08          37U 
#define GPIO_SD_09          38U 
#define GPIO_SD_10          39U 
#define GPIO_SD_11          40U 
#define GPIO_SD_12          41U 
#define GPIO_SD_13          42U 

#define PIN_SW_ACCENT        GPIO_00
#define PIN_SW_CRUSH         GPIO_02
#define PIN_HP_JACK_DETECT   GPIO_01
#define PIN_TOUCH_CLK        GPIO_03
#define PIN_TOUCH_DATA       GPIO_04
#define PIN_SYNC_JACK_DETECT GPIO_05
#define PIN_LED_1            GPIO_08
#define PIN_LED_2            GPIO_07
#define PIN_LED_3            GPIO_06
#define PIN_MIDI_RX          GPIO_09
#define PIN_MIDI_TX          GPIO_10
#define PIN_ROW_1            GPIO_13
#define PIN_ROW_2            GPIO_12
#define PIN_ROW_3            GPIO_SD_04
#define PIN_ROW_4            GPIO_11

#define PIN_SYNC_IN          GPIO_AD_00
#define PIN_SYNC_OUT         GPIO_AD_01
#define PIN_MQS_R            GPIO_AD_02
#define PIN_POT_1            GPIO_AD_03
#define PIN_POT_2            GPIO_AD_04
#define PIN_COL_6            GPIO_AD_05
#define PIN_COL_4            GPIO_AD_06
#define PIN_COL_3            GPIO_AD_07
#define PIN_COL_2            GPIO_AD_08
#define PIN_COL_1            GPIO_AD_09
#define PIN_AMP_MUTE         GPIO_AD_10
#define PIN_HP_ENABLE        GPIO_AD_11
#define PIN_SWD_CLK          GPIO_AD_12
#define PIN_SWD_SWDIO        GPIO_AD_13
#define PIN_SYN_MUX_IO       GPIO_AD_14

#define PIN_SYN_ADDR0        GPIO_SD_00
#define PIN_SYN_ADDR1        GPIO_SD_01
#define PIN_SYN_ADDR2        GPIO_SD_02
#define PIN_COL_5            GPIO_SD_13


#define UART_INTERFACES_COUNT 1

#define PIN_UART1_RX      GPIO_09 
#define PIN_UART1_TX      GPIO_10
#define UART1_NUM         LPUART1
#define UART1_IRQn        LPUART1_IRQn
#define UART1_HANDLER     LPUART1_IRQHandler

#define I2C1    LPI2C1
#define I2C1_SCL   
#define I2C1_SDA   

#define SCL I2C1_SCL
#define SDA I2C1_SDA

#define I2C2   
#define I2C2_SCL 
#define I2C2_SDA 

// default SPI PORT
#define SPI1_MOSI
#define SPI1_MISO
#define SPI1_SCK 

#define MOSI SPI1_MOSI
#define MISO SPI1_MISO
#define SCK  SPI1_SCK

#ifdef __cplusplus
}
#endif


#endif /*end of  PINS_ARDUINO_DUOBRAINS2 */
