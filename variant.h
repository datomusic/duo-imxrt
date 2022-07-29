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
#ifndef PINS_ARDUINO_IMXRT1010_EVK
#define PINS_ARDUINO_IMXRT1010_EVK

#include "WVariant.h"
#include "fsl_lpuart.h"
#include "fsl_common.h"
#include "fsl_gpio.h"

// Used by FastLED for various timings.
#define F_CPU DEFAULT_SYSTEM_CLOCK

// Disable FastLED reliance on digitalPinToBitMask and friends.
// These are only used in the FastLed Pin class, which is legacy code anyway,
// replaced by FastPin.
// Source: https://github.com/FastLED/FastLED/issues/766
#define FASTLED_NO_PINMAP


#define USB_VID 0x2886 // SeeedStudio
#define USB_PID 0x802B // TODO: replace

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define D1   1U
#define D2   2U
#define D3   3U
#define D4   4U
#define D5   5U
#define D6   6U
#define D7   7U
#define D8   8U
#define D9   9U
#define D10 10U
#define D11 11U
#define D12 12U

#define A0 16U
#define A1 17U
#define A2 18U
#define A3 19U
#define A4 20U
#define A5 21U

#define USER_LED 22U

#define LED_BUILTIN USER_LED

#define UART_INTERFACES_COUNT 3

#define PIN_UART1_RX      D0 // GPIO_09
#define PIN_UART1_TX      D1 // GPIO_10
#define UART1_NUM         LPUART1
#define UART1_IRQn        LPUART1_IRQn
#define UART1_HANDLER     LPUART1_IRQHandler

#define PIN_UART2_RX      J4_11 // GPIO_13
#define PIN_UART2_TX      J4_10 // GPIO_AD_00
#define UART2_NUM         LPUART2
#define UART2_IRQn        LPUART2_IRQn
#define UART2_HANDLER     LPUART2_IRQHandler

#define PIN_UART3_RX      USER_LED // GPIO_11
#define PIN_UART3_TX      J4_16 // GPIO_12
#define UART3_NUM         LPUART3
#define UART3_IRQn        LPUART3_IRQn
#define UART3_HANDLER     LPUART3_IRQHandler

#define I2C1    LPI2C1
#define I2C1_SCL   
#define I2C1_SDA   

// default I2C PORT
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


#endif /*end of  PINS_ARDUINO_IMXRT1010_EVK*/
