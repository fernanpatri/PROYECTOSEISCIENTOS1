#include "control/enriquecimiento/e_arranque.h"
#include "control/interpolation_1d.h"

/*
Enriquecimiento de arranque en frío
basado en temperatura del motor
*/

/* variable global para ver en debugger */
volatile uint16_t cranking_enrich_percent = 100;


/* eje de temperaturas (°C) */
static const int16_t cranking_temp_axis[] =
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
static const uint16_t cranking_enrichment_table[] =
{
    300,  // -10°C
    280,  // -5°C
    260,  // 0°C
    240,  // 5°C
    220,  // 10°C
    205,  // 15°C
    190,  // 20°C
    160,  // 30°C
    140,  // 40°C
    130,  // 50°C
    120,  // 60°C
    110,  // 70°C
    105,  // 80°C
    100   // 95°C
};


/* número de puntos */
#define CRANKING_POINTS (sizeof(cranking_temp_axis) / sizeof(cranking_temp_axis[0]))


/* comprobación en compilación */
_Static_assert(
    CRANKING_POINTS == (sizeof(cranking_enrichment_table) / sizeof(cranking_enrichment_table[0])),
    "ERROR: temp_axis y enrichment_table deben tener el mismo tamaño"
);


/* ========================= */
/* 🔥 FUNCIÓN PRINCIPAL      */
/* ========================= */

uint16_t enrichment_cranking_get(int16_t temp)
{
    return interp1d(
        temp,
        cranking_temp_axis,
        cranking_enrichment_table,
        CRANKING_POINTS
    );
}


/* ========================= */
/* 🔧 APLICACIÓN             */
/* ========================= */

uint16_t enrichment_cranking_apply(uint16_t base_pw, int16_t temp)
{
    cranking_enrich_percent = enrichment_cranking_get(temp);

    return (base_pw * cranking_enrich_percent) / 100;
}
