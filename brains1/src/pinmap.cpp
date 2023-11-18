#include "pinmap.h"

uint8_t muxDigitalRead(uint8_t channel) { 
  pinMode(MUX_IO, INPUT_PULLUP);
  digitalWriteFast(SYN_ADDR0, bitRead(channel,0));
  digitalWriteFast(SYN_ADDR1, bitRead(channel,1));
  digitalWriteFast(SYN_ADDR2, bitRead(channel,2));
  delayMicroseconds(50);
  //Wait a few microseconds for the selection to propagate. 
  uint8_t p = digitalRead(MUX_IO);
  return p;
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

int muxAnalogRead(uint8_t channel) { 
  // Any call to pinMode sets the port mux to GPIO mode.
  // We want to force it back to analog mode
  // Might be equivalent to pinMode(MUX_IO, INPUT_DISABLE);
  volatile uint32_t *config;
  config = portConfigRegister(MUX_IO);
  *config = PORT_PCR_MUX(0);

  digitalWriteFast(SYN_ADDR0, bitRead(channel,0));
  digitalWriteFast(SYN_ADDR1, bitRead(channel,1));
  digitalWriteFast(SYN_ADDR2, bitRead(channel,2));
  //do we need to wait a few microseconds?
  delayMicroseconds(50);
  return analogRead(MUX_IO);
}

void pins_init() {
  // PROG pin is unused but connected to reset via a resistor. Make output to be sure.
  pinMode(PROG, INPUT);

  pinMode(BITC_PIN, INPUT_PULLUP);
  pinMode(ACCENT_PIN, INPUT_PULLUP);

  pinMode(SYNC_OUT_PIN, OUTPUT);
  // pinMode(AMP_ENABLE, OUTPUT);
  pinMode(HP_ENABLE, OUTPUT);
  pinMode(SYNC_IN, INPUT);

  pinMode(JACK_DETECT, INPUT);
  pinMode(SYNC_DETECT, INPUT);

  pinMode(SYN_ADDR0, OUTPUT);
  pinMode(SYN_ADDR1, OUTPUT);
  pinMode(SYN_ADDR2, OUTPUT);

  // LED_CLK pin is connected to the last LED in the string for loopback testing
  pinMode(LED_CLK, INPUT_PULLUP);

  randomSeed(analogRead(UNCONNECTED_ANALOG));
}
