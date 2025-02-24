#ifndef CONFIG_H
#define CONFIG_H

#include "pico/stdlib.h"

// Definições de hardware
#define BTN_START 22
#define BTN_A 5
#define BTN_B 6
#define LED_PWM_PIN 13
#define I2C_PORT i2c1
#define SDA_PIN 14
#define SCL_PIN 15
#define FORCE_SENSOR_X 26
#define FORCE_SENSOR_Y 27
#define BUZZER_PIN 21
#define LED_PIN 7
#define LED_COUNT 25
#define LED_ROWS 5
#define LED_COLS 5
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define MIC_PIN 28
#define SOUND_THRESHOLD 2300
#define BPM_LIMITE 160
#define DEBOUNCE_QUEDA 1000

// Definições das notas musicais
#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784
#define NOTE_A5 880
#define NOTE_B5 988
#define NOTE_C6 1047

#endif // CONFIG_H