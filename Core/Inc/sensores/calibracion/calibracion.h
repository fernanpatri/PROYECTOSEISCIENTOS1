#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <stdint.h>

typedef struct
{
    /* acelerador */

    uint16_t throttle_min;
    uint16_t throttle_max;

    /* temperatura motor */

    uint16_t temp_engine_min;
    uint16_t temp_engine_max;

    /* temperatura aire */

    uint16_t temp_air_min;
    uint16_t temp_air_max;

} CalibrationData;

extern CalibrationData calibration;

#endif
