
#include "board.h"
#include "lib/pin_mux.h"
#include "music-silence.h"
#include "lib/clock_config.h"

#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "fsl_iomuxc.h"
#include "fsl_debug_console.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI instance and clock */
#define DEMO_SAI SAI1

/* Select Audio PLL (786.432 MHz) as sai1 clock source */
#define DEMO_SAI_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai clock source */
#define DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER (4U)
/* Clock divider for sai clock source */
#define DEMO_SAI_CLOCK_SOURCE_DIVIDER (1U)
/* Get frequency of sai clock: SAI3_Clock = 786.432MHz /(3+1)/(1+1) = 98.304MHz */
#define DEMO_SAI_CLK_FREQ (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI_CLOCK_SOURCE_DIVIDER + 1U) / (DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER + 1U))

/* DMA */
#define DEMO_DMA           DMA0
#define DEMO_EDMA_CHANNEL  (0U)
#define DEMO_SAI_TX_SOURCE kDmaRequestMuxSai1Tx
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

#define I2S1_TMR SAI1->TMR
#define I2S1_TCSR SAI1->TCSR
#define I2S1_TCR1 SAI1->TCR1
#define I2S1_TCR2 SAI1->TCR2
#define I2S1_TCR3 SAI1->TCR3
#define I2S1_TCR4 SAI1->TCR4
#define I2S1_TCR5 SAI1->TCR5
#define I2S1_RMR SAI1->RMR
#define I2S1_RCR1 SAI1->RCR1
#define I2S1_RCR2 SAI1->RCR2
#define I2S1_RCR3 SAI1->RCR3
#define I2S1_RCR4 SAI1->RCR4
#define I2S1_RCR5 SAI1->RCR5

void config_pt8211(){
    int rsync = 0;
	int tsync = 1;
	#if defined(AUDIO_PT8211_OVERSAMPLING)
	int div = 0;
	#else
	int div = 3;
	#endif
	// configure transmitter
	I2S1_TMR = 0;
	I2S1_TCR1 = I2S_TCR1_TFW(0);
	I2S1_TCR2 = I2S_TCR2_SYNC(tsync) | I2S_TCR2_BCP(1) | I2S_TCR2_MSEL(1) | I2S_TCR2_BCD(1) | I2S_TCR2_DIV(div);
	I2S1_TCR3 = I2S_TCR3_TCE(1);
//	I2S1_TCR4 = I2S_TCR4_FRSZ(1) | I2S_TCR4_SYWD(15) | I2S_TCR4_MF | I2S_TCR4_FSE | I2S_TCR4_FSP | I2S_TCR4_FSD; //TDA1543
	I2S1_TCR4 = I2S_TCR4_FRSZ(1) | I2S_TCR4_SYWD(15) | I2S_TCR4_MF(1) /*| I2S_TCR4_FSE*/ | I2S_TCR4_FSP(1) | I2S_TCR4_FSD(1); //PT8211
	I2S1_TCR5 = I2S_TCR5_WNW(15) | I2S_TCR5_W0W(15) | I2S_TCR5_FBT(15);

	I2S1_RMR = 0;
	//I2S1_RCSR = (1<<25); //Reset
	I2S1_RCR1 = I2S_RCR1_RFW(0);
	I2S1_RCR2 = I2S_RCR2_SYNC(rsync) | I2S_RCR2_BCP(1) | I2S_RCR2_MSEL(1) | I2S_TCR2_BCD(1) | I2S_TCR2_DIV(div);
	I2S1_RCR3 = I2S_RCR3_RCE(1);
//	I2S1_TCR4 = I2S_TCR4_FRSZ(1) | I2S_TCR4_SYWD(15) | I2S_TCR4_MF | I2S_TCR4_FSE | I2S_TCR4_FSP | I2S_TCR4_FSD; //TDA1543
	I2S1_RCR4 = I2S_RCR4_FRSZ(1) | I2S_RCR4_SYWD(15) | I2S_RCR4_MF(1) /*| I2S_RCR4_FSE*/ | I2S_RCR4_FSP(1) | I2S_RCR4_FSD(1); //PT8211
	I2S1_RCR5 = I2S_RCR5_WNW(15) | I2S_RCR5_W0W(15) | I2S_RCR5_FBT(15);
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

    /*Clock setting for SAI. */
    CLOCK_SetMux(kCLOCK_Sai3Mux, DEMO_SAI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai3PreDiv, DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai3Div, DEMO_SAI_CLOCK_SOURCE_DIVIDER);

	IOMUXC_SetPinMux(IOMUXC_GPIO_04_SAI1_TX_DATA00, 0);
	IOMUXC_SetPinMux(IOMUXC_GPIO_06_SAI1_TX_BCLK, 0);
	IOMUXC_SetPinMux(IOMUXC_GPIO_07_SAI1_TX_SYNC , 0);

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
    SAI_GetClassicI2SConfig(&config, kSAI_WordWidth16bits, kSAI_Stereo, 1U << 0u);
    config.frameSync.frameSyncEarly = false;
    config.frameSync.frameSyncPolarity = kSAI_PolarityActiveHigh;
    SAI_TransferTxSetConfigEDMA(DEMO_SAI, &txHandle, &config);
    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_SAI_CLK_FREQ, kSAI_SampleRate44100Hz, kSAI_WordWidth16bits, 2u);

    config_pt8211();

    /*  xfer structure */
    xfer.data = (uint8_t *)(uint32_t)music;
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
