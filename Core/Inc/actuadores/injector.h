#ifndef INJECTOR_H
#define INJECTOR_H

#include "main.h"

extern uint16_t tiempo_final_pw;

void injector_open(void);
void injector_close(void);
void injector_schedule_close(uint16_t us);

#endif
