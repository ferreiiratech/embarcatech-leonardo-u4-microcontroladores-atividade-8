#include "system_config.h"

volatile uint64_t last_interrupt_time = 0;  // Tempo da última interrupção do botão
ssd1306_t ssd;                              // Inicializa a estrutura do display
volatile bool status_led_green = false;     // Variável para controlar o estado do LED verde
volatile bool status_led_red = false;       // Variável para controlar o estado do LED vermelho
volatile bool status_led_blue = false;      // Variável para controlar o estado do LED azul
uint slice_led_red;                         // o slice do LED vermelho
uint slice_led_blue;                        // o slice do LED azul

// Inicializa configuração o pino do led
void init_pin_gpio_config(uint pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, 0);
}

// Inicializa configuração do botao e determinação do pull up ou pull down
void init_button_settings(uint pin, bool out, bool is_pull_up) {
    gpio_init(pin);
    gpio_set_dir(pin, out);
    if(is_pull_up) gpio_pull_up(pin);
    else gpio_pull_down(pin);
}

// Função usada na interrupção dos botões
void button_a_isr(uint gpio, uint32_t events){
    // Obtém o tempo atual em microssegundos
    uint64_t current_time = time_us_64(); 

    // Ignora bouncing caso o tempo entre interrupções seja menor que 400ms
    if (current_time - last_interrupt_time < DEBOUNCE_TIME) return; 
    last_interrupt_time = current_time;

    // Se o botão A foi pressionado alterna onfiguração dos PWMs para ativado ou desativado
    if(gpio == BUTTON_PIN_A){
        printf("Button A is pressed\n");

        // Alterna o estado dos LEDs vermelho e azul
        status_led_blue = !status_led_blue;
        status_led_red = !status_led_red;

        // Atualiza os estados dos LEDs PWMs
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

// Inicialização do display OLED
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

// Configuração do pino como PWM
uint init_pin_pwm_config(uint pin, float divider, uint16_t wrap) {
    // Configuração do PWM
    gpio_set_function(pin, GPIO_FUNC_PWM);

    // Retorna o slice do PWM para o pino especificado
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_clkdiv(slice, divider);
    pwm_set_wrap(slice, wrap);
    pwm_set_gpio_level(pin, 0);

    // habilita o PWM do pino
    pwm_set_enabled(slice, true);

    return slice;
}

// Define o nível de PWM para um pino específico
void set_pwm_level(uint pin, uint16_t pulse_width) {
    uint level = (pulse_width * WRAP_PERIOD) / 20000;
    pwm_set_gpio_level(pin, level);
}