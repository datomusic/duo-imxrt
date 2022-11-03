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

static sai_transfer_t xfer;
static edma_config_t dmaConfig = {0};
static sai_transceiver_t config;
static edma_handle_t g_dmaHandle = {0};
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle) = {0};

static bool buffer_toggle = true;

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
    .loopDivider = 28, /* PLL loop divider. Valid range for DIV_SELECT divider
                          value: 27~54. */
    .postDivider =
        4, /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator = 2240,    /* 30 bit numerator of fractional loop divider. */
    .denominator = 10000, /* 30 bit denominator of fractional loop divider */
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
      IOMUXC_GPR, kIOMUXC_MqsPwmOverSampleRate32,
      0u); /* 98.304MHz/64/(0+1) = 1.536MHz
           Higher frequency PWM involves less low frequency harmonic.*/
}

static uint32_t I2S3_tx_buffer[AUDIO_BLOCK_SAMPLES];

void AudioOutputMQS::begin(void) {
  CLOCK_InitAudioPll(&audioPllConfig);

  // Clock setting for SAI.
  CLOCK_SetMux(kCLOCK_Sai3Mux, DEMO_SAI_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Sai3PreDiv, DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER);
  CLOCK_SetDiv(kCLOCK_Sai3Div, DEMO_SAI_CLOCK_SOURCE_DIVIDER);

  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_02_MQS_RIGHT, 0);

  DMAMUX_Init(DMAMUX);
  DMAMUX_SetSource(DMAMUX, DEMO_EDMA_CHANNEL, DEMO_SAI_TX_SOURCE);
  DMAMUX_EnableChannel(DMAMUX, DEMO_EDMA_CHANNEL);

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

  configMQS();

  update_responsibility = update_setup();
  xfer.data = (uint8_t *)(uint32_t)I2S3_tx_buffer;
  xfer.dataSize = sizeof(I2S3_tx_buffer) / 2;
  SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer);
}

void AudioOutputMQS::isr(I2S_Type *base, sai_edma_handle_t *handle,
                         status_t status, void *userData) {
  int16_t *dest;
  audio_block_t *blockR;

  uint32_t offsetR;

  if (buffer_toggle) {
    // DMA is transmitting the first half of the buffer
    // so we must fill the second half
    dest = (int16_t *)&I2S3_tx_buffer[AUDIO_BLOCK_SAMPLES / 2];
	if (AudioOutputMQS::update_responsibility) AudioStream::update_all();
  } else {
    // DMA is transmitting the second half of the buffer
    // so we must fill the first half
    dest = (int16_t *)I2S3_tx_buffer;
  }

  buffer_toggle = !buffer_toggle;

  blockR = AudioOutputMQS::block_right_1st;
  offsetR = AudioOutputMQS::block_right_offset;

  if (blockR) {
    memcpy_tointerleaveR(dest, blockR->data + offsetR);
    offsetR += AUDIO_BLOCK_SAMPLES / 2;
  } else {
    memset(dest, 0, sizeof(I2S3_tx_buffer) / 2);
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
  xfer.dataSize = sizeof(I2S3_tx_buffer) / 2;
  SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer);
}

void AudioOutputMQS::update(void) {
  audio_block_t *block;

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

