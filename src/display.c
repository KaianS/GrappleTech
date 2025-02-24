#include <stdio.h>
#include "display.h"
#include "config.h"
#include "pico/stdlib.h"
#include "ssd1306.h"

ssd1306_t display;

void inicializar_oled() {
    ssd1306_init(&display, 128, 64, false, 0x3C, I2C_PORT);
    ssd1306_config(&display);
}

void atualizar_oled(uint32_t tempo, uint16_t bpm, uint16_t forca, uint16_t quedas) {
    ssd1306_fill(&display, 0);
    char buffer[20];
    ssd1306_draw_string(&display, "GRAPPLE TECH", 10, 0);
    snprintf(buffer, sizeof(buffer), "TEMPO: %d s", tempo);
    ssd1306_draw_string(&display, buffer, 10, 10);
    snprintf(buffer, sizeof(buffer), "BPM: %d", bpm);
    ssd1306_draw_string(&display, buffer, 10, 20);
    snprintf(buffer, sizeof(buffer), "FORCA: %d", forca);
    ssd1306_draw_string(&display, buffer, 10, 30);
    snprintf(buffer, sizeof(buffer), "QUEDAS: %d", quedas);
    ssd1306_draw_string(&display, buffer, 10, 40);
    ssd1306_send_data(&display);
}