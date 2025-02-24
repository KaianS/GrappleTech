#ifndef SENSORES_H
#define SENSORES_H

#include <stdint.h>

void calibrar_sensores();
int16_t medir_forca_x();
int16_t medir_forca_y();
uint16_t ler_microfone();

#endif // SENSORES_H