#include <stdio.h>
#include "pico/stdlib.h"
#include "drivers/BH1750/bh1750.h"
#include "drivers/SERVO_MOTOR/servo_motor.h"
#include "u8g2pico.h"

// Configuração do display OLED
#define I2C1_PORT i2c1
#define SDA_PIN 14
#define SCL_PIN 15
#define I2C_ADDR 0x3C

static u8g2pico_t u8g2pico;

// Variáveis globais para controle do servo
static uint current_angle = 90; // Ângulo atual do servo (inicia no meio)
static uint target_angle = 90;  // Ângulo alvo
static uint step_delay = 100;   // Delay entre os movimentos (velocidade)

// Função para mapear lux para ângulo do servo
static uint map_lux_to_angle(float lux) {
    if (lux < 0) lux = 0;
    if (lux > 1000) lux = 1000;
    return (uint)(lux * 180.0 / 1000.0);
}

// Função para mapear lux para velocidade (delay entre movimentos)
static uint map_lux_to_speed(float lux) {
    if (lux < 0) lux = 0;
    if (lux > 1000) lux = 1000;
    
    // Lux alto = movimento rápido (delay pequeno)
    // Lux baixo = movimento lento (delay grande)
    // Mapeia de 0-1000 lux para 200-20 ms de delay
    return (uint)(200 - (lux * 180.0 / 1000.0));
}

// Função para mover o servo suavemente para o ângulo alvo
void move_servo_smoothly(void) {
    if (current_angle == target_angle) {
        return; // Já está na posição desejada
    }
    
    // Move um passo em direção ao ângulo alvo
    if (current_angle < target_angle) {
        current_angle++;
    } else {
        current_angle--;
    }
    
    // Aplica o ângulo atual no servo
    servo_set_angle(current_angle);
    
    // Aguarda o tempo calculado baseado na luminosidade
    sleep_ms(step_delay);
}

// Função para inicializar o display OLED
void display_init(void) {
    
    // Configurar display
    u8g2_Setup_ssd1306_i2c_128x64_noname_f_pico(&u8g2pico, I2C1_PORT, SDA_PIN, SCL_PIN, U8G2_R0, I2C_ADDR);
    
    // Inicializar
    u8g2_InitDisplay(&u8g2pico);
    u8g2_SetPowerSave(&u8g2pico, 0);
    u8g2_ClearBuffer(&u8g2pico);
    
    printf("[DISPLAY] OLED inicializado com sucesso.\n");
}

// Função para imprimir informações no terminal (igual ao display)
void print_terminal_display(float lux, uint angle, uint speed_delay) {
    printf("=========================================\n");
    printf("         SISTEMA DE CONTROLE LUX        \n");
    printf("=========================================\n");
    printf("Luminosidade: %7.1f lux\n", lux);
    printf("Angulo Servo: %7u graus\n", angle);
    printf("Velocidade  \nInverção:   %7u ms/passo\n", speed_delay);
    printf("-----------------------------------------\n");
    
    // Barra de progresso visual para luminosidade
    int bar_width = (int)((lux / 1000.0) * 40);
    if (bar_width > 40) bar_width = 40;
    
    printf("Taxa    de   Lúmens: [");
    for (int i = 0; i < 40; i++) {
        if (i < bar_width) {
            printf("=");
        } else {
            printf(" ");
        }
    }
    printf("] %d%%\n", (int)(lux / 10.0));
    
    // Barra de progresso visual para velocidade
    int speed_indicator = (int)((200 - speed_delay) * 40 / 180);
    printf("Inversão de Sentido: [");
    for (int i = 0; i < 40; i++) {
        if (i < speed_indicator) {
            printf(">");
        } else {
            printf(" ");
        }
    }
    printf("] %d%%\n", (int)((200 - speed_delay) * 100 / 180));
    
    printf("=========================================\n");
    printf("Mais luz = Servo para sentido horário | Menos luz = Servo para sentido anti-horário\n");
}

// Função para atualizar o display OLED com os valores atuais
void display_update(float lux, uint angle, uint speed_delay) {
    char lux_str[16];
    char angle_str[16];
    char speed_str[16];
    
    // Limpar buffer
    u8g2_ClearBuffer(&u8g2pico);
    
    // Configurar fonte
    u8g2_SetFont(&u8g2pico, u8g2_font_8bitclassic_tf);
    
    // Título
    u8g2_DrawStr(&u8g2pico, 0, 12, "Controle LUX");
    
    // Linha separadora
    u8g2_DrawHLine(&u8g2pico, 0, 15, 128);
    
    // Valor de luminosidade
    snprintf(lux_str, sizeof(lux_str), "LUX: %.1f", lux);
    u8g2_DrawStr(&u8g2pico, 0, 25, lux_str);
    
    // Valor do ângulo do servo
    snprintf(angle_str, sizeof(angle_str), "Angulo: %u", angle);
    u8g2_DrawStr(&u8g2pico, 0, 38, angle_str);
    
    // Velocidade do servo
    snprintf(speed_str, sizeof(speed_str), "Veloc: %ums", speed_delay);
    u8g2_DrawStr(&u8g2pico, 0, 51, speed_str);
    
    // Barra de progresso visual para luminosidade
    uint8_t bar_width = (uint8_t)((lux / 1000.0) * 120);
    if (bar_width > 120) bar_width = 120;
    u8g2_DrawBox(&u8g2pico, 4, 55, bar_width, 6);
    
    // Indicador de velocidade
    uint8_t speed_indicator = (uint8_t)((200 - speed_delay) * 120 / 180);
    u8g2_DrawFrame(&u8g2pico, 4, 62, 120, 4);
    u8g2_DrawBox(&u8g2pico, 4, 62, speed_indicator, 4);
    
    // Enviar buffer para o display
    u8g2_SendBuffer(&u8g2pico);
}

int main(void) {
    stdio_init_all();
    sleep_ms(1000);
    
    // Inicializa o display OLED
    display_init();
    
    // Inicializa o sensor BH1750 (I2C0)
    i2c_inst_t *i2c = bh1750_init(i2c0);
    sleep_ms(200);
    
    // Inicializa o servo motor
    servo_init();
    
    float lux;
    uint new_target_angle;
    bool first_run = true;

    // Tela de boas-vindas no OLED
    u8g2_ClearBuffer(&u8g2pico);
    u8g2_SetFont(&u8g2pico, u8g2_font_8bitclassic_tf);
    u8g2_DrawStr(&u8g2pico, 0, 25, "Sistema Ativo");
    u8g2_DrawStr(&u8g2pico, 0, 40, "Monitorando...");
    u8g2_SendBuffer(&u8g2pico);
        
    sleep_ms(2000);

    while (true) {
        // Lê a luminosidade
        bh1750_read_lux(i2c, &lux);

        // Calcula o novo ângulo alvo baseado na luminosidade
        new_target_angle = map_lux_to_angle(lux);
        
        // Calcula a velocidade baseada na luminosidade
        step_delay = map_lux_to_speed(lux);
        
        // Se o ângulo alvo mudou ou é a primeira execução
        if (new_target_angle != target_angle || first_run) {
            target_angle = new_target_angle;
            
            // Atualiza o display OLED
            display_update(lux, current_angle, step_delay);
            
            // Imprime as mesmas informações no terminal
            print_terminal_display(lux, current_angle, step_delay);
            
            first_run = false;
        }
        
        // Move o servo suavemente em direção ao ângulo alvo
        move_servo_smoothly();
        
        // Pequeno delay para evitar leituras muito rápidas
        sleep_ms(50);
    }
}