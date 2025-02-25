#include "buzzer.h"      // Inclui o cabeçalho personalizado para funções relacionadas ao buzzer
#include "config.h"       // Inclui o cabeçalho com configurações específicas do projeto, como pinos
#include "pico/stdlib.h" // Inclui a biblioteca padrão do SDK do Raspberry Pi Pico
#include "hardware/pwm.h" // Inclui a biblioteca para controle de modulação por largura de pulso (PWM)
#include "hardware/clocks.h" // Inclui a biblioteca para obter informações sobre os clocks do sistema

// Variáveis globais para controle do buzzer e da melodia
uint slice_num_buzzer; // Número da fatia PWM usada para controlar o buzzer
bool melodia_tocando = false; // Indica se uma melodia está sendo tocada
uint32_t ultima_nota = 0; // Armazena o tempo em milissegundos da última nota tocada
uint8_t nota_atual = 0; // Índice da nota atual na melodia

// Definição da melodia de alerta
const Nota melodia_alerta[] = {
    {NOTE_C5, 150}, // Nota C5 com duração de 150 milissegundos
    {NOTE_E5, 150}, // Nota E5 com duração de 150 milissegundos
    {NOTE_G5, 150}, // Nota G5 com duração de 150 milissegundos
    {NOTE_C6, 300}, // Nota C6 com duração de 300 milissegundos
};
#define NUM_NOTAS (sizeof(melodia_alerta) / sizeof(Nota)) // Calcula o número de notas na melodia

// Função para configurar o pino do buzzer como saída PWM
void configurar_buzzer(void) {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM); // Define o pino do buzzer como função PWM
    slice_num_buzzer = pwm_gpio_to_slice_num(BUZZER_PIN); // Obtém o número da fatia PWM associada ao pino
    pwm_config config = pwm_get_default_config(); // Obtém a configuração padrão para PWM
    pwm_config_set_clkdiv(&config, 4.0f); // Define o divisor de clock para 4.0 (ajusta a frequência do PWM)
    pwm_init(slice_num_buzzer, &config, true); // Inicializa a fatia PWM com a configuração definida
    pwm_set_gpio_level(slice_num_buzzer, 0); // Define o nível inicial do PWM para 0 (desligado)
}

// Função para definir a frequência do buzzer
void definir_frequencia_buzzer(uint16_t freq) {
    if (freq == 0) { // Se a frequência for 0, desliga o buzzer
        pwm_set_gpio_level(slice_num_buzzer, 0);
        return;
    }
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN); // Obtém o número da fatia PWM
    uint32_t clock_freq = clock_get_hz(clk_sys); // Obtém a frequência do clock do sistema
    uint32_t top = clock_freq / freq - 1; // Calcula o valor máximo do contador PWM para a frequência desejada
    pwm_set_wrap(slice_num, top); // Define o valor máximo do contador PWM
    pwm_set_gpio_level(BUZZER_PIN, top / 2); // Define o nível do PWM para 50% do valor máximo (duty cycle de 50%)
}

// Função para desligar o buzzer e parar a melodia
void desligar_buzzer(void) {
    definir_frequencia_buzzer(0); // Define a frequência para 0 (desliga o buzzer)
    melodia_tocando = false; // Indica que a melodia não está mais tocando
    nota_atual = 0; // Reinicia o índice da nota atual
}

// Função para iniciar a reprodução da melodia de alerta
void tocar_alerta(void) {
    if (!melodia_tocando) { // Se a melodia não estiver tocando
        melodia_tocando = true; // Indica que a melodia está tocando
        nota_atual = 0; // Reinicia o índice da nota atual
        ultima_nota = to_ms_since_boot(get_absolute_time()); // Armazena o tempo atual como o tempo da última nota
        definir_frequencia_buzzer(melodia_alerta[0].freq); // Define a frequência para a primeira nota da melodia
    }
}

// Função para atualizar a melodia (trocar de nota)
void atualizar_melodia(void) {
    if (!melodia_tocando) return; // Se a melodia não estiver tocando, não faz nada
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time()); // Obtém o tempo atual
    if (tempo_atual - ultima_nota >= melodia_alerta[nota_atual].duracao) { // Se o tempo desde a última nota for maior ou igual à duração da nota atual
        nota_atual++; // Avança para a próxima nota
        if (nota_atual >= NUM_NOTAS) { // Se chegou ao final da melodia
            nota_atual = 0; // Reinicia o índice da nota
        }
        definir_frequencia_buzzer(melodia_alerta[nota_atual].freq); // Define a frequência para a próxima nota
        ultima_nota = tempo_atual; // Atualiza o tempo da última nota
    }
}