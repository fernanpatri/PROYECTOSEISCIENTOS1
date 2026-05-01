	#include "control/fuel_table.h"

const uint16_t rpm_bins[RPM_POINTS] =
{800,1500,2000,2500,3000,3500,4000,4500};

const uint16_t tps_bins[TPS_POINTS] =
{0,10,20,40,60,80,90,100};

const uint16_t fuel_table[RPM_POINTS][TPS_POINTS] =
{
/* TPS →   0     1     2     3     4     5     6     7  */  // Tanto porciento de apertura de acelerador
/* RPM ↓ */

{1200,1250,1300,1450,1600,1750,1900,2050}, // 1000 rpm
{1200,1300,1400,1550,1700,1850,2000,2150}, // 1500 rpm
{1250,1350,1500,1700,1900,2100,2300,2500}, // 2000 rpm
{1300,1450,1650,1900,2150,2400,2650,2900}, // 2500 rpm
{1350,1550,1800,2100,2400,2700,3000,3300}, // 3000 rpm
{1400,1650,1950,2300,2650,3000,3350,3700}, // 3500 rpm
{1450,1750,2100,2500,2900,3300,3700,4100}, // 4000 rpm
{1500,1850,2250,2700,3150,3600,4050,4500}  // 4500 rpm
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
