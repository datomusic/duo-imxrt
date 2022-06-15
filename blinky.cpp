#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#define EXAMPLE_LED_GPIO     BOARD_USER_LED_GPIO
#define EXAMPLE_LED_GPIO_PIN BOARD_USER_LED_GPIO_PIN

volatile uint32_t g_systickCounter;
/* The PIN status */
volatile bool g_pinSet = false;


#ifdef __cplusplus
extern "C" {
#endif
void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}
#ifdef __cplusplus
}
#endif

void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while (g_systickCounter != 0U)
    {
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Board pin init */
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitNeopixels();

    /* Set systick reload value to generate 1ms interrupt */
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        while (1)
        {
        }
    }
    
    uint8_t teal[] = {0x00,0xf0,0xc0};
    uint8_t yellow[] = {0xff,0xff,0x00};
    uint8_t blue[] = {0x00,0x00,0xff};
    uint8_t black[] = {0x00,0x00,0x00};
    board_rgb_write(blue);

    while (1)
    {
        /* Delay 1000 ms */

        SysTick_DelayTicks(500U);
        if (g_pinSet)
        {
            GPIO_PinWrite(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, 0U);
            board_rgb_write(black);
            g_pinSet = false;
        }
        else
        {
            GPIO_PinWrite(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, 1U);
            board_rgb_write(blue);
            g_pinSet = true;
        }
    }
}
