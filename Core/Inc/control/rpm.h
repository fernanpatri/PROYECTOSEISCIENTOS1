#ifndef RPM_H
#define RPM_H

#include <stdint.h>
#include "main.h"   // ✅ necesario para HAL_GetTick

extern volatile uint32_t engine_rpm;

void rpm_init(void);
void rpm_capture_callback(uint32_t capture);
void rpm_timeout_update(void);

#endif
