#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "pico/cyw43_arch.h"  // Biblioteca para Wi-Fi
#include "ssd1306.h"
#include "ws2818b.pio.h"

// Definições de hardware
#define BTN_START 22
#define BTN_A 5
#define BTN_B 6
#define MIC_INPUT 28
#define LED_MATRIX_PIN 7
#define BUZZER_PIN 21
#define I2C_PORT i2c1
#define SDA_PIN 14
#define SCL_PIN 15
#define LED_COUNT 25
#define HEART_SENSOR 26
#define FORCE_SENSOR 27

// Estrutura para os LEDs
PIO np_pio;
uint sm;
struct pixel_t {
    uint8_t G, R, B;
};
typedef struct pixel_t npLED_t;
npLED_t leds[LED_COUNT];

// Display OLED
ssd1306_t display;

// Variáveis globais
bool treino_ativo = false;
uint32_t inicio_treino = 0;
uint8_t pontos_a = 0;
uint8_t pontos_b = 0;
uint16_t batimentos = 60;  // BPM inicial (simulado)
uint16_t batimentos_alvo = 60;
uint16_t forca = 0;
uint16_t offset_forca = 0;  // Valor de referência para o joystick parado
int8_t delta_bpm = 0;  // Variação sutil para tornar realista

// Inicializa Wi-Fi
void init_wifi() {
    if (cyw43_arch_init()) {
        printf("Erro ao inicializar Wi-Fi\n");
    }
}

// Função para medir batimentos cardíacos
uint16_t medir_batimentos() {
    adc_select_input(0);
    return adc_read();
}

// Função para medir força aplicada (com compensação do offset)
uint16_t medir_forca() {
    adc_select_input(1);
    int leitura_bruta = adc_read();

    // Compensa a leitura subtraindo o offset inicial
    int forca_corrigida = leitura_bruta - offset_forca;

    // Pequena margem para evitar leituras flutuantes perto do zero
    if (forca_corrigida < 50) {
        return 0;
    }
    
    return (uint16_t) forca_corrigida;
}

// Ajusta os batimentos baseando-se na força aplicada
void ajustar_batimentos() {
    static uint16_t forca_anterior = 0;
    int16_t variacao_forca = forca - forca_anterior;

    // Se a força aumentou significativamente, ajusta os BPMs gradualmente
    if (variacao_forca > 1000) {
        batimentos_alvo += 10;
    } else if (variacao_forca < -1000) {
        batimentos_alvo -= 5;  // Reduz levemente quando a força diminui
    }

    // Simulação de variação realista
    if (batimentos < batimentos_alvo) {
        batimentos += 1 + (rand() % 2);  // Aumenta 1 ou 2 BPM
    } else if (batimentos > batimentos_alvo) {
        batimentos -= 1;  // Reduz lentamente
    }

    // Atualiza o valor anterior da força
    forca_anterior = forca;
}

// Feedback sonoro para finalizações
void tocar_alerta() {
    pwm_set_gpio_level(BUZZER_PIN, 50000);
    sleep_ms(500);
    pwm_set_gpio_level(BUZZER_PIN, 0);
}

// Atualiza o display OLED com os dados do treino
void atualizar_oled() {
    ssd1306_fill(&display, 0);
    char buffer[20];
    ssd1306_draw_string(&display, "GRAPPLE TECH", 10, 0);
    snprintf(buffer, sizeof(buffer), "TEMPO: %d s", (to_ms_since_boot(get_absolute_time()) - inicio_treino) / 1000);
    ssd1306_draw_string(&display, buffer, 10, 10);
    snprintf(buffer, sizeof(buffer), "BPM: %d", batimentos);
    ssd1306_draw_string(&display, buffer, 10, 20);
    snprintf(buffer, sizeof(buffer), "FORCA: %d", forca);
    ssd1306_draw_string(&display, buffer, 10, 30);
    ssd1306_send_data(&display);
}

// Configuração inicial
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
    adc_gpio_init(HEART_SENSOR);
    adc_gpio_init(FORCE_SENSOR);
    i2c_init(I2C_PORT, 400000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    ssd1306_init(&display, 128, 64, false, 0x3C, I2C_PORT);
    ssd1306_config(&display);
    atualizar_oled();
    init_wifi();

    // Calibração inicial da força do joystick parado
    printf("Calibrando joystick...\n");
    sleep_ms(500);  // Aguarde para garantir leituras estáveis
    offset_forca = medir_forca();  // Define a referência inicial
    printf("Calibração concluída! Offset: %d\n", offset_forca);
}

// Loop principal
void loop() {
    while (true) {
        if (gpio_get(BTN_START) == 0) {
            treino_ativo = !treino_ativo;
            inicio_treino = to_ms_since_boot(get_absolute_time());
            printf("Treino iniciado!\n");
            sleep_ms(300);
        }

        if (treino_ativo) {
            forca = medir_forca();
            ajustar_batimentos();
            atualizar_oled();
        }

        if (gpio_get(BTN_A) == 0) {
            pontos_a++;
            printf("Ponto para A!\n");
            sleep_ms(300);
        }
        if (gpio_get(BTN_B) == 0) {
            pontos_b++;
            printf("Ponto para B!\n");
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
