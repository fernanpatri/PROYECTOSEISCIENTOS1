	#include "control/fuel_table.h"

const uint16_t rpm_bins[RPM_POINTS] =
{800,1500,2000,2500,3000,3500,4000,4500};

const uint16_t tps_bins[TPS_POINTS] =
{0,10,20,40,60,80,90,100};

const uint16_t fuel_table[RPM_POINTS][TPS_POINTS] =
{
/* TPS →   0     1     2     3     4     5     6     7  */  // Tanto porciento de apertura de acelerador
/* RPM ↓ */
{2800,2900,3000,3200,3400,3600,3800,4000}, // 1000 rpm
{2800,2950,3100,3300,3500,3700,3900,4100}, // 1500 rpm
{2850,3050,3250,3500,3750,4000,4250,4500}, // 2000 rpm
{2900,3150,3400,3700,4000,4300,4600,4900}, // 2500 rpm
{3000,3300,3600,3950,4300,4650,5000,5350}, // 3000 rpm
{3100,3450,3850,4250,4650,5050,5450,5850}, // 3500 rpm
{3200,3600,4050,4500,4950,5400,5850,6300}, // 4000 rpm
{3300,3750,4250,4750,5250,5750,6250,6750}  // 4500 rpm
};
uint16_t tabla_rpm_tps(uint16_t rpm, uint8_t tps)
{
    uint8_t i;
    uint8_t j;

    /* buscar índice de RPM */

    for(i = 0; i < RPM_POINTS - 1; i++)
    {
        if(rpm < rpm_bins[i+1])
            break;
    }

    /* buscar índice de TPS */

    for(j = 0; j < TPS_POINTS - 1; j++)
    {
        if(tps < tps_bins[j+1])
            break;
    }

    /* devolver valor de tabla */

    return fuel_table[i][j];
}
