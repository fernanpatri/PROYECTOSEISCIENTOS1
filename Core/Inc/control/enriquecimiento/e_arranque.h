#ifndef E_ARRANQUE_H
#define E_ARRANQUE_H

#include <stdint.h>

/* porcentaje de enriquecimiento usado (visible en debugger) */
extern volatile uint16_t cranking_enrich_percent;

/* obtiene porcentaje de enriquecimiento según temperatura */
uint16_t enrichment_cranking_get(int16_t temp);

/* aplica enriquecimiento al tiempo de inyección */
uint16_t enrichment_cranking_apply(uint16_t base_pw, int16_t temp);

#endif
