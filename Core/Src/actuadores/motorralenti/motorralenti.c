#include "actuadores/motorralenti/motorralenti.h"

/*==================== CONFIG ====================*/

extern TIM_HandleTypeDef htim3;
#define PWM_TIMER     &htim3
#define PWM_CHANNEL   TIM_CHANNEL_1

/*==================== HARDWARE ====================*/

static void set_pwm(uint16_t pwm)
{
    if (pwm > 1000) pwm = 1000;
    __HAL_TIM_SET_COMPARE(PWM_TIMER, PWM_CHANNEL, pwm);
}

static void pwm_start(void)
{
    HAL_TIM_PWM_Start(PWM_TIMER, PWM_CHANNEL);
}

static void pwm_stop(void)
{
    HAL_TIM_PWM_Stop(PWM_TIMER, PWM_CHANNEL);
}

/*==================== API ====================*/

void motorralenti_init(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

    pwm_stop();
}

void motorralenti_abrir(uint16_t pwm)
{
    // Dirección abrir
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

    set_pwm(pwm);
    pwm_start();
}


void motorralenti_cerrar(uint16_t pwm)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);

    set_pwm(pwm);
    pwm_start();
}

void motorralenti_stop(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

    pwm_stop();
}
