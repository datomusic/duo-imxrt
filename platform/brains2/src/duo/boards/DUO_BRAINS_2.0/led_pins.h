#ifndef LED_PINS_H_ZSKLTGQH
#define LED_PINS_H_ZSKLTGQH

#define PIN_LED_1            GPIO_08
#define PIN_LED_2            GPIO_05
#define PIN_LED_3            GPIO_01

#define write_osc_led(value) \
  analogWrite(PIN_LED_1, value);

#define write_filter_led(value) \
  analogWrite(PIN_LED_2, value);

#define write_env_led(value) \
  analogWrite(PIN_LED_3, value);


#endif /* end of include guard: LED_PINS_H_ZSKLTGQH */
