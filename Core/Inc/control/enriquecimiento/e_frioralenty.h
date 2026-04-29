#ifndef E_FRIORALENTY_H
#define E_FRIORALENTY_H

#include <stdint.h>

/* porcentaje de enriquecimiento visible en debugger */
extern volatile uint16_t frioralenty_enrich_percent;

/* obtiene porcentaje de enriquecimiento según temperatura */
uint16_t enrichment_frioralenty_get(int16_t temp);

/* aplica enriquecimiento al combustible */
uint16_t enrichment_frioralenty_apply(uint16_t base_pw, int16_t temp);

#endif
