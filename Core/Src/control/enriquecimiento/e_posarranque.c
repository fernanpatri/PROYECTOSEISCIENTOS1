#include "control/enriquecimiento/e_posarranque.h"

/* variable debug */
volatile uint16_t posarranque_enrich_percent = 100;

/* contador */
volatile uint16_t posarranque_counter = 0;

/* configuración */
#define POSARRANQUE_DURATION 200   // duración total
#define POSARRANQUE_MAX      120   // % inicial



uint8_t posarranque_activo(void)
{
    return (posarranque_counter > 0);
}
/* iniciar afterstart (llamar SOLO una vez al arrancar) */
void posarranque_begin(void)
{
    posarranque_counter = POSARRANQUE_DURATION;
}


/* aplicar enriquecimiento */
uint16_t enrichment_posarranque_apply(uint16_t base_pw)
{
    if (posarranque_counter > 0)
    {
        /* decaimiento lineal */
        uint16_t extra = (posarranque_counter * (POSARRANQUE_MAX - 100)) / POSARRANQUE_DURATION;

        posarranque_enrich_percent = 100 + extra;

        base_pw = (base_pw * posarranque_enrich_percent) / 100;
    }
    else
    {
        posarranque_enrich_percent = 100;
    }

    return base_pw;
}


/* actualizar contador (llamar SIEMPRE en loop) */
void posarranque_update(void)
{
    if (posarranque_counter > 0)
    {
        posarranque_counter--;
    }
}
