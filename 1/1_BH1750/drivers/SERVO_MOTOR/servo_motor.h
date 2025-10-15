#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"

// --- Constantes para o controle do Servo ---
// Frequência do sinal PWM para servos (geralmente 50Hz, ou um período de 20ms)
#define PWM_PERIOD_MS 20

// Define o pino GPIO que está conectado ao fio de sinal do servo
#define SERVO_PIN 2

// Largura do pulso em microssegundos (us) para o SG90
#define SERVO_MIN_PULSE_US 500
#define SERVO_MAX_PULSE_US 2500

// Function prototypes
void servo_init(void);
void servo_set_angle(uint angle);

#endif // SERVO_MOTOR_H