#include "actuadores/injector.h"
#include "control/fuel.h"

extern TIM_HandleTypeDef htim5;

static volatile uint8_t injector_active = 0;

/* límites de seguridad */
#define INJECTOR_MIN_US   500
#define INJECTOR_MAX_US   10000

/*==================== CLOSE ====================*/

void injector_close(void)
{
    HAL_GPIO_WritePin(INYECTOR_GPIO_Port, INYECTOR_Pin, GPIO_PIN_RESET);
    injector_active = 0;
}

/*==================== OPEN ====================*/

void injector_open(void)
{
    if(injector_active) return;

    injector_active = 1;

    HAL_GPIO_WritePin(INYECTOR_GPIO_Port, INYECTOR_Pin, GPIO_PIN_SET);
}

/*==================== SCHEDULE ====================*/

void injector_schedule_close(uint16_t us)
{
    if(!injector_active) return;

    if(us < INJECTOR_MIN_US) us = INJECTOR_MIN_US;
    if(us > INJECTOR_MAX_US) us = INJECTOR_MAX_US;

    /* 🔴 CRÍTICO: deshabilitar IRQ durante configuración */
    __disable_irq();

    HAL_TIM_Base_Stop_IT(&htim5);

    __HAL_TIM_SET_COUNTER(&htim5, 0);
    __HAL_TIM_SET_AUTORELOAD(&htim5, us);

    __HAL_TIM_CLEAR_FLAG(&htim5, TIM_FLAG_UPDATE);

    HAL_TIM_Base_Start_IT(&htim5);

    __enable_irq();
}
