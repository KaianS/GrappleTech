#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>     // Inclui tipos de dados de largura fixa (uint16_t, etc.)
#include "pico/stdlib.h" // Inclui a biblioteca padrão do SDK do Raspberry Pi Pico

/**
 * @brief Estrutura para representar uma nota musical.
 *
 * Esta estrutura define uma nota musical com sua frequência e duração.
 */
typedef struct {
    uint16_t freq;      /**< Frequência da nota em Hertz (Hz). */
    uint16_t duracao;   /**< Duração da nota em milissegundos (ms). */
} Nota;

/**
 * @brief Configura o pino do buzzer para saída PWM.
 *
 * Inicializa o pino do buzzer como uma saída de modulação por largura de pulso (PWM)
 * para gerar tons musicais.
 */
void configurar_buzzer(void);

/**
 * @brief Define a frequência do buzzer.
 *
 * Ajusta a frequência do sinal PWM para controlar a altura do som emitido pelo buzzer.
 *
 * @param freq A frequência desejada em Hertz (Hz). Se freq for 0, o buzzer é desligado.
 */
void definir_frequencia_buzzer(uint16_t freq);

/**
 * @brief Desliga o buzzer e para a reprodução da alerta.
 *
 * Interrompe a emissão de som pelo buzzer e reinicia o estado da alerta.
 */
void desligar_buzzer(void);

/**
 * @brief Inicia a reprodução da alerta de alerta.
 *
 * Começa a tocar a sequência de notas definida para o alerta.
 */
void tocar_alerta(void);

/**
 * @brief Atualiza a alerta, avançando para a próxima nota.
 *
 * Controla a progressão da alerta, trocando de nota com base na duração definida.
 */
void atualizar_alerta(void);

/**
 * @brief Indica se uma alerta está sendo tocada.
 *
 * Esta variável externa indica se o buzzer está atualmente tocando uma alerta.
 *
 * @note Deve ser definida no arquivo .c correspondente.
 */
extern bool alerta_tocando;

#endif // BUZZER_H