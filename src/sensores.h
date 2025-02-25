#ifndef SENSORES_H
#define SENSORES_H

#include <stdint.h> // Inclui tipos de dados de largura fixa (int16_t, uint16_t, etc.)

/**
 * @file sensores.h
 * @brief Cabeçalho para funções de leitura e calibração de sensores.
 *
 * Este arquivo define as funções para calibrar os sensores de força e ler
 * os valores dos sensores de força e do microfone.
 */

/**
 * @brief Calibra os sensores de força nos eixos X e Y.
 *
 * Realiza a leitura dos valores dos sensores em repouso e armazena os offsets
 * para compensar as leituras futuras.
 */
void calibrar_sensores();

/**
 * @brief Mede a força no eixo X.
 *
 * Seleciona o canal do ADC correspondente ao sensor de força no eixo X,
 * realiza a leitura e subtrai o offset para obter o valor calibrado.
 *
 * @return O valor da força medida no eixo X, compensado pelo offset.
 */
int16_t medir_forca_x();

/**
 * @brief Mede a força no eixo Y.
 *
 * Seleciona o canal do ADC correspondente ao sensor de força no eixo Y,
 * realiza a leitura e subtrai o offset para obter o valor calibrado.
 *
 * @return O valor da força medida no eixo Y, compensado pelo offset.
 */
int16_t medir_forca_y();

/**
 * @brief Lê o nível de som do microfone.
 *
 * Seleciona o canal do ADC correspondente ao microfone e realiza a leitura.
 *
 * @return O valor lido do microfone.
 */
uint16_t ler_microfone();

#endif // SENSORES_H