#ifndef LED_PINS_H_ZSKLTGQH
#define LED_PINS_H_ZSKLTGQH

#define PIN_LED_1            GPIO_08
#define PIN_LED_2            GPIO_07
#define PIN_LED_3            GPIO_06

#define write_osc_led(value)                                                   \
  analogWrite(PIN_LED_1, 128 - ((value) >> 3));

#define write_filter_led(value)                                                \
  analogWrite(PIN_LED_2, 1 + ((value * value) >> 13));

#define write_env_led(value)                                                   \
  analogWrite(PIN_LED_3, (value * 254.0f) + 1);

#endif /* end of include guard: LED_PINS_H_ZSKLTGQH */
