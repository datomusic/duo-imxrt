/* Audio Library for Teensy 3.X
 * Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
// Frank B

#include "./teensy_audio_stubs.h"

#if defined(__IMXRT1052__) || defined(__IMXRT1062__)
#include "memcpy_audio.h"
#include "output_mqs.h"
#include <Arduino.h>

//#include "utility/imxrt_hw.h"

audio_block_t *AudioOutputMQS::block_left_1st = NULL;
audio_block_t *AudioOutputMQS::block_right_1st = NULL;
audio_block_t *AudioOutputMQS::block_left_2nd = NULL;
audio_block_t *AudioOutputMQS::block_right_2nd = NULL;
uint16_t AudioOutputMQS::block_left_offset = 0;
uint16_t AudioOutputMQS::block_right_offset = 0;

bool AudioOutputMQS::update_responsibility = false;
DMAMEM __attribute__((aligned(32)))

static void
headphone_enable(void) {
#define HP_ENABLE_PINMUX IOMUXC_GPIO_AD_11_GPIOMUX_IO25
#define HP_ENABLE_PORT GPIO1
#define HP_ENABLE_PIN 25U

  IOMUXC_SetPinMux(HP_ENABLE_PINMUX, 0U);
  gpio_pin_config_t hp_enable_config = {kGPIO_DigitalOutput, 0};
  GPIO_PinInit(HP_ENABLE_PORT, HP_ENABLE_PIN, &hp_enable_config);
  GPIO_PinWrite(HP_ENABLE_PORT, HP_ENABLE_PIN, 1);
}

static void amp_enable(void) {
#define AMP_MUTE_PINMUX IOMUXC_GPIO_AD_10_GPIOMUX_IO24
#define AMP_MUTE_PORT GPIO1
#define AMP_MUTE_PIN 24U

  IOMUXC_SetPinMux(AMP_MUTE_PINMUX, 0U);
  gpio_pin_config_t amp_mute_config = {kGPIO_DigitalOutput, 0};
  GPIO_PinInit(AMP_MUTE_PORT, AMP_MUTE_PIN, &amp_mute_config);
  GPIO_PinWrite(AMP_MUTE_PORT, AMP_MUTE_PIN, 0);
}
static sai_transfer_t xfer;
static edma_config_t dmaConfig = {0};
static sai_transceiver_t config;
static edma_handle_t g_dmaHandle = {0};
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle) = {0};

#define DEMO_SAI SAI3

/* Select Audio PLL (786.432 MHz) as sai1 clock source */
#define DEMO_SAI_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai clock source */
#define DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER (4U)
/* Clock divider for sai clock source */
#define DEMO_SAI_CLOCK_SOURCE_DIVIDER (1U)
/* Get frequency of sai clock: SAI3_Clock = 786.432MHz /(3+1)/(1+1) = 98.304MHz
 */
#define DEMO_SAI_CLK_FREQ                                                      \
  (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI_CLOCK_SOURCE_DIVIDER + 1U) /  \
   (DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER + 1U))

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM)
 *                              = 24 * (32 + 768/1000)
 *                              = 786.432 MHz
 */
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 28,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 4,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator = 2240,   /* 30 bit numerator of fractional loop divider. */
    .denominator = 10000,/* 30 bit denominator of fractional loop divider */
};
/* DMA */
#define DEMO_DMA DMA0
#define DEMO_EDMA_CHANNEL (0U)
#define DEMO_SAI_TX_SOURCE kDmaRequestMuxSai3Tx

static void configMQS(void) {
  CLOCK_EnableClock(kCLOCK_Mqs);
  IOMUXC_MQSEnterSoftwareReset(IOMUXC_GPR, true);  /* Reset MQS. */
  IOMUXC_MQSEnterSoftwareReset(IOMUXC_GPR, false); /* Release reset MQS. */
  IOMUXC_MQSEnable(IOMUXC_GPR, true);              /* Enable MQS. */
  IOMUXC_MQSConfig(
      IOMUXC_GPR, kIOMUXC_MqsPwmOverSampleRate64,
      0u); /* 98.304MHz/64/(0+1) = 1.536MHz
           Higher frequency PWM involves less low frequency harmonic.*/
}

