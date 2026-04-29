#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <stdint.h>

/* estructura de mapa 2D */
typedef struct
{
    const uint16_t *x_bins;
    const uint16_t *y_bins;
    const uint16_t *table;

    uint8_t x_size;
    uint8_t y_size;

} map2d_t;


/* interpolación bilineal (sin float) */
uint16_t map2d_get(
    const map2d_t *map,
    uint16_t x,
    uint16_t y
);

#endif
