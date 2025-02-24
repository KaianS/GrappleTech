#include "leds.h"
#include "config.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2818b.pio.h"

PIO np_pio;
uint sm;
struct pixel_t {
    uint8_t G, R, B;
};
typedef struct pixel_t npLED_t;
npLED_t leds[LED_COUNT];

void init_leds(void) {
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
    sm = pio_claim_unused_sm(np_pio, true);
    ws2818b_program_init(np_pio, sm, offset, LED_PIN, 800000.f);
    clear_leds();
    write_leds();
}

void clear_leds(void) {
    for (int i = 0; i < LED_COUNT; i++) {
        leds[i].R = leds[i].G = leds[i].B = 0;
    }
}

void set_led(int index, uint8_t r, uint8_t g, uint8_t b) {
    if (index < LED_COUNT) {
        leds[index].R = r;
        leds[index].G = g;
        leds[index].B = b;
    }
}

void write_leds(void) {
    for (int i = 0; i < LED_COUNT; i++) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
}

void atualizar_matriz_leds(uint16_t bpm) {
    clear_leds();
    int linhas_acesas = 0;
    if (bpm >= 140) linhas_acesas = 5;
    else if (bpm >= 120) linhas_acesas = 4;
    else if (bpm >= 100) linhas_acesas = 3;
    else if (bpm >= 80) linhas_acesas = 2;
    else if (bpm >= 60) linhas_acesas = 1;

    for (int linha = 0; linha < linhas_acesas; linha++) {
        for (int col = 0; col < LED_COLS; col++) {
            int led_index = (LED_ROWS - 1 - linha) * LED_COLS + col;
            set_led(led_index, 50, 0, 0);
        }
    }
    write_leds();
}