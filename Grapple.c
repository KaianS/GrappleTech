#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "pico/cyw43_arch.h"
#include "ssd1306.h"
#include "ws2818b.pio.h"

// Definições de hardware
#define BTN_START 22
#define BTN_A 5
#define BTN_B 6
#define LED_PWM_PIN 13  // Pino do LED controlado via PWM
#define I2C_PORT i2c1
#define SDA_PIN 14
#define SCL_PIN 15
#define FORCE_SENSOR_X 26  // Sensor de força no eixo X
#define FORCE_SENSOR_Y 27  // Sensor de força no eixo Y

// Definições do buzzer
#define BUZZER_PIN 21
#define BPM_LIMITE 160

// Definições das notas musicais (frequências em Hz)
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988
#define NOTE_C6  1047

// Definições para matriz de LEDs
#define LED_PIN 7
#define LED_COUNT 25
#define LED_ROWS 5
#define LED_COLS 5

// Add UART definitions
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define MIC_PIN 28  // ADC2
#define SOUND_THRESHOLD 2300  // Ajuste este valor conforme a sensibilidade desejada


// Estrutura para armazenar dados do treino
typedef struct {
    uint32_t duracao;
    uint16_t bpm_max;
    uint16_t bpm_min;
    uint16_t bpm_medio;
    uint16_t forca_maxima;
    uint16_t pontuacao_a;
    uint16_t pontuacao_b;
    uint16_t quedas;  // Nova variável para contar quedas
} DadosTreino;

DadosTreino dados_treino = {0};
uint32_t amostras_bpm = 0;
uint32_t soma_bpm = 0;

// Estrutura para definir uma nota musical
typedef struct {
    uint16_t freq;
    uint16_t duracao;
} Nota;

// Melodia de alerta (pode ser modificada para criar diferentes padrões)
const Nota melodia_alerta[] = {
    {NOTE_C5, 150},
    {NOTE_E5, 150},
    {NOTE_G5, 150},
    {NOTE_C6, 300},
};
#define NUM_NOTAS (sizeof(melodia_alerta) / sizeof(Nota))


// Variáveis globais adicionais
uint slice_num_buzzer;
bool melodia_tocando = false;
uint32_t ultima_nota = 0;
uint8_t nota_atual = 0;
uint32_t ultimo_tempo_queda = 0;
const uint32_t DEBOUNCE_QUEDA = 1000; // Tempo mínimo entre quedas (ms)


// Display OLED
ssd1306_t display;

// State Machine para LEDs
PIO np_pio;
uint sm;
struct pixel_t {
    uint8_t G, R, B;
};
typedef struct pixel_t npLED_t;
npLED_t leds[LED_COUNT];

// Variáveis globais
bool treino_ativo = false;
uint32_t inicio_treino = 0;
uint8_t pontos_a = 0, pontos_b = 0;
uint16_t batimentos = 60, batimentos_alvo = 60;
int16_t forca_x = 0, forca_y = 0;
uint16_t offset_x = 0, offset_y = 0;
uint32_t ultimo_tempo_forca_baixa = 0;
bool buzzer_ativo = false;


// Protótipos das funções da matriz de LEDs
void init_leds(void);
void clear_leds(void);
void set_led(int index, uint8_t r, uint8_t g, uint8_t b);
void write_leds(void);
void atualizar_matriz_leds(uint16_t bpm);
// Protótipos das funções
void configurar_buzzer(void);
void definir_frequencia_buzzer(uint16_t freq);
void desligar_buzzer(void);
void tocar_melodia(void);
void atualizar_melodia(void);

void inicializar_uart() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}

// Função para ler o microfone
uint16_t ler_microfone() {
    adc_select_input(2);  // ADC2 corresponde ao GPIO28
    return adc_read();
}

// Função para enviar dados via UART
void enviar_dados_treino() {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), 
        "{"
        "\"duracao\":%lu,"
        "\"bpm_max\":%u,"
        "\"bpm_min\":%u,"
        "\"bpm_medio\":%u,"
        "\"forca_maxima\":%u,"
        "\"pontuacao_a\":%u,"
        "\"pontuacao_b\":%u,"
        "\"quedas\":%u"
        "}\n",
        dados_treino.duracao,
        dados_treino.bpm_max,
        dados_treino.bpm_min,
        dados_treino.bpm_medio,
        dados_treino.forca_maxima,
        dados_treino.pontuacao_a,
        dados_treino.pontuacao_b,
        dados_treino.quedas
    );
    printf("%s", buffer);
    uart_puts(UART_ID, buffer);
    sleep_ms(100);
}

