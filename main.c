#include <stdio.h>
#include "pico/stdlib.h"
#include "src/pwm_config.h"
#include "src/lib/ssd1306.h"

#define LED_PIN_GREEN 11
#define LED_PIN_BLUE 12
#define LED_PIN_RED 13
#define BUTTON_PIN_A 5
#define JOYSTICK_PIN_X 26
#define JOYSTICK_PIN_Y 27
#define JOYSTICK_PIN_BUTTON 22
#define DISPLAY_PIN_SDA 14
#define DISPLAY_PIN_SCL 15
#define DEBOUNCE_TIME 400000 // Tempo de debounce em microssegundos
#define ADDRESS 0x3C // Endereço do display OLED
#define I2C_PORT i2c1 // Porta I2C

volatile uint64_t last_interrupt_time = 0; // Tempo da última interrupção do botão A
volatile int pwm_level = 0;
volatile bool status_led_green = false;
volatile bool status_led_red = false;
volatile bool status_led_blue = false;
uint slice_led_red;
uint slice_led_blue;
ssd1306_t ssd; // Inicializa a estrutura do display

void button_a_isr(uint gpio, uint32_t events);
void init_display_settings();

int main()
{
    // Inicialização das configurações de I/O 
    stdio_init_all();

    init_display_settings();

    init_pin_gpio_config(LED_PIN_GREEN);

    slice_led_red = init_pin_pwm_config(LED_PIN_BLUE, PWM_DIVISER, WRAP_PERIOD);
    slice_led_blue = init_pin_pwm_config(LED_PIN_RED, PWM_DIVISER, WRAP_PERIOD);

    gpio_init(BUTTON_PIN_A);
    gpio_set_dir(BUTTON_PIN_A, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_A);

    gpio_init(JOYSTICK_PIN_BUTTON);
    gpio_set_dir(JOYSTICK_PIN_BUTTON, GPIO_IN);
    gpio_pull_up(JOYSTICK_PIN_BUTTON);

    gpio_set_irq_enabled_with_callback(BUTTON_PIN_A, GPIO_IRQ_EDGE_FALL, true, &button_a_isr);
    gpio_set_irq_enabled_with_callback(JOYSTICK_PIN_BUTTON, GPIO_IRQ_EDGE_FALL, true, &button_a_isr);

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}

void button_a_isr(uint gpio, uint32_t events){
    uint64_t current_time = time_us_64(); // Obtém o tempo atual em microssegundos

    if (current_time - last_interrupt_time < DEBOUNCE_TIME) return; // Ignora bouncing caso o tempo entre interrupções seja menor que 400ms
    last_interrupt_time = current_time;

    // Se o botão A foi pressionado alterna onfiguração dos PWMs para ativado ou desativado
    if(gpio == BUTTON_PIN_A){
        printf("Button A is pressed\n");

        status_led_blue = !status_led_blue;
        status_led_red = !status_led_red;

        pwm_set_enabled(slice_led_blue, status_led_blue);
        pwm_set_enabled(slice_led_red, status_led_red);
    }
    
    // Se o botão B foi pressionado alterna o estado do LED verde
    if(gpio == JOYSTICK_PIN_BUTTON){
        printf("Button B is pressed\n");
        status_led_green = !status_led_green;
        gpio_put(LED_PIN_GREEN, status_led_green);
    }
}

void init_display_settings() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(DISPLAY_PIN_SDA, GPIO_FUNC_I2C);
    gpio_set_function(DISPLAY_PIN_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(DISPLAY_PIN_SDA);
    gpio_pull_up(DISPLAY_PIN_SCL);

    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ADDRESS, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
}
