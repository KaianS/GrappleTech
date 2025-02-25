#include <stdio.h>       // Inclui a biblioteca padrão de entrada/saída
#include <stdlib.h>      // Inclui a biblioteca padrão para funções de propósito geral
#include "pico/stdlib.h" // Inclui a biblioteca padrão do SDK do Raspberry Pi Pico
#include "hardware.h"    // Inclui o cabeçalho personalizado para funções de hardware
#include "display.h"     // Inclui o cabeçalho personalizado para funções de display
#include "leds.h"        // Inclui o cabeçalho personalizado para funções de LEDs
#include "buzzer.h"      // Inclui o cabeçalho personalizado para funções de buzzer
#include "sensores.h"    // Inclui o cabeçalho personalizado para funções de sensores
#include "treino.h"      // Inclui o cabeçalho personalizado para funções de treino
#include "config.h"      // Inclui o cabeçalho com configurações do projeto

// Variáveis globais para controle do treino
bool treino_ativo = false;                      // Indica se o treino está ativo
uint32_t inicio_treino = 0;                     // Armazena o tempo de início do treino
uint8_t pontos_a = 0, pontos_b = 0;             // Pontuação dos jogadores A e B
uint16_t batimentos = 60, batimentos_alvo = 60; // Batimentos cardíacos atuais e alvo
int16_t forca_x = 0, forca_y = 0;               // Valores de força nos eixos X e Y
uint32_t ultimo_tempo_forca_baixa = 0;          // Armazena o tempo da última vez que a força foi baixa
uint32_t ultimo_tempo_queda = 0;                // Armazena o tempo da última queda detectada

/**
 * @brief Ajusta os batimentos cardíacos alvo com base na variação da força.
 *
 * Aumenta ou diminui os batimentos alvo dependendo da variação da força medida.
 * Também diminui os batimentos alvo se a força permanecer baixa por um período.
 */
void ajustar_batimentos()
{
    static int16_t forca_anterior_x = 0, forca_anterior_y = 0; // Armazena os valores de força anteriores
    int16_t variacao_x = abs(forca_x - forca_anterior_x);      // Calcula a variação da força no eixo X
    int16_t variacao_y = abs(forca_y - forca_anterior_y);      // Calcula a variação da força no eixo Y
    int16_t variacao_forca = variacao_x + variacao_y;          // Calcula a variação total da força
    uint16_t total_forca = (abs(forca_x) + abs(forca_y)) / 2;  // Calcula a força total média

    // Ajusta os batimentos alvo com base na variação da força
    if (variacao_forca > 1000)
    {
        batimentos_alvo += 10; // Aumenta os batimentos alvo se a variação for alta
    }
    else if (variacao_forca < -1000)
    {
        batimentos_alvo -= 5; // Diminui os batimentos alvo se a variação for baixa
    }

    // Diminui os batimentos alvo se a força permanecer baixa por um período
    if (total_forca < 200)
    {
        uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
        if (tempo_atual - ultimo_tempo_forca_baixa >= 10000)
        { // Verifica se já passaram 10 segundos
            batimentos_alvo -= 10;
            ultimo_tempo_forca_baixa = tempo_atual;
        }
    }

    // Limita os batimentos alvo a uma faixa razoável
    if (batimentos_alvo < 40)
        batimentos_alvo = 40;
    if (batimentos_alvo > 180)
        batimentos_alvo = 180;

    // Ajusta os batimentos cardíacos atuais para se aproximarem dos batimentos alvo
    if (batimentos < batimentos_alvo)
    {
        batimentos += 1 + (rand() % 2); // Aumenta os batimentos com um pequeno fator aleatório
    }
    else if (batimentos > batimentos_alvo)
    {
        batimentos -= 1; // Diminui os batimentos
    }

    // Atualiza os valores de força anteriores
    forca_anterior_x = forca_x;
    forca_anterior_y = forca_y;
}

/**
 * @brief Função principal do programa.
 *
 * Inicializa o hardware, display, LEDs, buzzer, sensores e dados de treino.
 * Gerencia o estado do treino, atualiza os dados e exibe as informações.
 *
 * @return 0 se o programa for executado com sucesso.
 */
