#include "sensores/calibracion/calibracion.h"

CalibrationData calibration =
{
    .throttle_min = 0,
    .throttle_max = 4095,

    .temp_engine_min = 100,
    .temp_engine_max = 4095,

    .temp_air_min = 1700,
    .temp_air_max = 3500
};
