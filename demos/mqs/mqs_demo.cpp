#include "board.h"
#include "pin_mux.h"
#include "music.h"
#include "clock_config.h"

#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "fsl_iomuxc.h"
#include "fsl_debug_console.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI instance and clock */
#define DEMO_SAI SAI3
#define DEMO_SAI_CHANNEL (0)
#define DEMO_AUDIO_WORD_WIDTH kSAI_WordWidth16bits

/* Select Audio PLL (786.432 MHz) as sai1 clock source */
#define DEMO_SAI_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai clock source */
#define DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER (3U)
/* Clock divider for sai clock source */
#define DEMO_SAI_CLOCK_SOURCE_DIVIDER (1U)
/* Get frequency of sai clock: SAI3_Clock = 786.432MHz /(3+1)/(1+1) = 98.304MHz */
#define DEMO_SAI_CLK_FREQ (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI_CLOCK_SOURCE_DIVIDER + 1U) / (DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER + 1U))

/* DMA */
#define DEMO_DMA           DMA0
#define DEMO_EDMA_CHANNEL  (0U)
#define DEMO_SAI_TX_SOURCE kDmaRequestMuxSai3Tx
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM)
 *                              = 24 * (32 + 768/1000)
 *                              = 786.432 MHz
 */
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator = 768,   /* 30 bit numerator of fractional loop divider. */
    .denominator = 1000,/* 30 bit denominator of fractional loop divider */
};
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle) = {0};
edma_handle_t g_dmaHandle                                = {0};

volatile bool isFinished      = false;

static void callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    isFinished = true;
}

void configMQS(void)
{
    CCM->CCGR0 = (CCM->CCGR0 & (~CCM_CCGR0_CG2_MASK)) | CCM_CCGR0_CG2(3);         /* Enable MQS hmclk. */

    IOMUXC_MQSEnterSoftwareReset(IOMUXC_GPR, true);                             /* Reset MQS. */
    IOMUXC_MQSEnterSoftwareReset(IOMUXC_GPR, false);                            /* Release reset MQS. */
    IOMUXC_MQSEnable(IOMUXC_GPR, true);                                         /* Enable MQS. */
    IOMUXC_MQSConfig(IOMUXC_GPR, kIOMUXC_MqsPwmOverSampleRate64, 0u);           /* 98.304MHz/64/(0+1) = 1.536MHz
                                                                                Higher frequency PWM involves less low frequency harmonic.*/
}

void headphone_enable(void) {
    #define HP_ENABLE_PINMUX IOMUXC_GPIO_AD_11_GPIOMUX_IO25
    #define HP_ENABLE_PORT GPIO1
    #define HP_ENABLE_PIN 25U

    IOMUXC_SetPinMux(HP_ENABLE_PINMUX, 0U);
    gpio_pin_config_t hp_enable_config = { kGPIO_DigitalOutput, 0};
    GPIO_PinInit(HP_ENABLE_PORT , HP_ENABLE_PIN, &hp_enable_config); 
    GPIO_PinWrite(HP_ENABLE_PORT, HP_ENABLE_PIN, 1);
}

void amp_enable(void) {
    #define AMP_MUTE_PINMUX IOMUXC_GPIO_AD_10_GPIOMUX_IO24
    #define AMP_MUTE_PORT GPIO1
    #define AMP_MUTE_PIN 24U

    IOMUXC_SetPinMux(AMP_MUTE_PINMUX, 0U);
    gpio_pin_config_t amp_mute_config = { kGPIO_DigitalOutput, 0};
    GPIO_PinInit(AMP_MUTE_PORT , AMP_MUTE_PIN, &amp_mute_config); 
    GPIO_PinWrite(AMP_MUTE_PORT, AMP_MUTE_PIN, 0);
}

void leds_enable(void) {
   #define LED1_PINMUX IOMUXC_GPIO_08_GPIOMUX_IO08
   #define LED1_PORT GPIO1
   #define LED1_PIN 8U

   IOMUXC_SetPinMux(LED1_PINMUX, 0U);
   gpio_pin_config_t led1_config = { kGPIO_DigitalOutput, 0};
   GPIO_PinInit(LED1_PORT , LED1_PIN, &led1_config); 
   GPIO_PinWrite(LED1_PORT, LED1_PIN, 0); 
}

/*!
 * @brief Main function
 */
int main(void)
{
    sai_transfer_t xfer;
    edma_config_t dmaConfig = {0};
    sai_transceiver_t config;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    CLOCK_InitAudioPll(&audioPllConfig);
    BOARD_InitDebugConsole();
    headphone_enable();
    amp_enable();
    leds_enable();

    /*Clock setting for SAI. */
    CLOCK_SetMux(kCLOCK_Sai3Mux, DEMO_SAI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai3PreDiv, DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai3Div, DEMO_SAI_CLOCK_SOURCE_DIVIDER);

    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_02_MQS_RIGHT, 0);

    DMAMUX_Init(DMAMUX);
    DMAMUX_SetSource(DMAMUX, DEMO_EDMA_CHANNEL, DEMO_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(DMAMUX, DEMO_EDMA_CHANNEL);

    PRINTF("SAI MQS DMA example started.\n\r");

    /* Create EDMA handle */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DEMO_DMA, &dmaConfig);
    EDMA_CreateHandle(&g_dmaHandle, DEMO_DMA, DEMO_EDMA_CHANNEL);

    /* SAI init */
    SAI_Init(DEMO_SAI);

    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, callback, NULL, &g_dmaHandle);

    /* I2S mode configurations */
    /* MQS expects
       - 2-channel, LSB-valid 16-bit, MSB shift-out first serial data (sdata)
       - Frame sync asserting with the first bit of the frame (fs)
       - Bit clock used to shift data out on the positive clock edge (bclk)
    */
    SAI_GetLeftJustifiedConfig(&config, DEMO_AUDIO_WORD_WIDTH, kSAI_Stereo, 1U << DEMO_SAI_CHANNEL);
    config.frameSync.frameSyncEarly = false;
    config.frameSync.frameSyncPolarity = kSAI_PolarityActiveHigh;
    SAI_TransferTxSetConfigEDMA(DEMO_SAI, &txHandle, &config);
    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_SAI_CLK_FREQ, kSAI_SampleRate48KHz, DEMO_AUDIO_WORD_WIDTH,2);

    configMQS();
    SDK_DelayAtLeastUs(1000, SystemCoreClock);

    /*  xfer structure */
    xfer.data = (uint8_t *)(int32_t)music;
    xfer.dataSize = MUSIC_LEN;

    while (1)
    {
        isFinished = false;
        SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer);
        /* Wait until finished */
        while (isFinished != true)
        {
        }
    }

}