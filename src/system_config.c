#include "system_config.h"

volatile uint64_t last_interrupt_time = 0;  // Tempo da última interrupção do botão
volatile bool status_led_green = false;     // Variável para controlar o estado do LED verde
volatile bool status_led_red = true;        // Variável para controlar o estado do LED vermelho
volatile bool status_led_blue = true;       // Variável para controlar o estado do LED azul
volatile uint16_t JOYSTICK_POSITION_X;      // Posição do joystick na direção X
volatile uint16_t JOYSTICK_POSITION_Y;      // Posição do joystick na direção Y
volatile uint8_t square_x = 60;             // Posição inicial aproximada no centro do display no eixo x
volatile uint8_t square_y = 28;             // Posição inicial aproximada no centro do display no eixo y
const uint8_t SQUARE_SIZE = 8;              // Tamanho do quadrado
const uint8_t SCREEN_WIDTH = 128;           // Largura do display
const uint8_t SCREEN_HEIGHT = 64;           // Altura do display
const uint16_t JOYSTICK_MIN = 0;            // Valor mínimo do joystick
const uint16_t JOYSTICK_MAX = 4095;         // Valor máximo do joystick
const uint16_t JOYSTICK_CENTER = 2048;      // Valor central do joystick
const uint8_t MOVEMENT_THRESHOLD = 80;      // Reduz sensibilidade
uint8_t border_style = 0;                   // Estado do estilo de borda
uint slice_led_red;                         // o slice do LED vermelho
uint slice_led_blue;                        // o slice do LED azul
ssd1306_t ssd;                              // Inicializa a estrutura do display

void draw_border() {
    switch (border_style) {
        case 0:
            // Sem borda
            break;
        case 1:
            // Borda simples
            ssd1306_rect(&ssd, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 0);
            break;
        case 2:
            // Dupla linha
            ssd1306_rect(&ssd, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 0);
            ssd1306_rect(&ssd, 2, 2, SCREEN_WIDTH - 4, SCREEN_HEIGHT - 4, 1, 0);
            break;
        case 3:
            // Cantos arredondados
            ssd1306_rect(&ssd, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 0);
            ssd1306_pixel(&ssd, 0, 0, 0);
            ssd1306_pixel(&ssd, 1, 1, 0);
            ssd1306_pixel(&ssd, SCREEN_WIDTH - 1, 0, 0);
            ssd1306_pixel(&ssd, SCREEN_WIDTH - 2, 1, 0);
            ssd1306_pixel(&ssd, 0, SCREEN_HEIGHT - 1, 0);
            ssd1306_pixel(&ssd, 1, SCREEN_HEIGHT - 2, 0);
            ssd1306_pixel(&ssd, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0);
            ssd1306_pixel(&ssd, SCREEN_WIDTH - 2, SCREEN_HEIGHT - 2, 0);
            break;
        case 4:
            // Borda de bolinhas espaçadas
            for (uint8_t x = 0; x < SCREEN_WIDTH; x += 4) {
                ssd1306_pixel(&ssd, x, 0, 1); // Linha superior
                ssd1306_pixel(&ssd, x, SCREEN_HEIGHT - 1, 1); // Linha inferior
            }
            for (uint8_t y = 0; y < SCREEN_HEIGHT; y += 4) {
                ssd1306_pixel(&ssd, 0, y, 1); // Linha esquerda
                ssd1306_pixel(&ssd, SCREEN_WIDTH - 1, y, 1); // Linha direita
            }
            break;
    }
}

// Função que desenha um quadrado no display OLED
void update_display() {
    // Limpa o buffer antes de desenhar
    ssd1306_fill(&ssd, false);

    // Desenha a borda
    draw_border();

    // Redesenha o quadrado na posição atual
    ssd1306_rect(&ssd, square_y, square_x, SQUARE_SIZE, SQUARE_SIZE, true, true);

    // Envia os dados para o display
    ssd1306_send_data(&ssd);
}

// Inicializa a configuração do joystick
void init_joystick_settings() {
    adc_init();
    adc_gpio_init(JOYSTICK_PIN_X);
    adc_gpio_init(JOYSTICK_PIN_Y);
}

// Adapta o nível do PWM para os LEDs em relação ao centro do joystick
uint16_t adapt_pwm_level(uint16_t pwm_level) {
    if(pwm_level > ZONE_OFF_LED) {
        pwm_level = pwm_level - ZONE_OFF_LED;
    } else {
        pwm_level = 0;
    }

    return pwm_level;
}

// Função que atualiza a posição do quadrado com base nas leituras do joystick
void update_square_position(uint16_t joystick_x, uint16_t joystick_y) {
    // Mapeia os valores do joystick (0-4095) para a tela (0-120 largura, 0-56 altura)
    square_x = (joystick_x * (SCREEN_WIDTH - SQUARE_SIZE)) / JOYSTICK_MAX;
    square_y = (SCREEN_HEIGHT - SQUARE_SIZE) - (joystick_y * (SCREEN_HEIGHT - SQUARE_SIZE)) / JOYSTICK_MAX;
    update_display();
}

// Função que atualiza os LEDs PWM com base nas posições do joystick
void update_led_pwm(uint16_t joystick_x, uint16_t joystick_y) {
    // Calcula a intensidade do PWM baseado no eixo X (LED Vermelho)
    uint16_t pwm_red = abs(joystick_x - JOYSTICK_CENTER) * 4096 / JOYSTICK_CENTER;
    
    // Calcula a intensidade do PWM baseado no eixo Y (LED Azul)
    uint16_t pwm_blue = abs(joystick_y - JOYSTICK_CENTER) * 4096 / JOYSTICK_CENTER;

    pwm_red = adapt_pwm_level(pwm_red);
    pwm_blue = adapt_pwm_level(pwm_blue);

    printf("X: %d, Y: %d\n", pwm_red, pwm_blue);
    
    set_pwm_level(LED_PIN_RED, pwm_red);
    set_pwm_level(LED_PIN_BLUE, pwm_blue);
}

// Função que lê as posições do joystick e atualiza as posições x e y do quadrado
void read_joystick_positions() {
    adc_select_input(JOYSTICK_ADC_CHANNEL_X);
    uint16_t new_x = adc_read();
    adc_select_input(JOYSTICK_ADC_CHANNEL_Y);
    uint16_t new_y = adc_read();

    // Apenas atualiza se houver uma mudança realmente significativa
    if (abs(new_x - JOYSTICK_POSITION_X) > MOVEMENT_THRESHOLD || 
        abs(new_y - JOYSTICK_POSITION_Y) > MOVEMENT_THRESHOLD) {
        
        JOYSTICK_POSITION_X = new_x;
        JOYSTICK_POSITION_Y = new_y;
        update_square_position(JOYSTICK_POSITION_X, JOYSTICK_POSITION_Y);
    }

    update_led_pwm(JOYSTICK_POSITION_X, JOYSTICK_POSITION_Y);
}
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

        border_style = (border_style + 1) % 5;
        update_display();
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
uint init_pin_pwm_config(uint pin, float divider, uint16_t wrap, bool status) {
    // Configuração do PWM
    gpio_set_function(pin, GPIO_FUNC_PWM);

    // Retorna o slice do PWM para o pino especificado
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_clkdiv(slice, divider);
    pwm_set_wrap(slice, wrap);
    pwm_set_gpio_level(pin, 0);

    // habilita o PWM do pino
    pwm_set_enabled(slice, status);

    return slice;
}

// Define o nível de PWM para um pino específico
void set_pwm_level(uint pin, uint16_t pulse_width) {
    pwm_set_gpio_level(pin, pulse_width);
}