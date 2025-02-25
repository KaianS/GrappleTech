#include "treino.h"    // Inclui o cabeçalho personalizado para funções de treino
#include "config.h"     // Inclui o cabeçalho com configurações do projeto
#include "pico/stdlib.h" // Inclui a biblioteca padrão do SDK do Raspberry Pi Pico
#include "hardware/uart.h" // Inclui a biblioteca para controle da UART (Comunicação Serial)
#include <stdio.h>      // Inclui a biblioteca padrão de entrada/saída

// Estrutura para armazenar os dados do treino
DadosTreino dados_treino = {0};
uint32_t amostras_bpm = 0; // Contador de amostras de BPM
uint32_t soma_bpm = 0;    // Soma dos valores de BPM para cálculo da média

/**
 * @brief Inicializa a estrutura de dados do treino.
 *
 * Define os valores iniciais para os dados do treino, como BPM mínimo, máximo,
 * médio, força máxima e número de quedas.
 */
void iniciar_dados_treino() {
    dados_treino.bpm_min = 0xFFFF; // Define o BPM mínimo como o maior valor possível (inicialmente)
    dados_treino.bpm_max = 0;      // Define o BPM máximo como 0 (inicialmente)
    dados_treino.bpm_medio = 0;    // Define o BPM médio como 0 (inicialmente)
    dados_treino.forca_maxima = 0; // Define a força máxima como 0 (inicialmente)
    dados_treino.quedas = 0;       // Define o número de quedas como 0 (inicialmente)
}

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
void atualizar_dados_treino(uint32_t tempo_treino, uint16_t bpm, uint16_t forca, uint16_t pontos_a, uint16_t pontos_b) {
    dados_treino.duracao = tempo_treino; // Atualiza a duração do treino
    if (bpm > dados_treino.bpm_max) dados_treino.bpm_max = bpm; // Atualiza o BPM máximo
    if (bpm < dados_treino.bpm_min) dados_treino.bpm_min = bpm; // Atualiza o BPM mínimo
    if (forca > dados_treino.forca_maxima) dados_treino.forca_maxima = forca; // Atualiza a força máxima
    dados_treino.pontuacao_a = pontos_a; // Atualiza a pontuação do jogador A
    dados_treino.pontuacao_b = pontos_b; // Atualiza a pontuação do jogador B
    soma_bpm += bpm; // Adiciona o BPM atual à soma total
    amostras_bpm++; // Incrementa o contador de amostras
    dados_treino.bpm_medio = (amostras_bpm > 0) ? (soma_bpm / amostras_bpm) : 0; // Calcula o BPM médio
}

/**
 * @brief Envia os dados do treino via UART e imprime no console.
 *
 * Formata os dados do treino em JSON e envia pela UART e imprime no console.
 */
void enviar_dados_treino() {
    char buffer[256]; // Buffer para armazenar a string formatada
    // Formata os dados do treino em JSON
    snprintf(buffer, sizeof(buffer),
             "{"
             "\"duracao\":%lu,"
             "\"bpm_max\":%u,"
             "\"bpm_min\":%u,"
             "\"bpm_medio\":%u,"
             "\"forca_maxima\":%u,"
             "\"pontuacao_a\":%u,"
             "\"pontuacao_b\":%u,"
             "\"quedas\":%u"
             "}\n",
             dados_treino.duracao,
             dados_treino.bpm_max,
             dados_treino.bpm_min,
             dados_treino.bpm_medio,
             dados_treino.forca_maxima,
             dados_treino.pontuacao_a,
             dados_treino.pontuacao_b,
             dados_treino.quedas);
    printf("%s", buffer); // Imprime os dados no console
    uart_puts(UART_ID, buffer); // Envia os dados pela UART
    sleep_ms(100); // Aguarda 100 milissegundos
}