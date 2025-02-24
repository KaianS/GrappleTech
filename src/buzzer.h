#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
#include "pico/stdlib.h"

typedef struct {
    uint16_t freq;
    uint16_t duracao;
} Nota;

void configurar_buzzer(void);
void definir_frequencia_buzzer(uint16_t freq);
void desligar_buzzer(void);
void tocar_alerta(void);
void atualizar_melodia(void);

extern bool melodia_tocando; // Adicionado

#endif // BUZZER_H