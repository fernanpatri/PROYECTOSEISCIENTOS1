#include "main.h"
#include "sensores/sensores.h"
#include "sensores/calibracion/calibracion.h"
#include "sensores/ntcaire/ntcaire.h"
#include "sensores/ntcmotor/ntcmotor.h"

extern ADC_HandleTypeDef hadc1;

/* buffer DMA */
static volatile uint16_t adc_buffer[3];

/* flag de inicialización del filtro */
static uint8_t sensors_initialized = 0;

/* estructura de sensores */
SensorData sensors;


/* inicialización del ADC */

void sensors_init(void)
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);

    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, 3);
}


/* actualización de sensores */

void sensors_update(void)
{
    /* guardar valores ADC RAW */

    sensors.temp_engine_adc = adc_buffer[0];
    sensors.temp_air_adc = adc_buffer[1];
    sensors.throttle_adc = adc_buffer[2];

    /* convertir temperaturas */

    sensors.temp_engine = ntc_motor_adc_to_temp(sensors.temp_engine_adc);
    sensors.temp_air = ntc_air_adc_to_temp(sensors.temp_air_adc);

    /* filtro TPS */

    if(!sensors_initialized)
    {
        sensors.throttle_filtered = sensors.throttle_adc;
        sensors_initialized = 1;
    }
    else
    {
        int32_t diff;

        diff = (int32_t)sensors.throttle_adc -
               (int32_t)sensors.throttle_filtered;

        sensors.throttle_filtered =
            sensors.throttle_filtered + diff / 8;
    }

    /* conversión a porcentaje */

    if(calibration.throttle_max > calibration.throttle_min)
    {
        sensors.throttle_percent =
            100 * (sensors.throttle_filtered - calibration.throttle_min) /
            (calibration.throttle_max - calibration.throttle_min);
    }
    else
    {
        sensors.throttle_percent = 0;
    }

    if(sensors.throttle_percent > 100)
        sensors.throttle_percent = 100;

    if(sensors.throttle_percent < 0)
        sensors.throttle_percent = 0;
}
