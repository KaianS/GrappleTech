#ifndef HARDWARE_H
#define HARDWARE_H

#include "pico/stdlib.h" // Inclui a biblioteca padrão do SDK do Raspberry Pi Pico

/**
 * @file hardware.h
 * @brief Cabeçalho para funções de inicialização e controle de hardware.
 *
 * Este arquivo define as funções para configurar e controlar os periféricos de hardware,
 * como GPIOs, ADC, I2C, PWM e UART.
 */

/**
 * @brief Inicializa os periféricos de hardware necessários para o projeto.
 *
 * Configura os GPIOs, ADC, I2C, PWM e UART de acordo com as definições em config.h.
 */
void inicializar_hardware();

/**
 * @brief Inicializa um pino GPIO como saída PWM.
 *
 * Configura o pino especificado como saída PWM com um valor máximo de 4095.
 *
 * @param gpio O número do pino GPIO a ser configurado como PWM.
 */
void iniciar_pwm(uint gpio);

/**
 * @brief Define o duty cycle do PWM em um pino GPIO.
 *
 * Ajusta o nível do PWM no pino especificado para controlar o duty cycle.
 *
 * @param gpio O número do pino GPIO configurado como PWM.
 * @param valor O valor do duty cycle (0-4095).
 */
void set_pwm_duty(uint gpio, uint16_t valor);

/**
 * @brief Inicializa a interface UART para comunicação serial.
 *
 * Configura a UART com a taxa de baud e pinos TX/RX definidos em config.h.
 */
void inicializar_uart();

#endif // HARDWARE_H