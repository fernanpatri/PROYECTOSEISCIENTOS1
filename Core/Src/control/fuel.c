#include "control/fuel.h"
#include "control/interpolation.h"
#include "control/enriquecimiento/e_arranque.h"
#include "control/fuel_table.h"
#include "sensores/sensores.h"
#include "control/enriquecimiento/e_frioralenty.h"
#include "control/enriquecimiento/e_posarranque.h"


/* variable global visible para el debugger */
extern volatile uint16_t tiempo_base_pw;
volatile uint16_t tiemp_base_pw = 0;
volatile uint16_t tiempo_final_pw = 0;


/* =========================
   ESTADOS DEL MOTOR
   ========================= */

typedef enum
{
    ENGINE_OFF = 0,
    ENGINE_CRANKING,
    ENGINE_RUNNING
} engine_state_t;

static engine_state_t engine_state = ENGINE_OFF;
static engine_state_t prev_state   = ENGINE_OFF;


/* =========================
   CONFIGURACIÓN
   ========================= */

#define CRANK_RPM_THRESHOLD   300
#define RUN_RPM_THRESHOLD     650
#define STABLE_TIME           40   // ciclos (ajusta según loop)


/* =========================
   VARIABLES INTERNAS
   ========================= */

static uint16_t rpm_filtered = 0;
static uint16_t state_timer  = 0;


/* =========================
   FUNCIÓN PRINCIPAL
   ========================= */

uint16_t fuel_calculate_pw(uint16_t rpm, uint8_t tps)
{
    uint16_t base_pw;
    uint16_t final_pw;

    /* =========================
       FILTRO RPM
       ========================= */

    rpm_filtered = (rpm_filtered * 3 + rpm) / 4;


    /* =========================
       DETECCIÓN DE ESTADO ESTABLE
       ========================= */

    if (rpm_filtered < CRANK_RPM_THRESHOLD)
    {
        if (engine_state != ENGINE_CRANKING)
        {
            state_timer++;

            if (state_timer > STABLE_TIME)
            {
                engine_state = ENGINE_CRANKING;
                state_timer = 0;
            }
        }
        else
        {
            state_timer = 0;
        }
    }
    else if (rpm_filtered > RUN_RPM_THRESHOLD)
    {
        if (engine_state != ENGINE_RUNNING)
        {
            state_timer++;

            if (state_timer > STABLE_TIME)
            {
                engine_state = ENGINE_RUNNING;
                state_timer = 0;
            }
        }
        else
        {
            state_timer = 0;
        }
    }


    /* =========================
       DETECTAR ARRANQUE REAL
       ========================= */

    if (engine_state == ENGINE_RUNNING && prev_state == ENGINE_CRANKING)
    {
    	if (!posarranque_activo())
        {
            posarranque_begin();   // SOLO UNA VEZ
        }
    }

    prev_state = engine_state;


    /* =========================
       COMBUSTIBLE BASE
       ========================= */

    base_pw = interpolate_2d(rpm_filtered, tps);
    tiemp_base_pw = base_pw;


    /* =========================
       ENRIQUECIMIENTOS
       ========================= */

    /* CRANKING */
    if (engine_state == ENGINE_CRANKING)
    {
        base_pw = enrichment_cranking_apply(base_pw, sensors.temp_engine);
    }
    else
    {
        cranking_enrich_percent = 100;
    }


    /* AFTERSTART (usa tu módulo) */
    base_pw = enrichment_posarranque_apply(base_pw);


    /* WARMUP */
    base_pw = enrichment_frioralenty_apply(base_pw, sensors.temp_engine);


    /* =========================
       PROTECCIONES (muy importante)
       ========================= */

    if (base_pw < 100)
        base_pw = 100;

    if (base_pw > 6000)
        base_pw = 6000;


    /* =========================
       RESULTADO FINAL
       ========================= */

    final_pw = base_pw;
    tiempo_final_pw = final_pw;

    return final_pw;
}
