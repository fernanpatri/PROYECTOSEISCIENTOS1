#include "sensores/ntcaire/ntcaire.h"
#include <math.h>

#define ADC_MAX   4095.0f
#define ADC_MIN_VALID  1U
#define ADC_MAX_VALID  4094U
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


float ntc_air_adc_to_temp(uint16_t adc)
{
    float voltage;
    float r_ntc;

    if (adc < ADC_MIN_VALID)
        adc = ADC_MIN_VALID;

    if (adc > ADC_MAX_VALID)
        adc = ADC_MAX_VALID;

    /* ADC to voltage */
    voltage = ((float)adc * VREF) / ADC_MAX;

    /* divisor resistivo */
    r_ntc = R_FIXED * voltage / (VREF - voltage);
}
