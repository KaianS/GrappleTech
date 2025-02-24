#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

void init_leds(void);
void clear_leds(void);
void set_led(int index, uint8_t r, uint8_t g, uint8_t b);
void write_leds(void);
void atualizar_matriz_leds(uint16_t bpm);

#endif // LEDS_H