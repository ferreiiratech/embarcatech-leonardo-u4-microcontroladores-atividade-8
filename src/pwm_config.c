#include "pwm_config.h"

void init_pin_gpio_config(uint pin) {
    // Configuração do pino como saída
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, 0);
}

// Configuração do pino como PWM
uint init_pin_pwm_config(uint pin, float divider, uint16_t wrap) {
    // Configuração do PWM
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_clkdiv(slice, divider);
    pwm_set_wrap(slice, wrap);
    pwm_set_gpio_level(pin, 0);
    pwm_set_enabled(slice, true);

    return slice;
}

// Define o nível de PWM para um pino específico
void set_pwm_level(uint pin, uint16_t pulse_width) {
    uint level = (pulse_width * WRAP_PERIOD) / 20000;
    pwm_set_gpio_level(pin, level);
}