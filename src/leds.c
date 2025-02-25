#include "leds.h"      // Inclui o cabeçalho personalizado para funções de LEDs
#include "config.h"     // Inclui o cabeçalho com configurações do projeto
#include "pico/stdlib.h" // Inclui a biblioteca padrão do SDK do Raspberry Pi Pico
#include "hardware/pio.h" // Inclui a biblioteca para controle do PIO (Programmable I/O)
#include "ws2818b.pio.h" // Inclui o programa PIO para controlar os LEDs WS2818B

// Variáveis globais para controle dos LEDs
PIO np_pio; // Instância do PIO utilizado para controlar os LEDs
uint sm;     // Número da State Machine do PIO utilizada
struct pixel_t { // Estrutura para representar um pixel RGB
    uint8_t G, R, B; // Componentes de cor: Verde, Vermelho, Azul
};
typedef struct pixel_t npLED_t; // Define npLED_t como um tipo de pixel_t
npLED_t leds[LED_COUNT]; // Array para armazenar os dados dos LEDs

/**
 * @brief Inicializa a matriz de LEDs WS2818B.
 *
 * Configura o PIO, a State Machine e o programa PIO para controlar os LEDs.
 * Limpa e atualiza os LEDs para garantir que estejam desligados inicialmente.
 */
void init_leds(void) {
    uint offset = pio_add_program(pio0, &ws2818b_program); // Adiciona o programa PIO ao PIO0 e obtém o offset
    np_pio = pio0; // Define o PIO utilizado como PIO0
    sm = pio_claim_unused_sm(np_pio, true); // Obtém uma State Machine não utilizada do PIO
    ws2818b_program_init(np_pio, sm, offset, LED_PIN, 800000.f); // Inicializa o programa PIO para controlar os LEDs
    clear_leds(); // Limpa todos os LEDs
    write_leds(); // Atualiza os LEDs com os dados limpos
}

/**
 * @brief Limpa todos os LEDs da matriz.
 *
 * Define todas as componentes RGB de cada LED como zero (desligado).
 */
void clear_leds(void) {
    for (int i = 0; i < LED_COUNT; i++) {
        leds[i].R = leds[i].G = leds[i].B = 0; // Define as componentes RGB como zero
    }
}

/**
 * @brief Define a cor de um LED específico na matriz.
 *
 * Define as componentes RGB de um LED no índice especificado.
 *
 * @param index O índice do LED na matriz.
 * @param r Componente vermelha (0-255).
 * @param g Componente verde (0-255).
 * @param b Componente azul (0-255).
 */
void set_led(int index, uint8_t r, uint8_t g, uint8_t b) {
    if (index < LED_COUNT) { // Verifica se o índice é válido
        leds[index].R = r; // Define a componente vermelha
        leds[index].G = g; // Define a componente verde
        leds[index].B = b; // Define a componente azul
    }
}

/**
 * @brief Atualiza os LEDs com os dados armazenados no array 'leds'.
 *
 * Envia os dados RGB para cada LED usando o PIO e a State Machine configurados.
 */
void write_leds(void) {
    for (int i = 0; i < LED_COUNT; i++) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G); // Envia a componente verde
        pio_sm_put_blocking(np_pio, sm, leds[i].R); // Envia a componente vermelha
        pio_sm_put_blocking(np_pio, sm, leds[i].B); // Envia a componente azul
    }
}

/**
 * @brief Atualiza a matriz de LEDs com base no valor de BPM.
 *
 * Acende um número de linhas de LEDs proporcional ao valor de BPM.
 *
 * @param bpm Batimentos por minuto (BPM).
 */
void atualizar_matriz_leds(uint16_t bpm) {
    clear_leds(); // Limpa todos os LEDs
    int linhas_acesas = 0; // Número de linhas de LEDs a serem acesas

    // Determina o número de linhas a serem acesas com base no BPM
    if (bpm >= 140) linhas_acesas = 5;
    else if (bpm >= 120) linhas_acesas = 4;
    else if (bpm >= 100) linhas_acesas = 3;
    else if (bpm >= 80) linhas_acesas = 2;
    else if (bpm >= 60) linhas_acesas = 1;

    // Acende as linhas de LEDs
    for (int linha = 0; linha < linhas_acesas; linha++) {
        for (int col = 0; col < LED_COLS; col++) {
            int led_index = (LED_ROWS - 1 - linha) * LED_COLS + col; // Calcula o índice do LED
            set_led(led_index, 50, 0, 0); // Define a cor do LED (vermelho)
        }
    }
    write_leds(); // Atualiza os LEDs com os novos dados
}