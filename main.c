#include <stdio.h>
#include "pico/stdlib.h"
#include "src/system_config.h"

int main()
{
    // Inicialização das configurações de I/O 
    stdio_init_all();

    // Inicialização das configurações do display OLED
    init_display_settings();

    // Inicialização da configuração do led verde
    init_pin_gpio_config(LED_PIN_GREEN);

    // Inicializa a configuração do joystick
    init_joystick_settings();
 
    // Inicialização das configurações do led vermelho e azul recebendo o slice do PWM
    slice_led_red = init_pin_pwm_config(LED_PIN_BLUE, PWM_DIVISER, WRAP_PERIOD, status_led_red);
    slice_led_blue = init_pin_pwm_config(LED_PIN_RED, PWM_DIVISER, WRAP_PERIOD, status_led_blue);

    // Inicialização das configurações do botão A e do joystick
    init_button_settings(BUTTON_PIN_A, GPIO_IN, true);
    init_button_settings(JOYSTICK_PIN_BUTTON, GPIO_IN, true);

    // Configuração de interrupção para o botão A e o joystick
    gpio_set_irq_enabled_with_callback(BUTTON_PIN_A, GPIO_IRQ_EDGE_FALL, true, &button_a_isr);
    gpio_set_irq_enabled_with_callback(JOYSTICK_PIN_BUTTON, GPIO_IRQ_EDGE_FALL, true, &button_a_isr);

    // Atualiza display
    update_display();

    while (true) {

        // Ler a posição x e y do joystick
        read_joystick_positions();

        printf("Mova o joystick!\n");
        sleep_ms(5);
    }
}