int main()
{
    stdio_init_all();            // Inicializa a entrada/saída padrão
    inicializar_hardware();      // Inicializa os periféricos de hardware
    inicializar_oled();          // Inicializa o display OLED
    init_leds();                 // Inicializa a matriz de LEDs
    configurar_buzzer();         // Inicializa o buzzer
    calibrar_sensores();         // Calibra os sensores de força
    iniciar_dados_treino();      // Inicializa os dados de treino
    atualizar_oled(0, 60, 0, 0); // Exibe os dados iniciais no display

    // Loop principal do programa
    while (true)
    {
        // Verifica se o botão START foi pressionado
        if (gpio_get(BTN_START) == 0)
        {
            if (!treino_ativo)
            { // Se o treino não estiver ativo, inicia o treino
                treino_ativo = true;
                inicio_treino = to_ms_since_boot(get_absolute_time());
                iniciar_dados_treino();
            }
            else
            { // Se o treino estiver ativo, finaliza o treino
                atualizar_dados_treino((to_ms_since_boot(get_absolute_time()) - inicio_treino) / 1000, batimentos, (abs(forca_x) + abs(forca_y)) / 2, pontos_a, pontos_b);
                enviar_dados_treino();

                // Limpa os LEDs e o display OLED
                clear_leds();
                write_leds();
                atualizar_oled(0, 60, 0, 0);

                // Reseta as variáveis globais
                pontos_a = 0;
                pontos_b = 0;
                batimentos = 60;
                treino_ativo = false;
            }
            sleep_ms(300); // Aguarda um curto período para evitar múltiplas detecções do botão
        }

        // Se o treino estiver ativo, executa a lógica do treino
        if (treino_ativo)
        {
            uint16_t nivel_som = ler_microfone();                         // Lê o nível de som do microfone
            uint32_t tempo_atual = to_ms_since_boot(get_absolute_time()); // Obtém o tempo atual

            // Detecta quedas com base no nível de som e no tempo decorrido desde a última queda
            if (nivel_som > SOUND_THRESHOLD && (tempo_atual - ultimo_tempo_queda) > DEBOUNCE_QUEDA)
            {
                dados_treino.quedas++; // Incrementa o número de quedas
                ultimo_tempo_queda = tempo_atual;
                tocar_alerta(); // Toca um alerta sonoro
                // Acende todos os LEDs de vermelho por um curto período
                for (int i = 0; i < LED_COUNT; i++)
                {
                    set_led(i, 255, 0, 0);
                }
                write_leds();
                sleep_ms(100);
                clear_leds();
                write_leds();
            }

            // Lê os valores de força dos sensores
            forca_x = medir_forca_x();
            forca_y = medir_forca_y();
            ajustar_batimentos(); // Ajusta os batimentos cardíacos

            uint16_t total_forca = (abs(forca_x) + abs(forca_y)) / 2; // Calcula a força total média
            // Atualiza os dados de treino e o display OLEDatualizar_dados_treino((to_ms_since_boot(get_absolute_time()) - inicio_treino) / 1000, batimentos, total_forca, pontos_a, pontos_b);
            atualizar_oled((to_ms_since_boot(get_absolute_time()) - inicio_treino) / 1000, batimentos, total_forca, dados_treino.quedas);
            atualizar_matriz_leds(batimentos); // Atualiza a matriz de LEDs com base nos batimentos

            // Verifica se os batimentos atingiram o limite e toca um alerta
            if (batimentos >= BPM_LIMITE)
            {
                tocar_alerta();
            }
            else if (batimentos < BPM_LIMITE - 5)
            {
                desligar_buzzer(); // Desliga o buzzer se os batimentos estiverem abaixo do limite
            }

            // Atualiza a alerta do buzzer, se estiver tocando
            if (alerta_tocando)
            {
                atualizar_alerta();
            }

            // Ajusta o brilho do LED PWM com base na força total
            uint16_t intensidade = total_forca;
            if (intensidade > 4095)
                intensidade = 4095; // Limita a intensidade ao valor máximo do PWM
            set_pwm_duty(LED_PWM_PIN, intensidade);
        }
        else
        {
            desligar_buzzer(); // Desliga o buzzer se o treino não estiver ativo
        }

        // Verifica se o botão A foi pressionado e incrementa a pontuação do jogador A
        if (gpio_get(BTN_A) == 0)
        {
            pontos_a++;
            sleep_ms(300); // Aguarda um curto período para evitar múltiplas detecções do botão
        }

        // Verifica se o botão B foi pressionado e incrementa a pontuação do jogador B
        if (gpio_get(BTN_B) == 0)
        {
            pontos_b++;
            sleep_ms(300); // Aguarda um curto período para evitar múltiplas detecções do botão
        }

        sleep_ms(50); // Aguarda um curto período antes de verificar os botões novamente
    }
    return 0; // Retorna 0 para indicar que o programa foi executado com sucesso
}