#include "control/enriquecimiento/e_posarranque.h"

/*==================== VARIABLES ====================*/

volatile uint16_t posarranque_enrich_percent = 100;
static uint16_t posarranque_counter = 0;

/* estado interno */
static uint8_t engine_running = 0;
static uint8_t start_pending = 0;
static uint32_t start_timer = 0;

/*==================== CONFIG ====================*/

#define POSARRANQUE_DURATION     200   // ciclos
#define POSARRANQUE_MAX          120   // % máximo

#define RPM_START_THRESHOLD      600
#define RPM_STOP_THRESHOLD       300

#define START_STABLE_TIME_MS     100   // estabilidad para confirmar arranque

/*==================== ESTADO ====================*/

uint8_t posarranque_activo(void)
{
    return (posarranque_counter > 0);
}

/*==================== UPDATE (CLAVE) ====================*/

void posarranque_update(uint16_t rpm, uint32_t now_ms)
{
    uint8_t running_now = (rpm > RPM_START_THRESHOLD);

    /* detección robusta de arranque */
    if (running_now && !engine_running)
    {
        if (!start_pending)
        {
            start_timer = now_ms;
            start_pending = 1;
        }

        if ((now_ms - start_timer) > START_STABLE_TIME_MS)
        {
            posarranque_counter = POSARRANQUE_DURATION;
            start_pending = 0;
            engine_running = 1;
        }
    }
    else
    {
        start_pending = 0;
    }

    /* detección de parada */
    if (rpm < RPM_STOP_THRESHOLD)
    {
        engine_running = 0;
    }

    /* decrementar contador */
    if (posarranque_counter > 0)
    {
        posarranque_counter--;
    }
}

/*==================== APPLY ====================*/

uint16_t enrichment_posarranque_apply(uint16_t base_pw)
{
    if (posarranque_counter > 0)
    {
        uint16_t extra = (posarranque_counter * (POSARRANQUE_MAX - 100)) / POSARRANQUE_DURATION;

        posarranque_enrich_percent = 100 + extra;

        return (base_pw * posarranque_enrich_percent) / 100;
    }
    else
    {
        posarranque_enrich_percent = 100;
        return base_pw;
    }
}
