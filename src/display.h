#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h> // Inclui tipos de dados de largura fixa (uint32_t, uint16_t, etc.)

/**
 * @file display.h
 * @brief Cabeçalho para funções de controle do display OLED.
 *
 * Este arquivo define as funções para inicializar e atualizar o display OLED.
 */

/**
 * @brief Inicializa o display OLED.
 *
 * Configura o display OLED para uso, incluindo a inicialização da comunicação I2C
 * e a configuração das dimensões do display.
 */
void inicializar_oled();

/**
 * @brief Atualiza o conteúdo do display OLED com os dados fornecidos.
 *
 * Exibe informações como tempo decorrido, batimentos por minuto (BPM), força medida
 * e número de quedas detectadas no display OLED.
 *
 * @param tempo Tempo decorrido em segundos.
 * @param bpm Batimentos por minuto (BPM).
 * @param forca Valor da força medida.
 * @param quedas Número de quedas detectadas.
 */
void atualizar_oled(uint32_t tempo, uint16_t bpm, uint16_t forca, uint16_t quedas);

#endif // DISPLAY_H