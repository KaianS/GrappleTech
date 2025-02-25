#include "hardware.h"   // Inclui o cabeçalho personalizado para funções de hardware
#include "config.h"    // Inclui o cabeçalho com configurações do projeto
#include "pico/stdlib.h" // Inclui a biblioteca padrão do SDK do Raspberry Pi Pico
#include "hardware/adc.h" // Inclui a biblioteca para controle do ADC (Conversor Analógico-Digital)
#include "hardware/i2c.h" // Inclui a biblioteca para controle da interface I2C
#include "hardware/pwm.h" // Inclui a biblioteca para controle de PWM (Modulação por Largura de Pulso)
#include "pico/cyw43_arch.h" // Inclui a biblioteca para controle do chip CYW43 (Wi-Fi/Bluetooth)

/**
 * @brief Inicializa os periféricos de hardware necessários para o projeto.
 *
 * Configura os GPIOs, ADC, I2C, PWM e UART de acordo com as definições em config.h.
 */
void inicializar_hardware() {
    // Inicialização dos GPIOs para botões
    gpio_init(BTN_START);
    gpio_set_dir(BTN_START, GPIO_IN);
    gpio_pull_up(BTN_START); // Configura o pino com pull-up interno
    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_up(BTN_A);
    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_B);

    // Inicialização do ADC para sensores de força e microfone
    adc_init();
    adc_gpio_init(FORCE_SENSOR_X);
    adc_gpio_init(FORCE_SENSOR_Y);
    adc_gpio_init(MIC_PIN);

    // Inicialização da interface I2C para comunicação com dispositivos I2C
    i2c_init(I2C_PORT, 400000); // Inicializa a porta I2C com velocidade de 400kHz
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C); // Configura o pino SDA para função I2C
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C); // Configura o pino SCL para função I2C
    gpio_pull_up(SDA_PIN); // Configura o pino SDA com pull-up interno
    gpio_pull_up(SCL_PIN); // Configura o pino SCL com pull-up interno

    // Inicialização do PWM para o LED controlado por PWM
    iniciar_pwm(LED_PWM_PIN);

    // Inicialização da UART para comunicação serial
    inicializar_uart();
}

/**
 * @brief Inicializa um pino GPIO como saída PWM.
 *
 * Configura o pino especificado como saída PWM com um valor máximo de 4095.
 *
 * @param gpio O número do pino GPIO a ser configurado como PWM.
 */
void iniciar_pwm(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM); // Configura o pino como função PWM
    uint slice_num = pwm_gpio_to_slice_num(gpio); // Obtém o número da fatia PWM associada ao pino
    pwm_set_wrap(slice_num, 4095); // Define o valor máximo do contador PWM (4095)
    pwm_set_gpio_level(slice_num, 0); // Define o nível inicial do PWM para 0 (desligado)
    pwm_set_enabled(slice_num, true); // Habilita a fatia PWM
}

/**
 * @brief Define o duty cycle do PWM em um pino GPIO.
 *
 * Ajusta o nível do PWM no pino especificado para controlar o duty cycle.
 *
 * @param gpio O número do pino GPIO configurado como PWM.
 * @param valor O valor do duty cycle (0-4095).
 */
void set_pwm_duty(uint gpio, uint16_t valor) {
    uint slice_num = pwm_gpio_to_slice_num(gpio); // Obtém o número da fatia PWM associada ao pino
    pwm_set_gpio_level(gpio, valor); // Define o nível do PWM para o valor especificado
}

/**
 * @brief Inicializa a interface UART para comunicação serial.
 *
 * Configura a UART com a taxa de baud e pinos TX/RX definidos em config.h.
 */
void inicializar_uart() {
    uart_init(UART_ID, BAUD_RATE); // Inicializa a UART com a taxa de baud definida
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); // Configura o pino TX para função UART
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); // Configura o pino RX para função UART
}