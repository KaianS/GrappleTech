#ifndef HARDWARE_H
#define HARDWARE_H

#include "pico/stdlib.h"

void inicializar_hardware();
void iniciar_pwm(uint gpio);
void set_pwm_duty(uint gpio, uint16_t valor);
void inicializar_uart();

#endif // HARDWARE_H