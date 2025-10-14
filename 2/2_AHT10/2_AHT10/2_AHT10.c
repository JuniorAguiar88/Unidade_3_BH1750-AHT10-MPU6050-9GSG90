#include <stdio.h>
#include "pico/stdlib.h"
#include "aht10/aht10.h"
#include "u8g2pico.h"

// Constantes para os limites de alerta
#define HUMIDITY_THRESHOLD 70.0f
#define TEMPERATURE_THRESHOLD 20.0f

// Configurações do display OLED
#define I2C_PORT i2c1
#define SDA_PIN 14
#define SCL_PIN 15
#define I2C_ADDR 0x3C

static u8g2pico_t u8g2pico;

// Função para inicializar o display
void display_init() {
    u8g2_Setup_ssd1306_i2c_128x64_noname_f_pico(&u8g2pico, I2C_PORT, SDA_PIN, SCL_PIN, U8G2_R0, I2C_ADDR);
    u8g2_InitDisplay(&u8g2pico);
    u8g2_SetPowerSave(&u8g2pico, 0);
}

// Função para limpar o display
void clear_display() {
    u8g2_ClearBuffer(&u8g2pico);
}

// Função para exibir texto no display
void display_text(const char *text, int x, int y, int font_size) {
    // Selecionar fonte baseada no tamanho
    if (font_size == 2) {
        u8g2_SetFont(&u8g2pico, u8g2_font_10x20_tf);
    } else {
        u8g2_SetFont(&u8g2pico, u8g2_font_8x13_tf);
    }
    u8g2_DrawStr(&u8g2pico, x, y, text);
}

// Função para exibir texto sem limpar o buffer
void display_text_no_clear(const char *text, int x, int y, int font_size) {
    // Selecionar fonte baseada no tamanho
    if (font_size == 2) {
        u8g2_SetFont(&u8g2pico, u8g2_font_10x20_tf);
    } else {
        u8g2_SetFont(&u8g2pico, u8g2_font_8x13_tf);
    }
    u8g2_DrawStr(&u8g2pico, x, y, text);
}

// Função para atualizar o display
void show_display() {
    u8g2_SendBuffer(&u8g2pico);
}

// Função para verificar se há condições de alerta
bool check_alert_conditions(float temp, float hum) {
    return (hum > HUMIDITY_THRESHOLD || temp < TEMPERATURE_THRESHOLD);
}

// Função para exibir alerta no display e serial
void display_alert(float temp, float hum) {
    // No Serial Monitor
    printf("ALERTA! ");
    
    // Verificar qual condição foi violada
    if (hum > HUMIDITY_THRESHOLD && temp < TEMPERATURE_THRESHOLD) {
        // Ambas as condições violadas
        printf("Umidade ALTA e Temperatura BAIXA - ");
    } else if (hum > HUMIDITY_THRESHOLD) {
        // Apenas umidade alta
        printf("Umidade ALTA! - ");
    } else if (temp < TEMPERATURE_THRESHOLD) {
        // Apenas temperatura baixa
        printf("Temperatura BAIXA! - ");
    }
    printf("H: %.1f%% | T: %.1fC\n", hum, temp);

    // No Display OLED
    clear_display();
    
    // Título do alerta
    display_text_no_clear("ALERTA!", 30, 15, 2);

    char hum_str[10], temp_str[10];
    sprintf(hum_str, "%.1f%%", hum);
    sprintf(temp_str, "%.1fC", temp);

    // Verificar qual condição foi violada
    if (hum > HUMIDITY_THRESHOLD && temp < TEMPERATURE_THRESHOLD) {
        // Ambas as condições violadas
        display_text_no_clear("Umidade ALTA", 5, 35, 1);
        display_text_no_clear("Temp BAIXA", 5, 50, 1);
    } else if (hum > HUMIDITY_THRESHOLD) {
        // Apenas umidade alta
        display_text_no_clear("Umidade ALTA!", 15, 35, 1);
    } else if (temp < TEMPERATURE_THRESHOLD) {
        // Apenas temperatura baixa
        display_text_no_clear("Temp BAIXA!", 15, 35, 1);
    }

    // Exibir valores
    display_text_no_clear("H:", 5, 60, 1);
    display_text_no_clear(hum_str, 25, 60, 1);
    display_text_no_clear("T:", 70, 60, 1);
    display_text_no_clear(temp_str, 90, 60, 1);
    
    show_display();
}

// Função para exibir informações normais no display e serial
void display_normal_info(float temp, float hum) {
    // No Serial Monitor
    printf("Monitor - Umidade: %.1f%% | Temperatura: %.1fC | Status: OK\n", hum, temp);

    // No Display OLED
    clear_display();
    
    // Título
    display_text_no_clear("Monitor", 25, 15, 2);
    
    // Umidade
    display_text_no_clear("Umidade:", 5, 30, 1);
    char hum_str[10];
    sprintf(hum_str, "%.1f%%", hum);
    display_text_no_clear(hum_str, 70, 30, 1);
    
    // Temperatura
    display_text_no_clear("Temp:", 5, 45, 1);
    char temp_str[10];
    sprintf(temp_str, "%.1fC", temp);
    display_text_no_clear(temp_str, 70, 45, 1);
    
    // Status
    display_text_no_clear("Status: OK", 5, 60, 1);
    
    show_display();
}

int main()
{
    stdio_init_all();
    sleep_ms(1000);

    // Inicializar display
    display_init();
    sleep_ms(500);
    
    // Exibir mensagem de inicialização no display
    clear_display();
    display_text("Iniciando...", 15, 35, 2);
    show_display();

    // Inicializar sensor AHT10
    aht10_i2c_init();
    aht10_init();
    sleep_ms(200);
    
    float temp, hum;
    bool alert_active = false;

    while (true) {
        aht10_trigger_measurement();
        sleep_ms(200);

        // Tenta ler sensor
        if (aht10_read(&temp, &hum)) {
            printf("TEMPERATURA: %.2f°C | UMIDADE: %.2f%%\n", temp, hum);
            
            // Verificar condições de alerta
            if (check_alert_conditions(temp, hum)) {
                if (!alert_active) {
                    printf("ALERTA: Condições críticas detectadas!\n");
                    alert_active = true;
                }
                display_alert(temp, hum);
            } else {
                if (alert_active) {
                    printf("Condições normalizadas.\n");
                    alert_active = false;
                }
                display_normal_info(temp, hum);
            }
        } else {
            printf("Falha ao ler sensor AHT10...\n");
            
            // Exibir erro no display
            clear_display();
            display_text("Erro Sensor", 20, 35, 2);
            show_display();
        }

        sleep_ms(1000); // Atualizar a cada 2 segundos
    }
}