static uint32_t I2S3_tx_buffer[AUDIO_BLOCK_SAMPLES];

void AudioOutputMQS::begin(void) {
CLOCK_InitAudioPll(&audioPllConfig);
  headphone_enable();
  amp_enable();

  // Clock setting for SAI.
  CLOCK_SetMux(kCLOCK_Sai3Mux, DEMO_SAI_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Sai3PreDiv, DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER);
  CLOCK_SetDiv(kCLOCK_Sai3Div, DEMO_SAI_CLOCK_SOURCE_DIVIDER);

  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_02_MQS_RIGHT, 0);

  DMAMUX_Init(DMAMUX);
  DMAMUX_SetSource(DMAMUX, DEMO_EDMA_CHANNEL, DEMO_SAI_TX_SOURCE);
  DMAMUX_EnableChannel(DMAMUX, DEMO_EDMA_CHANNEL);

  // PRINTF("SAI MQS DMA example started.\n\r");

  // Create EDMA handle
  EDMA_GetDefaultConfig(&dmaConfig);
  EDMA_Init(DEMO_DMA, &dmaConfig);
  EDMA_CreateHandle(&g_dmaHandle, DEMO_DMA, DEMO_EDMA_CHANNEL);

  // SAI init
  SAI_Init(DEMO_SAI);

  SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, isr, NULL, &g_dmaHandle);

  // I2S mode configurations
  SAI_GetClassicI2SConfig(&config, kSAI_WordWidth16bits, kSAI_Stereo, 1U << 0u);
  config.frameSync.frameSyncEarly = false;
  config.frameSync.frameSyncPolarity = kSAI_PolarityActiveHigh;
  SAI_TransferTxSetConfigEDMA(DEMO_SAI, &txHandle, &config);

  // set bit clock divider
  SAI_TxSetBitClockRate(DEMO_SAI, DEMO_SAI_CLK_FREQ, kSAI_SampleRate44100Hz,
                        kSAI_WordWidth16bits, 2u);

  update_responsibility = AudioStream::update_setup();

  configMQS();
  xfer.data = (uint8_t *)(uint32_t)I2S3_tx_buffer;
  xfer.dataSize = AUDIO_BLOCK_SAMPLES / 2;
  SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer);
}

/*

DMAChannel AudioOutputMQS::dma(false);

void AudioOutputMQS::begin(void)
{
        dma.begin(true); // Allocate the DMA channel first
        block_left_1st = NULL;
        block_right_1st = NULL;

        config_i2s();

        CORE_PIN10_CONFIG = 2;//B0_00 MQS_RIGHT
        CORE_PIN12_CONFIG = 2;//B0_01 MQS_LEFT

        //DMA0->TCD->SADDR = I2S3_tx_buffer;
        DMA0->TCD->SOFF = 2;
        //DMA0->TCD->ATTR = DMA_TCD_ATTR_SSIZE(1) | DMA_TCD_ATTR_DSIZE(1);
        DMA0->TCD->NBYTES_MLNO = 2;
        DMA0->TCD->SLAST = -sizeof(I2S3_tx_buffer);
        DMA0->TCD->DOFF = 0;
        DMA0->TCD->CITER_ELINKNO = sizeof(I2S3_tx_buffer) / 2;
        DMA0->TCD->DLAST_SGA = 0;
        DMA0->TCD->BITER_ELINKNO = sizeof(I2S3_tx_buffer) / 2;
        //DMA0->TCD->CSR = DMA_TCD_CSR_INTHALF | DMA_TCD_CSR_INTMAJOR;
        //DMA0->TCD->DADDR = (void *)((uint32_t)&I2S3_TDR0 + 0);
        //dma.triggerAtHardwareEvent(DMAMUX_SOURCE_SAI3_TX);

        //I2S3_TCSR |= I2S_TCSR_TE | I2S_TCSR_BCE | I2S_TCSR_FRDE;
        //update_responsibility = update_setup();

        dma.attachInterrupt(isr);
        dma.enable();
}
        */

