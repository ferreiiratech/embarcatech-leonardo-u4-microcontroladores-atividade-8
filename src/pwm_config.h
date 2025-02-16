#ifndef PWM_CONFIG_H
#define PWM_CONFIG_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#define PWM_DIVISER 125.0
#define WRAP_PERIOD 20000

uint init_pin_pwm_config(uint pin, float divider, uint16_t wrap);
void set_pwm_level(uint pin, uint16_t pulse_width);
void init_pin_gpio_config(uint pin);

#endif