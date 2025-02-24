#ifndef TREINO_H
#define TREINO_H

#include <stdint.h>

typedef struct {
    uint32_t duracao;
    uint16_t bpm_max;
    uint16_t bpm_min;
    uint16_t bpm_medio;
    uint16_t forca_maxima;
    uint16_t pontuacao_a;
    uint16_t pontuacao_b;
    uint16_t quedas;
} DadosTreino;

extern DadosTreino dados_treino;

void iniciar_dados_treino();
void atualizar_dados_treino(uint32_t tempo_treino, uint16_t bpm, uint16_t forca, uint16_t pontos_a, uint16_t pontos_b);
void enviar_dados_treino();

#endif // TREINO_H