void AudioOutputMQS::isr(I2S_Type *base, sai_edma_handle_t *handle,
                         status_t status, void *userData) {
  int16_t *dest;
  audio_block_t *blockL, *blockR;

  uint32_t saddr, offsetL, offsetR;
  saddr = (uint32_t)(DMA0->TCD->SADDR);

  // dma.clearInterrupt();

  if (saddr < (uint32_t)I2S3_tx_buffer + sizeof(I2S3_tx_buffer) / 2) {
    // DMA is transmitting the first half of the buffer
    // so we must fill the second half
    dest = (int16_t *)&I2S3_tx_buffer[AUDIO_BLOCK_SAMPLES / 2];
    if (AudioOutputMQS::update_responsibility)
      AudioStream::update_all();
  } else {
    // DMA is transmitting the second half of the buffer
    // so we must fill the first half
    dest = (int16_t *)I2S3_tx_buffer;
  }

  blockL = AudioOutputMQS::block_left_1st;
  blockR = AudioOutputMQS::block_right_1st;
  offsetL = AudioOutputMQS::block_left_offset;
  offsetR = AudioOutputMQS::block_right_offset;

  if (blockL && blockR) {
    memcpy_tointerleaveLR(dest, blockL->data + offsetL, blockR->data + offsetR);
    offsetL += AUDIO_BLOCK_SAMPLES / 2;
    offsetR += AUDIO_BLOCK_SAMPLES / 2;
  } else if (blockL) {
    memcpy_tointerleaveL(dest, blockL->data + offsetL);
    offsetL += AUDIO_BLOCK_SAMPLES / 2;
  } else if (blockR) {
    memcpy_tointerleaveR(dest, blockR->data + offsetR);
    offsetR += AUDIO_BLOCK_SAMPLES / 2;
  } else {
    memset(dest, 0, sizeof(I2S3_tx_buffer) / 2);
  }

#if IMXRT_CACHE_ENABLED >= 2
  arm_dcache_flush_delete(dest, sizeof(I2S3_tx_buffer) / 2);
#endif

  if (offsetL < AUDIO_BLOCK_SAMPLES) {
    AudioOutputMQS::block_left_offset = offsetL;
  } else {
    AudioOutputMQS::block_left_offset = 0;
    AudioStream::release(blockL);
    AudioOutputMQS::block_left_1st = AudioOutputMQS::block_left_2nd;
    AudioOutputMQS::block_left_2nd = NULL;
  }
  if (offsetR < AUDIO_BLOCK_SAMPLES) {
    AudioOutputMQS::block_right_offset = offsetR;
  } else {
    AudioOutputMQS::block_right_offset = 0;
    AudioStream::release(blockR);
    AudioOutputMQS::block_right_1st = AudioOutputMQS::block_right_2nd;
    AudioOutputMQS::block_right_2nd = NULL;
  }

  xfer.data = (uint8_t *)dest;
  xfer.dataSize = AUDIO_BLOCK_SAMPLES / 2;
  SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer);
}

void AudioOutputMQS::update(void) {
  // null audio device: discard all incoming data
  // if (!active) return;
  // audio_block_t *block = receiveReadOnly();
  // if (block) release(block);
  // digitalWriteFast(13, LOW);
  audio_block_t *block;
  block = receiveReadOnly(0); // input 0 = left channel
  if (block) {
    __disable_irq();
    if (block_left_1st == NULL) {
      block_left_1st = block;
      block_left_offset = 0;
      __enable_irq();
    } else if (block_left_2nd == NULL) {
      block_left_2nd = block;
      __enable_irq();
    } else {
      audio_block_t *tmp = block_left_1st;
      block_left_1st = block_left_2nd;
      block_left_2nd = block;
      block_left_offset = 0;
      __enable_irq();
      release(tmp);
    }
  }
  block = receiveReadOnly(1); // input 1 = right channel
  if (block) {
    __disable_irq();
    if (block_right_1st == NULL) {
      block_right_1st = block;
      block_right_offset = 0;
      __enable_irq();
    } else if (block_right_2nd == NULL) {
      block_right_2nd = block;
      __enable_irq();
    } else {
      audio_block_t *tmp = block_right_1st;
      block_right_1st = block_right_2nd;
      block_right_2nd = block;
      block_right_offset = 0;
      __enable_irq();
      release(tmp);
    }
  }
}

#endif // defined(__IMXRT1062__)
