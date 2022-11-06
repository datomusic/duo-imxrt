#pragma once
/*
 * pinmap.h maps the physical IO pins to software functions
 * the hardware pin mappings can be found in variant.h
 */
#define SEQ_1_2
#define POT_1       PIN_POT_1
#define POT_2       PIN_POT_2
#define MUX_IO      PIN_SYN_MUX_IO
// DAC_OUT
#define TOUCH1      16
#define TOUCH2      17
#define TOUCH3      19
#define TOUCH4      18
#define LED_DATA     32
#define LED_CLK      25
#define JACK_DETECT  26
#define SYNC_DETECT   2
#define SYNC_IN      14
#define LED_1        20
#define LED_2        21
#define LED_3         5
#define SW1           8
#define SW2          30

const int ACCENT_PIN = SW1;
const int BITC_PIN = SW2;

// One more LED than the physical number of leds for loopback testing
const int NUM_LEDS = 19+1;
const int led_order[NUM_LEDS] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

// Multiplexer channels.

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

int muxAnalogRead(uint8_t channel) { 
  // Any call to pinMode sets the port mux to GPIO mode.
  // We want to force it back to analog mode
  // Might be equivalent to pinMode(MUX_IO, INPUT_DISABLE);
  //  volatile uint32_t *config;
  //  config = portConfigRegister(PIN_SYN_MUX_IO);
  //  *config = PORT_PCR_MUX(0);

  digitalWrite(PIN_SYN_ADDR0, bitRead(channel,0));
  digitalWrite(PIN_SYN_ADDR1, bitRead(channel,1));
  digitalWrite(PIN_SYN_ADDR2, bitRead(channel,2));
  //do we need to wait a few microseconds?
  delayMicroseconds(50);
  return analogRead(PIN_SYN_MUX_IO);
}

int potRead(uint8_t num) {
  if(num == 1) {
    return 1023-analogRead(POT_1);
  }
  if(num == 2) {
    return 1023-analogRead(POT_2);
  }
  return muxAnalogRead(num);
}

uint8_t muxDigitalRead(uint8_t channel) { 
  pinMode(PIN_SYN_MUX_IO, INPUT_PULLUP);
  digitalWrite(PIN_SYN_ADDR0, bitRead(channel,0));
  digitalWrite(PIN_SYN_ADDR1, bitRead(channel,1));
  digitalWrite(PIN_SYN_ADDR2, bitRead(channel,2));
  delayMicroseconds(50);
  //Wait a few microseconds for the selection to propagate. 
  uint8_t p = digitalRead(PIN_SYN_MUX_IO);
  return p;
}
