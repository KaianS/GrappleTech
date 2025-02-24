#include "treino.h"
#include "config.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <stdio.h>

DadosTreino dados_treino = {0};
uint32_t amostras_bpm = 0;
uint32_t soma_bpm = 0;

void iniciar_dados_treino() {
    dados_treino.bpm_min = 0xFFFF;
    dados_treino.bpm_max = 0;
    dados_treino.bpm_medio = 0;
    dados_treino.forca_maxima = 0;
    dados_treino.quedas = 0;
}

void atualizar_dados_treino(uint32_t tempo_treino, uint16_t bpm, uint16_t forca, uint16_t pontos_a, uint16_t pontos_b) {
    dados_treino.duracao = tempo_treino;
    if (bpm > dados_treino.bpm_max) dados_treino.bpm_max = bpm;
    if (bpm < dados_treino.bpm_min) dados_treino.bpm_min = bpm;
    if (forca > dados_treino.forca_maxima) dados_treino.forca_maxima = forca;
    dados_treino.pontuacao_a = pontos_a;
    dados_treino.pontuacao_b = pontos_b;
    soma_bpm += bpm;
    amostras_bpm++;
    dados_treino.bpm_medio = (amostras_bpm > 0) ? (soma_bpm / amostras_bpm) : 0;
}

void enviar_dados_treino() {
    char buffer[256];
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
    printf("%s", buffer);
    uart_puts(UART_ID, buffer);
    sleep_ms(100);
}