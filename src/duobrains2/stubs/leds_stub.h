#ifndef LEDS_STUB_H_DY6XTB7P
#define LEDS_STUB_H_DY6XTB7PLEDS_STUB_H_DY6XTB7P

const int ENV_LED = 0;
const int FILTER_LED = 0;
const int OSC_LED = 0;
const int LED_CLK = 1;
const int GRB = 1;
const int SK6812 = 1;
const int LED_DATA = 1;

const int led_order[NUM_LEDS] = {1};


struct Peak {
  float read() { return 0; };
};
Peak peak1;

#endif /* end of include guard: LEDS_STUB_H_DY6XTB7PLEDS_STUB_H_DY6XTB7P */
