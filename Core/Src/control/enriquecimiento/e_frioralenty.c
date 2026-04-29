#include "control/enriquecimiento/e_frioralenty.h"
#include "control/interpolation_1d.h"

/*
Enriquecimiento de mezcla durante el calentamiento del motor
(warmup enrichment / cold idle enrichment)
*/

/* variable para ver en debugger */
volatile uint16_t frioralenty_enrich_percent = 100;


/* eje de temperatura del motor (°C) */
static const int16_t frioralenty_temp_axis[] =
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


/* número de puntos */
#define FRIORALENTY_POINTS (sizeof(frioralenty_temp_axis) / sizeof(frioralenty_temp_axis[0]))


/* comprobación en compilación */
_Static_assert(
    FRIORALENTY_POINTS == (sizeof(frioralenty_enrichment_table) / sizeof(frioralenty_enrichment_table[0])),
    "ERROR: frioralenty axis y tabla deben tener el mismo tamaño"
);


/* ========================= */
/* 🔥 FUNCIÓN PRINCIPAL      */
/* ========================= */

uint16_t enrichment_frioralenty_get(int16_t temp)
{
    return interp1d(
        temp,
        frioralenty_temp_axis,
        frioralenty_enrichment_table,
        FRIORALENTY_POINTS
    );
}


/* ========================= */
/* 🔧 APLICACIÓN             */
/* ========================= */

uint16_t enrichment_frioralenty_apply(uint16_t base_pw, int16_t temp)
{
    frioralenty_enrich_percent = enrichment_frioralenty_get(temp);

    return (base_pw * frioralenty_enrich_percent) / 100;
}
