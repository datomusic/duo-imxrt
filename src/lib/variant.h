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

#define USB_VID 0x16D0 // MCS Electronics
#define USB_PID 0x10A7 // DUO Brains 2 VID

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// GPIO_0
#define PIN_SW1              0U
#define PIN_HP_JACK_DETECT   1U
#define PIN_SW2              2U
#define PIN_TOUCH_CLK        3U
#define PIN_TOUCH_DATA       4U
#define PIN_SYNC_JACK_DETECT 5U
#define PIN_LED_1            6U
#define PIN_LED_2            7U
#define PIN_LED_3            8U
#define PIN_MIDI_RX          9U
#define PIN_MIDI_TX         10U
#define PIN_ROW_4           11U
#define PIN_ROW_2           12U
#define PIN_ROW_1           13U

// GPIO_AD
#define PIN_SYNC_IN         14U
#define PIN_SYNC_OUT        15U
#define PIN_MQS_R           16U
#define PIN_POT_1           17U
#define PIN_POT_2           18U
#define PIN_COL_6           19U
#define PIN_COL_4           20U
#define PIN_COL_3           21U
#define PIN_COL_2           22U
#define PIN_COL_1           23U
#define PIN_AMP_MUTE        24U
#define PIN_HP_ENABLE       25U
#define PIN_SWD_CLK         26U
#define PIN_SWD_SWDIO       27U
#define PIN_SYN_MUX_IO      28U

// GPIO_SD
#define PIN_SYN_ADDR0       29U
#define PIN_SYN_ADDR1       30U
#define PIN_SYN_ADDR2       31U
#define PIN_SD_03           32U
#define PIN_ROW_3           33U

#define PIN_COL_5           34U


#define UART_INTERFACES_COUNT 1

#define PIN_UART1_RX      D0 // GPIO_09
#define PIN_UART1_TX      D1 // GPIO_10
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
