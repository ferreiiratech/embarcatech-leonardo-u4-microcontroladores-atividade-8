# Projeto Conversores A/D

Esse projeto consiste na implementação de um conversor analógico-digital (ADC) utilizando o **RP2040**, explorando as funcionalidades da placa **BitDogLab**. O sistema permite a interação com um **joystick**, um **display OLED SSD1306** e um **LED RGB**, utilizando **PWM** para controle de intensidade luminosa.

## Descrição do projeto

Este projeto visa:
- Compreender o funcionamento do conversor analógico-digital (ADC) no RP2040.
- Controlar a intensidade de dois LEDs RGB via PWM, com base nos valores do joystick.
- Exibir no display SSD1306 a posição do joystick por meio de um quadrado móvel (8x8 pixels).
- Utilizar o protocolo I2C para comunicação com o display.
- Implementar tratamento de bouncing via software para os botões.
- Utilizar rotinas de interrupção (IRQ) para gerenciar os eventos dos botões.

## Execução do programa

[]()

## Requisitos

Para rodar este projeto, você precisará de:
- **Raspberry Pi Pico**
- **Display OLED SSD1306**
- **JOYSTICK**
- **LED RGB**
- **Push Button**
- **Software para compilação do Pico SDK**
- **CMake e Make**
- **Plugin do Raspberry Pi Pico no Visual Studio Code**
- **Simulador Wokwi no Visual Studio Code**

## Funcionalidades
**1. Movimento do Joystick**:
- Ao movimentar o joystick, é possível controlar a posição do quadrado 8x8 no display OLED.
- Ao movimentar o joystick para cima ou para baixo, ou seja, ao longo do eixo Y, podemos alterar a intensidade do LED Azul.
- Ao movimentar o joystick para a esquerda ou para a direita, ou seja, ao longo do eixo X, podemos alterar a intensidade do LED Vermelho.
- Os leds são controlados via PWM para que seja possível controlar a intensidade dos leds suavemente.

**2. Botão do Joystick**: 
- Ao pressionar o botão do joystick, é possível alterar o estado do LED Verde.
- Ademais, podemos alterar o estilo da borda do display OLED.

**3. Push Button A**:
- Ao pressionar o botão A, é posssível ativar ou desativar os leds PWM. Assim, eles podem ser acessender ou não ao movimentar o joystick.

## Como compilar e executar
1. Configure o PICO-SDK no seu ambiente de desenvolvimento usando a extensão Raspberry Pi Pico no Visual Studio Code.
2. Abra o arquivo correspondente no Visual Studio Code.
3. Compile o firmware usando o botão de compilação no Visual Studio Code.
4. Conecte a placa BitDogLab ao computador no modo BOOTSEL.
5. Clique no botão denominado "run" no Visual Studio Code para carregar o firmware na placa.
6. A placa executará automaticamente o firmware.

## Video de apresentação
Link: []()

## Autor
Leonardo Ferreira Lima

Email: ferreiiratech@gmail.com