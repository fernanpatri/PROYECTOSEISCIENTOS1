#ifndef ECU_H
#define ECU_H

#include <stdint.h>

extern volatile uint8_t ecu_tick;

void ecu_init(void);
void ecu_loop(void);

#endif
