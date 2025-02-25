#ifndef LEDS_H
#define LEDS_H

#include <stdint.h> // Inclui tipos de dados de largura fixa (uint8_t, uint16_t, etc.)

/**
 * @file leds.h
 * @brief Cabeçalho para funções de controle da matriz de LEDs WS2818B.
 *
 * Este arquivo define as funções para inicializar, limpar, definir cores e atualizar
 * a matriz de LEDs WS2818B.
 */

/**
 * @brief Inicializa a matriz de LEDs WS2818B.
 *
 * Configura o PIO (Programmable I/O) e a State Machine para controlar os LEDs.
 */
void init_leds(void);

/**
 * @brief Limpa todos os LEDs da matriz.
 *
 * Define todas as componentes RGB de cada LED como zero (desligado).
 */
void clear_leds(void);

/**
 * @brief Define a cor de um LED específico na matriz.
 *
 * Define as componentes RGB de um LED no índice especificado.
 *
 * @param index O índice do LED na matriz.
 * @param r Componente vermelha (0-255).
 * @param g Componente verde (0-255).
 * @param b Componente azul (0-255).
 */
void set_led(int index, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Atualiza os LEDs com os dados armazenados no array interno.
 *
 * Envia os dados RGB para cada LED usando o PIO e a State Machine configurados.
 */
void write_leds(void);

/**
 * @brief Atualiza a matriz de LEDs com base no valor de BPM.
 *
 * Acende um número de linhas de LEDs proporcional ao valor de BPM.
 *
 * @param bpm Batimentos por minuto (BPM).
 */
void atualizar_matriz_leds(uint16_t bpm);

#endif // LEDS_H