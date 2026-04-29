#ifndef SENSORES_H
#define SENSORES_H

#include <stdint.h>

typedef struct
{
    uint16_t temp_engine_adc;
    uint16_t temp_air_adc;
    uint16_t throttle_adc;

    float temp_engine;
    float temp_air;

    uint16_t throttle_filtered;
    uint16_t throttle_percent;

} SensorData;

extern SensorData sensors;

void sensors_init(void);
void sensors_update(void);

#endif