// Funções do buzzer
void configurar_buzzer(void) {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num_buzzer = pwm_gpio_to_slice_num(BUZZER_PIN);
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

void tocar_melodia(void) {
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

// Funções para matriz de LEDs
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

// Nova função para atualizar matriz de LEDs baseado nos batimentos
void atualizar_matriz_leds(uint16_t bpm) {
    clear_leds();
    
    // Calcula quantas linhas devem ser acesas baseado nos batimentos
    int linhas_acesas = 0;
    if (bpm >= 140) linhas_acesas = 5;
    else if (bpm >= 120) linhas_acesas = 4;
    else if (bpm >= 100) linhas_acesas = 3;
    else if (bpm >= 80) linhas_acesas = 2;
    else if (bpm >= 60) linhas_acesas = 1;
    
    // Acende as linhas de baixo para cima
    for (int linha = 0; linha < linhas_acesas; linha++) {
        for (int col = 0; col < LED_COLS; col++) {
            // Calcula o índice do LED na matriz 5x5
            int led_index = (LED_ROWS - 1 - linha) * LED_COLS + col;
            // Acende LED em vermelho
            set_led(led_index, 50, 0, 0);  // R=50, G=0, B=0
        }
    }
    
    write_leds();
}

// Inicializa PWM no pino especificado
void iniciar_pwm(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, 4095); // Resolução de 12 bits
    pwm_set_gpio_level(slice_num, 0);
    pwm_set_enabled(slice_num, true);
}

// Define a intensidade do LED
void set_pwm_duty(uint gpio, uint16_t valor) {
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_gpio_level(gpio, valor);
}

// Funções para medir força
int16_t medir_forca_x() {
    adc_select_input(0);
    return adc_read() - offset_x;
}

int16_t medir_forca_y() {
    adc_select_input(1);
    return adc_read() - offset_y;
}

// Atualiza o display OLED
void atualizar_oled() {
    uint16_t total_forca = (abs(forca_x) + abs(forca_y)) / 2;
    ssd1306_fill(&display, 0);
    char buffer[20];
    ssd1306_draw_string(&display, "GRAPPLE TECH", 10, 0);
    snprintf(buffer, sizeof(buffer), "TEMPO: %d s", (to_ms_since_boot(get_absolute_time()) - inicio_treino) / 1000);
    ssd1306_draw_string(&display, buffer, 10, 10);
    snprintf(buffer, sizeof(buffer), "BPM: %d", batimentos);
    ssd1306_draw_string(&display, buffer, 10, 20);
    snprintf(buffer, sizeof(buffer), "FORCA: %d", total_forca);
    ssd1306_draw_string(&display, buffer, 10, 30);
    snprintf(buffer, sizeof(buffer), "QUEDAS: %d", dados_treino.quedas);
    ssd1306_draw_string(&display, buffer, 10, 40);
    ssd1306_send_data(&display);
}

// Ajusta os batimentos baseando-se na força aplicada
void ajustar_batimentos() {
    static int16_t forca_anterior_x = 0, forca_anterior_y = 0;
    int16_t variacao_x = abs(forca_x - forca_anterior_x);
    int16_t variacao_y = abs(forca_y - forca_anterior_y);
    int16_t variacao_forca = variacao_x + variacao_y;

    if (variacao_forca > 1000) {
        batimentos_alvo += 10;
    } else if (variacao_forca < -1000) {
        batimentos_alvo -= 5;
    }

    if (batimentos < batimentos_alvo) {
        batimentos += 1 + (rand() % 2);
    } else if (batimentos > batimentos_alvo) {
        batimentos -= 1;
    }

    forca_anterior_x = forca_x;
    forca_anterior_y = forca_y;
}

//setup
void setup() {
    stdio_init_all();
    gpio_init(BTN_START);
    gpio_set_dir(BTN_START, GPIO_IN);
    gpio_pull_up(BTN_START);
    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_up(BTN_A);
    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_B);
    adc_init();
    adc_gpio_init(FORCE_SENSOR_X);
    adc_gpio_init(FORCE_SENSOR_Y);
    adc_gpio_init(MIC_PIN);
    i2c_init(I2C_PORT, 400000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    ssd1306_init(&display, 128, 64, false, 0x3C, I2C_PORT);
    ssd1306_config(&display);
    iniciar_pwm(LED_PWM_PIN);
    init_leds();
    configurar_buzzer(); 
    inicializar_uart();
    printf("Calibrando joystick...\n");
    sleep_ms(500);
    offset_x = adc_read();
    sleep_ms(500);
    offset_y = adc_read();
    printf("Calibração concluída! Offsets - X: %d, Y: %d\n", offset_x, offset_y);
    atualizar_oled();
    dados_treino.quedas = 0;

     // Inicializar dados do treino
     dados_treino.bpm_min = 0xFFFF;  // Valor máximo para começar
     dados_treino.bpm_max = 0;
     dados_treino.bpm_medio = 0;
     dados_treino.forca_maxima = 0;
}

void loop() {
    while (true) {
        if (gpio_get(BTN_START) == 0) {
            if (!treino_ativo) {
                // Início do treino
                treino_ativo = true;
                inicio_treino = to_ms_since_boot(get_absolute_time());
                amostras_bpm = 0;
                soma_bpm = 0;
                dados_treino.bpm_min = 0xFFFF;
                dados_treino.bpm_max = 0;
                dados_treino.forca_maxima = 0;
                dados_treino.quedas = 0;  // Reinicia contador de quedas
            } else {
                // Fim do treino
                treino_ativo = false;
                dados_treino.duracao = (to_ms_since_boot(get_absolute_time()) - inicio_treino) / 1000;
                dados_treino.bpm_medio = (amostras_bpm > 0) ? (soma_bpm / amostras_bpm) : 0;
                dados_treino.pontuacao_a = pontos_a;
                dados_treino.pontuacao_b = pontos_b;
                enviar_dados_treino();
            }
            sleep_ms(300);
        }

        if (treino_ativo) {
            // Detectar quedas pelo som
            uint16_t nivel_som = ler_microfone();
            uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());


            if (nivel_som > SOUND_THRESHOLD && (tempo_atual - ultimo_tempo_queda) > DEBOUNCE_QUEDA) {
                dados_treino.quedas++;
                ultimo_tempo_queda = tempo_atual;
                
                
                // Feedback visual e sonoro da queda
                tocar_melodia();
                
                // Piscar todos os LEDs em vermelho
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
            if (total_forca > dados_treino.forca_maxima) {
                dados_treino.forca_maxima = total_forca;
            }
            if (batimentos > dados_treino.bpm_max) {
                dados_treino.bpm_max = batimentos;
            }
            if (batimentos < dados_treino.bpm_min) {
                dados_treino.bpm_min = batimentos;
            }
            soma_bpm += batimentos;
            amostras_bpm++;

            atualizar_oled();
            atualizar_matriz_leds(batimentos);

            // Verifica os batimentos e controla o buzzer
            if (batimentos >= BPM_LIMITE) {
                tocar_melodia();
            } else if (batimentos < BPM_LIMITE - 5) { // Histerese para evitar oscilação
                desligar_buzzer();
            }
            
            if (melodia_tocando) {
                atualizar_melodia();
            }

            uint16_t intensidade = (abs(forca_x) + abs(forca_y)) / 2;
            if (intensidade > 4095) intensidade = 4095;
            set_pwm_duty(LED_PWM_PIN, intensidade);

            if (total_forca < 200) {
                uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
                if (tempo_atual - ultimo_tempo_forca_baixa >= 5000) {
                    batimentos -= 10;
                    if (batimentos < 40) batimentos = 40;
                    if (batimentos > 180) batimentos = 180;
                    ultimo_tempo_forca_baixa = tempo_atual;
                }
            }
        } else {
            desligar_buzzer();  // Desativa o buzzer quando o treino não está ativo
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
}

int main() {
    setup();
    loop();
    return 0;
}
