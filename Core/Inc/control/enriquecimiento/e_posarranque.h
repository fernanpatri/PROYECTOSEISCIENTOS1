#ifndef E_POSARRANQUE_H
#define E_POSARRANQUE_H

#include <stdint.h>

/* porcentaje visible en debugger */
extern volatile uint16_t posarranque_enrich_percent;

/* iniciar enriquecimiento después del arranque */
void posarranque_begin(void);

/* aplicar enriquecimiento */
uint16_t enrichment_posarranque_apply(uint16_t base_pw);

/* actualizar contador */
/* llamada periódica para reducir duración del posarranque */
void posarranque_update(void);
uint8_t posarranque_activo(void);

#endif
