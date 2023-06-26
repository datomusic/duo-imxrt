#ifndef LED_PINS_H_ZSKLTGQH
#define LED_PINS_H_ZSKLTGQH

#define PIN_LED_1 GPIO_08
#define PIN_LED_2 GPIO_05
#define PIN_LED_3 GPIO_01

#define write_env_led(value)                                                   \
  analogWrite(PIN_LED_3, 254 - (uint16_t)(value * 254.0f));

#define write_osc_led(value)                                                   \
  analogWrite(PIN_LED_1, (uint16_t)(value / 4.03f) + 1);

#define write_filter_led(value)                                                \
  analogWrite(PIN_LED_2, 254 - (uint16_t)((value * value) / 4128));

#endif /* end of include guard: LED_PINS_H_ZSKLTGQH */
