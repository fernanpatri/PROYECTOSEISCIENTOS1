#include "control/enriquecimiento/e_frioralenty.h"

/*
Enriquecimiento de mezcla durante el calentamiento del motor
(warmup enrichment / cold idle enrichment)
*/

/* variable para ver en debugger */
volatile uint16_t frioralenty_enrich_percent = 100;


/* eje de temperatura del motor */
static const int8_t frioralenty_temp_axis[] =
{
    -10,
    -5,
    0,
    5,
    10,
    15,
    20,
    30,
    40,
    50,
    60,
    70,
    80,
    95
};


/* tabla de enriquecimiento (%) */
static const uint16_t frioralenty_enrichment_table[] =
{
    180,  // -10
    170,  // -5
    160,  // 0
    150,  // 5
    145,  // 10
    138,  // 15
    130,  // 20
    120,  // 30
    110,  // 40
    108,  // 50
    105,  // 60
    103,  // 70
    101,  // 80
    100   // 95
};


/* número de puntos (calculado en compilación) */
#define FRIORALENTY_POINTS (sizeof(frioralenty_temp_axis) / sizeof(frioralenty_temp_axis[0]))


/* comprobación en compilación */
_Static_assert(
    FRIORALENTY_POINTS == (sizeof(frioralenty_enrichment_table) / sizeof(frioralenty_enrichment_table[0])),
    "ERROR: frioralenty axis y tabla deben tener el mismo tamaño"
);


/* obtiene enriquecimiento interpolado */
uint16_t enrichment_frioralenty_get(int16_t temp)
{
    uint8_t i;

    /* límites */
    if (temp <= frioralenty_temp_axis[0])
        return frioralenty_enrichment_table[0];

    if (temp >= frioralenty_temp_axis[FRIORALENTY_POINTS - 1])
        return frioralenty_enrichment_table[FRIORALENTY_POINTS - 1];

    /* buscar intervalo */
    for (i = 0; i < (FRIORALENTY_POINTS - 1); i++)
    {
        if (temp < frioralenty_temp_axis[i + 1])
        {
            int16_t t1 = frioralenty_temp_axis[i];
            int16_t t2 = frioralenty_temp_axis[i + 1];

            uint16_t e1 = frioralenty_enrichment_table[i];
            uint16_t e2 = frioralenty_enrichment_table[i + 1];

            /* interpolación lineal (entera) */
            return e1 + ((temp - t1) * (e2 - e1)) / (t2 - t1);
        }
    }

    return frioralenty_enrichment_table[FRIORALENTY_POINTS - 1];
}


/* aplica enriquecimiento */
uint16_t enrichment_frioralenty_apply(uint16_t base_pw, int16_t temp)
{
    frioralenty_enrich_percent = enrichment_frioralenty_get(temp);

    return (base_pw * frioralenty_enrich_percent) / 100;
}
