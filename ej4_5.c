/* ============================================================
 * Práctica 6 - Ejercicios 4 y 5 (Variables renombradas)
 * ADC Modo Continuo + DMA Circular
 * ============================================================ */

#include "main.h"
#include <string.h>

/* -- Handles generados por CubeMX -- */
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

/* ============================================================
 * BUFFER DMA
 * ============================================================ */
#define NUM_CANALES_ADC  3

volatile uint16_t buffer_adc_dma[NUM_CANALES_ADC] = {0};

/* Flag de actualización */
volatile uint8_t flag_dma_completo = 0;

/* ============================================================
 * EJERCICIO 4 — ADC modo continuo
 * ============================================================ */
void ADC_ModoContinuo_Iniciar(void)
{
    HAL_ADC_Start(&hadc1);
}

/* Lectura en modo continuo */
void ADC_ModoContinuo_Leer(uint16_t *valor_pot,
                           uint16_t *valor_ldr,
                           uint16_t *valor_temp)
{
    HAL_ADC_PollForConversion(&hadc1, 10);
    *valor_pot = HAL_ADC_GetValue(&hadc1);

    HAL_ADC_PollForConversion(&hadc1, 10);
    *valor_ldr = HAL_ADC_GetValue(&hadc1);

    HAL_ADC_PollForConversion(&hadc1, 10);
    *valor_temp = HAL_ADC_GetValue(&hadc1);
}

/* ============================================================
 * EJERCICIO 5 — ADC + DMA
 * ============================================================ */
void ADC_DMA_Iniciar(void)
{
    HAL_ADC_Start_DMA(&hadc1,
                      (uint32_t*)buffer_adc_dma,
                      NUM_CANALES_ADC);
}

/* ============================================================
 * CALLBACK DMA
 * ============================================================ */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        flag_dma_completo = 1;
    }
}

/* ============================================================
 * LECTURA DEL BUFFER
 * ============================================================ */
void ADC_DMA_Leer(uint16_t *valor_pot,
                  uint16_t *valor_ldr,
                  uint16_t *valor_temp)
{
    __disable_irq();
    *valor_pot  = buffer_adc_dma[0];
    *valor_ldr  = buffer_adc_dma[1];
    *valor_temp = buffer_adc_dma[2];
    __enable_irq();
}

/* ============================================================
 * VERIFICACIÓN DMA
 * ============================================================ */
uint8_t ADC_DMA_EstaActivo(void)
{
    if (flag_dma_completo)
    {
        flag_dma_completo = 0;
        return 1;
    }
    return 0;
}
