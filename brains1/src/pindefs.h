#ifndef PINDEFS_H
#define PINDEFS_HPINDEFS_H


#define SEQ_0_4
#define POT_1       A10
#define POT_2       A11
#define MUX_IO       29
#define UNCONNECTED_ANALOG 13 //ADC0_DM3/A13 is unconnected
// DAC_OUT
#define SYN_ADDR0    33
#define SYN_ADDR1    24
#define SYN_ADDR2     3
#define HP_ENABLE     4
#define TOUCH_1      16
#define TOUCH_2      17
#define TOUCH_3      19
#define TOUCH_4      18
#define RX1           0
#define RX2           1
#define LED_DATA     32
#define LED_CLK      25
#define ROW_1        15
#define ROW_2        22
#define ROW_3        23
#define ROW_4         9
#define COL_1        10
#define COL_2        13
#define COL_3        11
#define COL_4        12
#define COL_5        28
#define COL_6        27
#define JACK_DETECT  26
#define SYNC_DETECT   2
#define SYNC_IN      14
#define SYNC_OUT      7
#define AMP_ENABLE    6
#define LED_1        20
#define LED_2        21
#define LED_3         5
#define SW1           8
#define SW2          30
#define PROG         31

const int BTN_SYN1 = SW1;
const int BTN_SYN2 = SW2;

const int TOUCH1 = TOUCH_1;
const int TOUCH2 = TOUCH_2;
const int TOUCH3 = TOUCH_3;
const int TOUCH4 = TOUCH_4;

const int ACCENT_PIN = BTN_SYN1;
const int BITC_PIN = BTN_SYN2;

const int OSC_LED = LED_1;
const int FILTER_LED = LED_2;
const int ENV_LED = LED_3;

const int SYNC_OUT_PIN = SYNC_OUT;



// One more LED than the physical number of leds for loopback testing
const int NUM_LEDS = 19+1;
const int led_order[NUM_LEDS] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

// Multiplexer channels.
#define POT_SYN1 0
#define POT_SYN2 1

#define SW_SYN3  2
#define POT_SYN3 3

#define SW_SYN4  4 

#define POT_SYN4 5 
#define POT_SYN5 6 
#define POT_SYN6 7 

const int SLIDE_PIN = 1;
const int DELAY_PIN = 2;
const int AMP_ENV_POT = 4;
const int FILTER_RES_POT = 0;

const int FILTER_FREQ_POT = 5;

const int OSC_DETUNE_POT = 3;  

const int OSC_PW_POT = 7;

const int AMP_POT = 6; 

const int TEMPO_POT = 1;
const int GATE_POT = 2;

#endif /* end of include guard: PINDEFS_HPINDEFS_H */
