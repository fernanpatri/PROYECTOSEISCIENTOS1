#include "sensores/ntcmotor/ntcmotor.h"
#include <math.h>

#define ADC_MAX   4095.0f
#define VREF      3.3f

/* divisor resistivo */
#define R_FIXED   10000.0f

/* parámetros NTC */
#define NTC_BETA  3950.0f
#define NTC_R0    10000.0f
#define NTC_T0    298.15f


static float ntc_resistance_to_temp(float r_ntc)
{
    float temp;

    temp = 1.0f / ((1.0f / NTC_T0) + (1.0f / NTC_BETA) * logf(r_ntc / NTC_R0));

    return temp - 273.15f;
}


float ntc_motor_adc_to_temp(uint16_t adc)
{
    float voltage;
    float r_ntc;

    /* ADC → voltaje */
    voltage = ((float)adc * VREF) / ADC_MAX;

    /* divisor resistivo */
    r_ntc = R_FIXED * voltage / (VREF - voltage);

    return ntc_resistance_to_temp(r_ntc);
}
