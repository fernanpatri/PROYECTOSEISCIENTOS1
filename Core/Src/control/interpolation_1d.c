#include <stdint.h>

uint16_t interp1d(
    int16_t x,
    const int16_t *axis,
    const uint16_t *values,
    uint8_t size)
{
    uint8_t i;

    // 🔒 Clamp
    if (x <= axis[0])
        return values[0];

    if (x >= axis[size - 1])
        return values[size - 1];

    // 🔍 Buscar intervalo
    for (i = 0; i < size - 1; i++)
    {
        if (x < axis[i + 1])
        {
            int16_t x1 = axis[i];
            int16_t x2 = axis[i + 1];

            uint16_t y1 = values[i];
            uint16_t y2 = values[i + 1];

            return y1 + ((x - x1) * (y2 - y1)) / (x2 - x1);
        }
    }

    return values[size - 1];
}
