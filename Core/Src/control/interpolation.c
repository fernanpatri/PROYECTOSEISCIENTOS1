#include "control/interpolation.h"
#include "control/fuel_table.h"

/* buscar índice inferior de RPM */
static uint8_t find_rpm_index(uint16_t rpm)
{
    for(uint8_t i = 0; i < RPM_POINTS - 1; i++)
    {
        if(rpm < rpm_bins[i + 1])
            return i;
    }

    return RPM_POINTS - 2;
}


/* buscar índice inferior de TPS */
static uint8_t find_tps_index(uint8_t tps)
{
    for(uint8_t i = 0; i < TPS_POINTS - 1; i++)
    {
        if(tps < tps_bins[i + 1])
            return i;
    }

    return TPS_POINTS - 2;
}


/* interpolación bilineal */
uint16_t interpolate_2d(uint16_t rpm, uint8_t tps)
{
    uint8_t r;
    uint8_t t;

    uint16_t A;
    uint16_t B;
    uint16_t C;
    uint16_t D;

    float rpm_frac;
    float tps_frac;

    float top;
    float bottom;
    float result;

    /* encontrar celda de la tabla */
    r = find_rpm_index(rpm);
    t = find_tps_index(tps);

    /* cuatro puntos de la celda */
    A = fuel_table[r][t];
    B = fuel_table[r][t + 1];
    C = fuel_table[r + 1][t];
    D = fuel_table[r + 1][t + 1];

    /* posición relativa en RPM */
    rpm_frac =
        (float)(rpm - rpm_bins[r]) /
        (rpm_bins[r + 1] - rpm_bins[r]);

    /* posición relativa en TPS */
    tps_frac =
        (float)(tps - tps_bins[t]) /
        (tps_bins[t + 1] - tps_bins[t]);

    /* interpolación horizontal */
    top = A + (B - A) * tps_frac;
    bottom = C + (D - C) * tps_frac;

    /* interpolación vertical */
    result = top + (bottom - top) * rpm_frac;

    return (uint16_t)result;
}
