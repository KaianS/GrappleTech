#ifndef TREINO_H
#define TREINO_H

#include <stdint.h> // Inclui tipos de dados de largura fixa (uint32_t, uint16_t, etc.)

/**
 * @file treino.h
 * @brief Cabeçalho para funções de gerenciamento de dados de treino.
 *
 * Este arquivo define a estrutura de dados do treino e as funções para
 * inicializar, atualizar e enviar os dados do treino.
 */

/**
 * @brief Estrutura para armazenar os dados do treino.
 *
 * Contém informações como duração do treino, BPM máximo, mínimo e médio,
 * força máxima, pontuação dos jogadores e número de quedas.
 */
typedef struct {
    uint32_t duracao;     /**< Duração do treino em segundos. */
    uint16_t bpm_max;     /**< Batimentos por minuto (BPM) máximo durante o treino. */
    uint16_t bpm_min;     /**< Batimentos por minuto (BPM) mínimo durante o treino. */
    uint16_t bpm_medio;   /**< Batimentos por minuto (BPM) médio durante o treino. */
    uint16_t forca_maxima; /**< Força máxima medida durante o treino. */
    uint16_t pontuacao_a; /**< Pontuação do jogador A. */
    uint16_t pontuacao_b; /**< Pontuação do jogador B. */
    uint16_t quedas;      /**< Número de quedas detectadas durante o treino. */
} DadosTreino;

/**
 * @brief Instância global da estrutura de dados do treino.
 *
 * Esta variável externa contém os dados do treino e é definida no arquivo treino.c.
 */
extern DadosTreino dados_treino;

/**
 * @brief Inicializa a estrutura de dados do treino.
 *
 * Define os valores iniciais para os dados do treino.
 */
void iniciar_dados_treino();

/**
 * @brief Atualiza os dados do treino com os valores fornecidos.
 *
 * Atualiza a duração do treino, os valores de BPM máximo, mínimo e médio,
 * a força máxima, a pontuação dos jogadores e o número de quedas.
 *
 * @param tempo_treino Duração do treino em segundos.
 * @param bpm Batimentos por minuto (BPM) atuais.
 * @param forca Valor da força medida.
 * @param pontos_a Pontuação do jogador A.
 * @param pontos_b Pontuação do jogador B.
 */
void atualizar_dados_treino(uint32_t tempo_treino, uint16_t bpm, uint16_t forca, uint16_t pontos_a, uint16_t pontos_b);

/**
 * @brief Envia os dados do treino via UART e imprime no console.
 *
 * Formata os dados do treino em JSON e envia pela UART e imprime no console.
 */
void enviar_dados_treino();

#endif // TREINO_H