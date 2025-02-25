#include <stdio.h>      // Inclui a biblioteca padrão de entrada/saída
#include "display.h"    // Inclui o cabeçalho personalizado para funções de display
#include "config.h"     // Inclui o cabeçalho com configurações do projeto
#include "pico/stdlib.h" // Inclui a biblioteca padrão do SDK do Raspberry Pi Pico
#include "ssd1306.h"    // Inclui a biblioteca para controle do display OLED SSD1306

// Instância da estrutura ssd1306_t para controlar o display OLED
ssd1306_t display;

/**
 * @brief Inicializa o display OLED SSD1306.
 *
 * Configura o display OLED com as dimensões, endereço I2C e porta I2C definidos em config.h.
 */
void inicializar_oled() {
    ssd1306_init(&display, 128, 64, false, 0x3C, I2C_PORT); // Inicializa o display com 128x64 pixels, endereço 0x3C e porta I2C definida
    ssd1306_config(&display); // Configura o display com as configurações padrão
}

/**
 * @brief Atualiza o conteúdo do display OLED com os dados fornecidos.
 *
 * Limpa o display, formata e exibe os dados de tempo, BPM, força e número de quedas.
 *
 * @param tempo Tempo decorrido em segundos.
 * @param bpm Batimentos por minuto (BPM).
 * @param forca Valor da força medida.
 * @param quedas Número de quedas detectadas.
 */
void atualizar_oled(uint32_t tempo, uint16_t bpm, uint16_t forca, uint16_t quedas) {
    ssd1306_fill(&display, 0); // Limpa o display preenchendo com preto (0)
    char buffer[20]; // Buffer para armazenar strings formatadas

    // Exibe o título "GRAPPLE TECH"
    ssd1306_draw_string(&display, "GRAPPLE TECH", 10, 0);

    // Formata e exibe o tempo decorrido
    snprintf(buffer, sizeof(buffer), "TEMPO: %d s", tempo);
    ssd1306_draw_string(&display, buffer, 10, 10);

    // Formata e exibe o BPM
    snprintf(buffer, sizeof(buffer), "BPM: %d", bpm);
    ssd1306_draw_string(&display, buffer, 10, 20);

    // Formata e exibe a força medida
    snprintf(buffer, sizeof(buffer), "FORCA: %d", forca);
    ssd1306_draw_string(&display, buffer, 10, 30);

    // Formata e exibe o número de quedas
    snprintf(buffer, sizeof(buffer), "QUEDAS: %d", quedas);
    ssd1306_draw_string(&display, buffer, 10, 40);

    ssd1306_send_data(&display); // Envia os dados para o display para atualização
}