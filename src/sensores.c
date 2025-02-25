#include <stdio.h>      // Inclui a biblioteca padrão de entrada/saída
#include "sensores.h"   // Inclui o cabeçalho personalizado para funções de sensores
#include "config.h"     // Inclui o cabeçalho com configurações do projeto
#include "pico/stdlib.h" // Inclui a biblioteca padrão do SDK do Raspberry Pi Pico
#include "hardware/adc.h" // Inclui a biblioteca para controle do ADC (Conversor Analógico-Digital)

// Variáveis globais para armazenar os offsets dos sensores de força
uint16_t offset_x = 0, offset_y = 0;

/**
 * @brief Calibra os sensores de força nos eixos X e Y.
 *
 * Realiza a leitura dos valores dos sensores em repouso e armazena os offsets
 * para compensar as leituras futuras.
 */
void calibrar_sensores() {
    printf("Calibrando joystick...\n"); // Imprime uma mensagem de início de calibração
    sleep_ms(500); // Aguarda 500 milissegundos
    adc_select_input(0); // Seleciona o canal do ADC para o sensor de força no eixo X
    offset_x = adc_read(); // Lê o valor do sensor e armazena como offset
    sleep_ms(500); // Aguarda 500 milissegundos
    adc_select_input(1); // Seleciona o canal do ADC para o sensor de força no eixo Y
    offset_y = adc_read(); // Lê o valor do sensor e armazena como offset
    printf("Calibração concluída! Offsets - X: %d, Y: %d\n", offset_x, offset_y); // Imprime mensagem de conclusão e os offsets
}

/**
 * @brief Mede a força no eixo X.
 *
 * Seleciona o canal do ADC correspondente ao sensor de força no eixo X,
 * realiza a leitura e subtrai o offset para obter o valor calibrado.
 *
 * @return O valor da força medida no eixo X, compensado pelo offset.
 */
int16_t medir_forca_x() {
    adc_select_input(0); // Seleciona o canal do ADC para o sensor de força no eixo X
    return adc_read() - offset_x; // Retorna a leitura subtraída do offset
}

/**
 * @brief Mede a força no eixo Y.
 *
 * Seleciona o canal do ADC correspondente ao sensor de força no eixo Y,
 * realiza a leitura e subtrai o offset para obter o valor calibrado.
 *
 * @return O valor da força medida no eixo Y, compensado pelo offset.
 */
int16_t medir_forca_y() {
    adc_select_input(1); // Seleciona o canal do ADC para o sensor de força no eixo Y
    return adc_read() - offset_y; // Retorna a leitura subtraída do offset
}

/**
 * @brief Lê o nível de som do microfone.
 *
 * Seleciona o canal do ADC correspondente ao microfone e realiza a leitura.
 *
 * @return O valor lido do microfone.
 */
uint16_t ler_microfone() {
    adc_select_input(2); // Seleciona o canal do ADC para o microfone
    return adc_read(); // Retorna a leitura do microfone
}