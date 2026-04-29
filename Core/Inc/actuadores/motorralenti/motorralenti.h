#ifndef MOTORRALENTI_H
#define MOTORRALENTI_H

#include "main.h"
#include <stdint.h>

void motorralenti_init(void);

void motorralenti_abrir(uint16_t pwm);
void motorralenti_cerrar(uint16_t pwm);
void motorralenti_stop(void);

#endif
