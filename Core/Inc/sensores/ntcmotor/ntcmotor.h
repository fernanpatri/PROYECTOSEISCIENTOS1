#ifndef NTCMOTOR_H
#define NTCMOTOR_H

#include <stdint.h>

/* temperatura motor en °C */
float ntc_motor_adc_to_temp(uint16_t adc);

#endif
