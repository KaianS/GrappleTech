#ifndef CONFIG_H
#define CONFIG_H

#include "pico/stdlib.h" // Inclui a biblioteca padrão do SDK do Raspberry Pi Pico

/**
 * @file config.h
 * @brief Arquivo de configuração para o projeto.
 *
 * Este arquivo define constantes e configurações de hardware para o projeto,
 * incluindo pinos, frequências, limiares e definições de notas musicais.
 */

// Definições de hardware
#define BTN_START 22         /**< Pino do botão START. */
#define BTN_A 5              /**< Pino do botão A. */
#define BTN_B 6              /**< Pino do botão B. */
#define LED_PWM_PIN 13       /**< Pino do LED controlado por PWM. */
#define I2C_PORT i2c1        /**< Porta I2C utilizada (i2c1). */
#define SDA_PIN 14           /**< Pino SDA para comunicação I2C. */
#define SCL_PIN 15           /**< Pino SCL para comunicação I2C. */
#define FORCE_SENSOR_X 26    /**< Pino do sensor de força no eixo X. */
#define FORCE_SENSOR_Y 27    /**< Pino do sensor de força no eixo Y. */
#define BUZZER_PIN 21        /**< Pino do buzzer. */
#define LED_PIN 7            /**< Pino do LED simples. */
#define LED_COUNT 25         /**< Número total de LEDs na matriz. */
#define LED_ROWS 5           /**< Número de linhas na matriz de LEDs. */
#define LED_COLS 5           /**< Número de colunas na matriz de LEDs. */
#define UART_ID uart0        /**< Identificador da UART utilizada (uart0). */
#define BAUD_RATE 115200     /**< Taxa de baud da UART (115200 bps). */
#define UART_TX_PIN 0        /**< Pino TX da UART. */
#define UART_RX_PIN 1        /**< Pino RX da UART. */
#define MIC_PIN 28           /**< Pino do microfone. */
#define SOUND_THRESHOLD 2300 /**< Limiar de som para detecção (valor analógico). */
#define BPM_LIMITE 160        /**< Limite de batimentos por minuto (BPM). */
#define DEBOUNCE_QUEDA 1000   /**< Tempo de debounce para detecção de queda (em milissegundos). */

// Definições das notas musicais
#define NOTE_C5 523          /**< Frequência da nota C5 (523 Hz). */
#define NOTE_D5 587          /**< Frequência da nota D5 (587 Hz). */
#define NOTE_E5 659          /**< Frequência da nota E5 (659 Hz). */
#define NOTE_F5 698          /**< Frequência da nota F5 (698 Hz). */
#define NOTE_G5 784          /**< Frequência da nota G5 (784 Hz). */
#define NOTE_A5 880          /**< Frequência da nota A5 (880 Hz). */
#define NOTE_B5 988          /**< Frequência da nota B5 (988 Hz). */
#define NOTE_C6 1047         /**< Frequência da nota C6 (1047 Hz). */

#endif // CONFIG_H