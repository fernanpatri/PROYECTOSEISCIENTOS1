#include "control/rpm.h"
#include "control/enriquecimiento/e_posarranque.h"
#include "actuadores/injector.h"
#include "control/fuel.h"

/*==================== VARIABLES ====================*/

volatile uint32_t engine_rpm = 0;

static uint32_t last_capture = 0;
static uint32_t rpm_filtered = 0;

static uint32_t last_pulse_time = 0;   // ✅ NUEVO (tiempo real)

static uint8_t engine_running = 0;

extern uint16_t tiempo_final_pw;

// NUEVO
static uint8_t injection_counter = 0;
/*==================== CONFIG ====================*/

#define TIMER_FREQ        1000000
#define PULSES_PER_REV    2

#define MIN_PERIOD        100
#define MAX_PERIOD        2000000

#define RPM_FILTER        4

#define RPM_TIMEOUT_MS    400   // ✅ tiempo real
// nuevo
#define INJECTION_DIVIDER   2   // 👉 1 = cada pulso, 2 = cada 2 pulsos, 3 = cada 3...

/*==================== INIT ====================*/

void rpm_init(void)
{
    last_capture = 0;
    engine_rpm = 0;
    rpm_filtered = 0;
    engine_running = 0;
    last_pulse_time = HAL_GetTick();
}

/*==================== CAPTURE ====================*/

void rpm_capture_callback(uint32_t capture)
{
    uint32_t diff;
    uint32_t rpm_raw;
    int32_t filter_diff;

    diff = capture - last_capture;
    last_capture = capture;

    last_pulse_time = HAL_GetTick();   // ✅ CLAVE

    if(diff == 0)
        return;

    if(diff < MIN_PERIOD || diff > MAX_PERIOD)
        return;

    rpm_raw = (TIMER_FREQ * 60) / (diff * PULSES_PER_REV);

    filter_diff = (int32_t)rpm_raw - (int32_t)rpm_filtered;

    rpm_filtered = rpm_filtered + filter_diff / RPM_FILTER;

    engine_rpm = rpm_filtered;

    /*==================== LIMITADOR ====================*/

    if(engine_rpm > 5500)
    {
        injector_close();
        return;
    }

    /*==================== INYECCIÓN ====================*/
/*
    uint16_t pw = tiempo_final_pw;

    injector_open();
    injector_schedule_close(pw);
*/
    injection_counter++;

    if(injection_counter >= INJECTION_DIVIDER)
    {
        injection_counter = 0;
       // uint16_t pw = tiempo_final_pw * INJECTION_DIVIDER;
        uint16_t pw = tiempo_final_pw;

        injector_open();
        injector_schedule_close(pw);
    }
    /*==================== ARRANQUE ====================*/

    if(engine_rpm > 600 && engine_running == 0)
    {
        posarranque_begin();
        engine_running = 1;
    }

    if(engine_rpm < 400)
    {
        engine_running = 0;
    }
}

/*==================== TIMEOUT ====================*/

void rpm_timeout_update(void)
{
    uint32_t now = HAL_GetTick();

    if((now - last_pulse_time) > RPM_TIMEOUT_MS)
    {
        engine_rpm = 0;
        rpm_filtered = 0;
        engine_running = 0;

        injector_close();
    }
}
