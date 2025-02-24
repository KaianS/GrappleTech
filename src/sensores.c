#include <stdio.h>
#include "sensores.h"
#include "config.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

uint16_t offset_x = 0, offset_y = 0;

void calibrar_sensores() {
    printf("Calibrando joystick...\n");
    sleep_ms(500);
    offset_x = adc_read();
    sleep_ms(500);
    offset_y = adc_read();
    printf("Calibração concluída! Offsets - X: %d, Y: %d\n", offset_x, offset_y);
}

int16_t medir_forca_x() {
    adc_select_input(0);
    return adc_read() - offset_x;
}

int16_t medir_forca_y() {
    adc_select_input(1);
    return adc_read() - offset_y;
}

uint16_t ler_microfone() {
    adc_select_input(2);
    return adc_read();
}