#include "buzzer.h"
#include "config.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

uint slice_num_buzzer;
bool melodia_tocando = false;
uint32_t ultima_nota = 0;
uint8_t nota_atual = 0;

const Nota melodia_alerta[] = {
    {NOTE_C5, 150},
    {NOTE_E5, 150},
    {NOTE_G5, 150},
    {NOTE_C6, 300},
};
#define NUM_NOTAS (sizeof(melodia_alerta) / sizeof(Nota))

void configurar_buzzer(void) {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    slice_num_buzzer = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f);
    pwm_init(slice_num_buzzer, &config, true);
    pwm_set_gpio_level(slice_num_buzzer, 0);
}

void definir_frequencia_buzzer(uint16_t freq) {
    if (freq == 0) {
        pwm_set_gpio_level(slice_num_buzzer, 0);
        return;
    }
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint32_t top = clock_freq / freq - 1;
    pwm_set_wrap(slice_num, top);
    pwm_set_gpio_level(BUZZER_PIN, top / 2);
}

void desligar_buzzer(void) {
    definir_frequencia_buzzer(0);
    melodia_tocando = false;
    nota_atual = 0;
}

void tocar_alerta(void) {
    if (!melodia_tocando) {
        melodia_tocando = true;
        nota_atual = 0;
        ultima_nota = to_ms_since_boot(get_absolute_time());
        definir_frequencia_buzzer(melodia_alerta[0].freq);
    }
}

void atualizar_melodia(void) {
    if (!melodia_tocando) return;
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
    if (tempo_atual - ultima_nota >= melodia_alerta[nota_atual].duracao) {
        nota_atual++;
        if (nota_atual >= NUM_NOTAS) {
            nota_atual = 0;
        }
        definir_frequencia_buzzer(melodia_alerta[nota_atual].freq);
        ultima_nota = tempo_atual;
    }
}