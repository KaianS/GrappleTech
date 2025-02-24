#include "hardware.h"
#include "config.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "pico/cyw43_arch.h"

void inicializar_hardware() {
    // Inicialização dos GPIOs
    gpio_init(BTN_START);
    gpio_set_dir(BTN_START, GPIO_IN);
    gpio_pull_up(BTN_START);
    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_up(BTN_A);
    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_B);

    // Inicialização do ADC
    adc_init();
    adc_gpio_init(FORCE_SENSOR_X);
    adc_gpio_init(FORCE_SENSOR_Y);
    adc_gpio_init(MIC_PIN);

    // Inicialização do I2C
    i2c_init(I2C_PORT, 400000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Inicialização do PWM para o LED
    iniciar_pwm(LED_PWM_PIN);

    // Inicialização da UART
    inicializar_uart();
}

void iniciar_pwm(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, 4095);
    pwm_set_gpio_level(slice_num, 0);
    pwm_set_enabled(slice_num, true);
}

void set_pwm_duty(uint gpio, uint16_t valor) {
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_gpio_level(gpio, valor);
}

void inicializar_uart() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}