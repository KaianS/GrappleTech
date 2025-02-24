#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware.h"
#include "display.h"
#include "leds.h"
#include "buzzer.h"
#include "sensores.h"
#include "treino.h"
#include "config.h"

bool treino_ativo = false;
uint32_t inicio_treino = 0;
uint8_t pontos_a = 0, pontos_b = 0;
uint16_t batimentos = 60, batimentos_alvo = 60;
int16_t forca_x = 0, forca_y = 0;
uint32_t ultimo_tempo_forca_baixa = 0;
uint32_t ultimo_tempo_queda = 0;

void ajustar_batimentos() {
    static int16_t forca_anterior_x = 0, forca_anterior_y = 0;
    int16_t variacao_x = abs(forca_x - forca_anterior_x);
    int16_t variacao_y = abs(forca_y - forca_anterior_y);
    int16_t variacao_forca = variacao_x + variacao_y;
    uint16_t total_forca = (abs(forca_x) + abs(forca_y)) / 2;

    if (variacao_forca > 1000) {
        batimentos_alvo += 10;
    } else if (variacao_forca < -1000) {
        batimentos_alvo -= 5;
    }

    if (total_forca < 200) {
        uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
        if (tempo_atual - ultimo_tempo_forca_baixa >= 10000) {
            batimentos_alvo -= 10;
            ultimo_tempo_forca_baixa = tempo_atual;
        }
    }

    if (batimentos_alvo < 40) batimentos_alvo = 40;
    if (batimentos_alvo > 180) batimentos_alvo = 180;

    if (batimentos < batimentos_alvo) {
        batimentos += 1 + (rand() % 2);
    } else if (batimentos > batimentos_alvo) {
        batimentos -= 1;
    }

    forca_anterior_x = forca_x;
    forca_anterior_y = forca_y;
}

int main() {
    stdio_init_all();
    inicializar_hardware();
    inicializar_oled();
    init_leds();
    configurar_buzzer();
    calibrar_sensores();
    iniciar_dados_treino();
    atualizar_oled(0, 60, 0, 0);

    while (true) {
        if (gpio_get(BTN_START) == 0) {
            if (!treino_ativo) {
                treino_ativo = true;
                inicio_treino = to_ms_since_boot(get_absolute_time());
                iniciar_dados_treino();
            } else {
                atualizar_dados_treino((to_ms_since_boot(get_absolute_time()) - inicio_treino) / 1000, batimentos, (abs(forca_x) + abs(forca_y)) / 2, pontos_a, pontos_b);
                enviar_dados_treino();

                // Limpar LEDs
                clear_leds();
                write_leds();
                // Resetar display OLED
                atualizar_oled(0, 60, 0, 0);

                // Resetar variáveis globais
                pontos_a = 0;
                pontos_b = 0;
                batimentos = 60;
                treino_ativo = false;
            }
            sleep_ms(300);
        }

        if (treino_ativo) {
            uint16_t nivel_som = ler_microfone();
            uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());

            if (nivel_som > SOUND_THRESHOLD && (tempo_atual - ultimo_tempo_queda) > DEBOUNCE_QUEDA) {
                dados_treino.quedas++;
                ultimo_tempo_queda = tempo_atual;
                tocar_alerta();
                for (int i = 0; i < LED_COUNT; i++) {
                    set_led(i, 255, 0, 0);
                }
                write_leds();
                sleep_ms(100);
                clear_leds();
                write_leds();
            }

            forca_x = medir_forca_x();
            forca_y = medir_forca_y();
            ajustar_batimentos();

            uint16_t total_forca = (abs(forca_x) + abs(forca_y)) / 2;
            atualizar_dados_treino((to_ms_since_boot(get_absolute_time()) - inicio_treino) / 1000, batimentos, total_forca, pontos_a, pontos_b);

            atualizar_oled((to_ms_since_boot(get_absolute_time()) - inicio_treino) / 1000, batimentos, total_forca, dados_treino.quedas);
            atualizar_matriz_leds(batimentos);

            if (batimentos >= BPM_LIMITE) {
                tocar_alerta();
            } else if (batimentos < BPM_LIMITE - 5) {
                desligar_buzzer();
            }

            if (melodia_tocando) {
                atualizar_melodia();
            }

            uint16_t intensidade = total_forca;
            if (intensidade > 4095) intensidade = 4095;
            set_pwm_duty(LED_PWM_PIN, intensidade);
        } else {
            desligar_buzzer();
        }

        if (gpio_get(BTN_A) == 0) {
            pontos_a++;
            sleep_ms(300);
        }
        if (gpio_get(BTN_B) == 0) {
            pontos_b++;
            sleep_ms(300);
        }

        sleep_ms(50);
    }
    return 0;
}