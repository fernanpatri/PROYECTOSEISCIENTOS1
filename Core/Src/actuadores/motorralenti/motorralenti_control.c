#include "actuadores/motorralenti/motorralenti.h"
#include "actuadores/motorralenti/motorralenti_control.h"



/*  1. Inicializa el motor (abre del todo)
 *
2. Calcula una posición objetivo según temperatura

3. Mueve el motor hasta alcanzar esa posición

4. Se detiene cuando llega

*/
/*==================== CONFIG ====================*/

#define CONTROL_PERIOD_MS   200  // cada 200 ms → el sistema decide si mover el motor
#define STEP_MAX            30 // Máximo movimiento por ciclo
#define ERROR_MIN           20  // Zona muerta (deadband)

#define PWM_TRABAJO         700// Potencia del motor

/*==================== VARIABLES ====================*/

static int32_t posicion_actual = 1000;   // empieza abierto no tienes sensor → esta es una posición estimada  muy abierto
static uint32_t timer = 0;// controla el tiempo entre decisiones
static uint8_t init_done = 0;// indica si ya has calibrado
static uint32_t init_timer = 0;// controla cuánto tiempo dura la apertura inicial

/*==================== MAPA TEMPERATURA ====================*/

static int32_t posicion_objetivo(int16_t temp)
{
    if (temp < 40) return 1000;
    if (temp < 50) return 800;
    if (temp < 70) return 500;
    if (temp < 85) return 300;



    return 250;
}

/*==================== INIT ====================*/

void motorralenti_control_init(void) // inicializa
{
    posicion_actual = 1000;
    timer = 0;

    init_done = 0;
    init_timer = 0;
}

/*==================== UPDATE ====================*/

void motorralenti_control_update(int16_t temp)
{
    /*================ CALIBRACIÓN INICIAL =================*/

    if (!init_done)
    {
        init_timer += 100;

        if (init_timer == 100)
        {
            motorralenti_abrir(1000);   // abrir a tope
        }

        if (init_timer >= 1500)
        {
            motorralenti_stop();
            posicion_actual = 1000;
            init_done = 1;
        }

        return;
    }

    /*================ CONTROL NORMAL =================*/

    timer += 100;

    if (timer < CONTROL_PERIOD_MS)
        return;

    timer = 0;

    int32_t objetivo = posicion_objetivo(temp);
    int32_t error = objetivo - posicion_actual;

    /*================ CONTROL =================*/

    if (error > ERROR_MIN)
    {
        uint16_t paso = error;

        if (paso > STEP_MAX)
            paso = STEP_MAX;

        motorralenti_abrir(PWM_TRABAJO);

        posicion_actual += paso;
    }
    else if (error < -ERROR_MIN)
    {
        uint16_t paso = -error;

        if (paso > STEP_MAX)
            paso = STEP_MAX;

        motorralenti_cerrar(PWM_TRABAJO);

        posicion_actual -= paso;
    }
    else
    {
        motorralenti_stop();   // 🔥 CLAVE
    }
}

