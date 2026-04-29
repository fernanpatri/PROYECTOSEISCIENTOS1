#ifndef NTCAIRE_H
#define NTCAIRE_H

#include <stdint.h>

/* temperatura aire en °C */
float ntc_air_adc_to_temp(uint16_t adc);

#endif
