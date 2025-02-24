#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

void inicializar_oled();
void atualizar_oled(uint32_t tempo, uint16_t bpm, uint16_t forca, uint16_t quedas);

#endif // DISPLAY_H