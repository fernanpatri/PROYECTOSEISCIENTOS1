#ifndef E_POSARRANQUE_H
#define E_POSARRANQUE_H

#include <stdint.h>

/* debug */
extern volatile uint16_t posarranque_enrich_percent;

/* update con contexto (CLAVE) */
void posarranque_update(uint16_t rpm, uint32_t now_ms);

/* aplicar enriquecimiento */
uint16_t enrichment_posarranque_apply(uint16_t base_pw);

/* estado */
uint8_t posarranque_activo(void);

#endif
