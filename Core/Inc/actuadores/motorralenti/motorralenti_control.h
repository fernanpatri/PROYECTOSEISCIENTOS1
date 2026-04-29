#ifndef MOTORRALENTI_CONTROL_H
#define MOTORRALENTI_CONTROL_H

#include <stdint.h>

void motorralenti_control_init(void);
void motorralenti_control_update(int16_t temp);

#endif
