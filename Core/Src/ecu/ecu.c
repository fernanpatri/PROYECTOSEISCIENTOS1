
#include "main.h"
#include "ecu/ecu.h"
#include "sensores/sensores.h"
#include "control/rpm.h"
#include "control/fuel.h"
#include "sensores/ntcmotor/ntcmotor.h"
#include "actuadores/motorralenti/motorralenti.h"
#include "actuadores/motorralenti/motorralenti_control.h"
#include "control/enriquecimiento/e_posarranque.h"






volatile uint8_t ecu_tick = 0;
static uint8_t counter_100ms = 0;
static uint16_t counter_1s = 0;

volatile float dbg_temp_engine;
volatile float dbg_temp_air;
volatile uint16_t dbg_throttle;
volatile uint32_t dbg_rpm;
volatile uint16_t dbg_throttle_percent;






static uint8_t init_done = 0;
static uint32_t init_timer = 0;





void ecu_init(void)
{
	sensors_init();

   motorralenti_init();

   motorralenti_control_init();

}

void ecu_loop(void)
{
 rpm_timeout_update();
	 /* ===== 1. SENSORES ===== */
	    sensors_update();

	    /* ===== 2. RPM CRÍTICO ===== */


	    uint32_t rpm;

	    __disable_irq();
	    rpm = engine_rpm;
	    __enable_irq();

	    /* ===== 3. INYECCIÓN (CRÍTICO) ===== */
	    uint16_t pw;

	    pw = fuel_calculate_pw(
	            rpm,
	            sensors.throttle_percent);

	    /* ===== 4. ARRANQUE ===== */
	   // posarranque_update();

	    posarranque_update(engine_rpm, HAL_GetTick());
	    /* ===== tareas cada 100 ms ===== */

	    counter_100ms++;

	    if(counter_100ms >= 10)
	    {
	        counter_100ms = 0;

	        /* ===== CALIBRACIÓN INICIAL ===== */


	        int16_t temp_engine = (int16_t)(dbg_temp_engine + 0.5f);
	        motorralenti_control_update(temp_engine);
	       // motorralenti_update();

	       // motorralenti_control_update(dbg_temp_engine);
//MOVER EL MOTOR


  /* ===== DEBUG ===== */
	        dbg_temp_engine = sensors.temp_engine;
	        dbg_temp_air = sensors.temp_air;
	        dbg_throttle = sensors.throttle_adc;
	        dbg_rpm = rpm;
	        dbg_throttle_percent = sensors.throttle_percent;






	    }



	    /* ===== tareas cada 1 segundo ===== */

	    counter_1s++;

	    if(counter_1s >= 100)
	    {
	        counter_1s = 0;

	       // HAL_GPIO_WritePin(ECU2_GPIO_Port, ECU2_Pin, GPIO_PIN_SET);

	      // 	HAL_GPIO_WritePin(ECU2_GPIO_Port, ECU2_Pin, GPIO_PIN_RESET);
	        // tareas cada 1 segundo
	    }


}




