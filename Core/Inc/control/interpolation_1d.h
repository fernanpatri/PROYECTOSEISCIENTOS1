#ifndef INTERPOLATION_1D_H
#define INTERPOLATION_1D_H

#include <stdint.h>

/**
 * Interpolación lineal 1D (sin coma flotante)
 *
 * @param x       Valor de entrada (ej: temperatura)
 * @param axis    Eje X (ordenado ascendente)
 * @param values  Valores asociados al eje
 * @param size    Número de puntos de la tabla
 *
 * @return Valor interpolado
 */
uint16_t interp1d(int16_t x,const int16_t *axis,const uint16_t *values, uint8_t size);

#endif /* INTERPOLATION_1D_H */
