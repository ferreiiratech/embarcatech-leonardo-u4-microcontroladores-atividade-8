#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "src/lib/ssd1306.h"
#include "hardware/adc.h"

#define PWM_DIVISER 125.0                       // Divisor do PWM
#define WRAP_PERIOD 20000                       // Período do PWM
#define DEBOUNCE_TIME 400000                    // Tempo de debounce em microssegundos
#define BUTTON_PIN_A 5                          // Pino do botão A
#define JOYSTICK_PIN_BUTTON 22                  // Pino do botão do joystick
#define LED_PIN_GREEN 11                        // Pino do LED verde
#define LED_PIN_BLUE 12                         // Pino do LED azul
#define LED_PIN_RED 13                          // Pino do LED vermelho
#define JOYSTICK_PIN_X 26                       // Pino do joystick na direção X
#define JOYSTICK_PIN_Y 27                       // Pino do joystick na direção Y
#define DISPLAY_PIN_SDA 14                      // Pino SDA do display OLED
#define DISPLAY_PIN_SCL 15                      // Pino SCL do display OLED
#define ADDRESS 0x3C                            // Endereço do display OLED
#define I2C_PORT i2c1                           // Porta I2C
#define JOYSTICK_ADC_CHANNEL_X 1                 // Canal ADC do joystick na direção X
#define JOYSTICK_ADC_CHANNEL_Y 0                 // Canal ADC do joystick na direção Y


extern volatile uint64_t last_interrupt_time;   // Tempo da última interrupção do botão A
extern volatile bool status_led_green;          // Variável para controlar o estado do LED verde
extern volatile bool status_led_red;            // Variável para controlar o estado do LED vermelho
extern volatile bool status_led_blue;           // Variável para controlar o estado do LED azul
extern uint slice_led_red;                      // Slice do PWM do LED vermelho
extern uint slice_led_blue;                     // Slice do PWM do LED azul
extern volatile uint16_t JOYSTICK_POSITION_X;   // Posição do joystick na direção X
extern volatile uint16_t JOYSTICK_POSITION_Y;   // Posição do joystick na direção Y
extern volatile uint8_t square_x;  // Posição inicial aproximada no centro
extern volatile uint8_t square_y;

void init_pin_gpio_config(uint pin);
void init_button_settings(uint pin, bool out, bool is_pull_up);
void button_a_isr(uint gpio, uint32_t events);
void init_display_settings();
uint init_pin_pwm_config(uint pin, float divider, uint16_t wrap);
void set_pwm_level(uint pin, uint16_t pulse_width);
void draw_square(uint8_t x, uint8_t y);
void init_joystick_settings();
void read_joystick_positions();

#endif