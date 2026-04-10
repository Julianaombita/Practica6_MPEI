/* ============================================================
 * Práctica 6 - Ejercicios 1, 2 y 3 (VERSIÓN OPTIMIZADA)
 * ADC Multicanal: Potenciómetro, LDR, LM35
 * ============================================================ */

#include "main.h"
#include "stdio.h"
#include "string.h"

/* -- Handle ADC -- */
extern ADC_HandleTypeDef hadc1;

/* ============================================================
 * CONSTANTES
 * ============================================================ */
#define ADC_MAX_VALUE   4095.0f
#define VREF            3.3f

/* ============================================================
 * FUNCIONES AUXILIARES
 * ============================================================ */

/* Lee un valor del ADC (asume scan activo) */
static uint16_t ADC_LeerValor(void)
{
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    return HAL_ADC_GetValue(&hadc1);
}

/* ============================================================
 * ESTRUCTURA DE DATOS
 * ============================================================ */
typedef struct {
    uint16_t raw_pot;
    uint16_t raw_ldr;
    uint16_t raw_temp;

    float voltaje_pot;
    float voltaje_ldr;
    float voltaje_temp;

    float temperatura;
    float iluminacion;
    float posicion_pot;
} SensoresData_t;

/* ============================================================
 * EJERCICIO 1 — Lectura ADC multicanal
 * ============================================================ */
void Ejercicio1_LeerADC(uint16_t *pot,
                        uint16_t *ldr,
                        uint16_t *temp)
{
    HAL_ADC_Start(&hadc1);

    *pot  = ADC_LeerValor();
    *ldr  = ADC_LeerValor();
    *temp = ADC_LeerValor();

    HAL_ADC_Stop(&hadc1);
}

/* ============================================================
 * EJERCICIO 2 — Conversión a magnitudes físicas
 * ============================================================ */
void Ejercicio2_ConvertirMagnitudes(SensoresData_t *d,
                                    uint16_t raw_pot,
                                    uint16_t raw_ldr,
                                    uint16_t raw_temp)
{
    d->raw_pot  = raw_pot;
    d->raw_ldr  = raw_ldr;
    d->raw_temp = raw_temp;

    /* Voltajes */
    d->voltaje_pot  = (raw_pot  / ADC_MAX_VALUE) * VREF;
    d->voltaje_ldr  = (raw_ldr  / ADC_MAX_VALUE) * VREF;
    d->voltaje_temp = (raw_temp / ADC_MAX_VALUE) * VREF;

    /* Conversión física */
    d->temperatura    = d->voltaje_temp * 100.0f;
    d->iluminacion    = (raw_ldr  / ADC_MAX_VALUE) * 100.0f;
    d->posicion_pot   = (raw_pot  / ADC_MAX_VALUE) * 100.0f;
}

/* ============================================================
 * EJERCICIO 3 — LCD
 * ============================================================ */
extern void LCD_SetCursor(uint8_t col, uint8_t row);
extern void LCD_Print(char *str);

void Ejercicio3_MostrarLCD(SensoresData_t *d)
{
    char linea1[17];
    char linea2[17];

    snprintf(linea1, sizeof(linea1),
             "T:%4.1fC L:%3.0f%%",
             d->temperatura,
             d->iluminacion);

    snprintf(linea2, sizeof(linea2),
             "Pot:%3.0f%% V:%4.2fV",
             d->posicion_pot,
             d->voltaje_pot);

    LCD_SetCursor(0, 0);
    LCD_Print(linea1);

    LCD_SetCursor(0, 1);
    LCD_Print(linea2);
}

/* ============================================================
 * LOOP PRINCIPAL INTEGRADO
 * ============================================================ */
void Practica6_Loop(void)
{
    uint16_t pot, ldr, temp;
    SensoresData_t sensores = {0};

    /* 1. Lectura ADC */
    Ejercicio1_LeerADC(&pot, &ldr, &temp);

    /* 2. Procesamiento */
    Ejercicio2_ConvertirMagnitudes(&sensores, pot, ldr, temp);

    /* 3. Visualización */
    Ejercicio3_MostrarLCD(&sensores);

    HAL_Delay(500);
}
