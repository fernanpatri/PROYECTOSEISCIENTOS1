	#include "control/fuel_table.h"

const uint16_t rpm_bins[RPM_POINTS] =
{800,1500,2000,2500,3000,3500,4000,4500};

const uint16_t tps_bins[TPS_POINTS] =
{0,10,20,40,60,80,90,100};

const uint16_t fuel_table[RPM_POINTS][TPS_POINTS] =
{
/* TPS →   0     1     2     3     4     5     6     7  */
/* RPM ↓ */

{1000,1050,1100,1200,1350,1500,1650,1800}, // 1000 rpm
{1000,1100,1200,1350,1500,1700,1850,2000}, // 1500 rpm
{1050,1150,1300,1500,1700,1900,2100,2300}, // 2000 rpm
{1100,1250,1450,1700,1950,2200,2450,2700}, // 2500 rpm
{1150,1350,1600,1900,2200,2500,2800,3100}, // 3000 rpm
{1200,1450,1750,2100,2450,2800,3150,3500}, // 3500 rpm
{1250,1550,1900,2300,2700,3100,3500,3900}, // 4000 rpm
{1300,1650,2050,2500,2950,3400,3850,4300}  // 4500 rpm
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
