#include "control/interpolation.h"

#define TABLE_AT(m, i, j) (m->table[(i)*(m->y_size) + (j)])

uint16_t map2d_get(const map2d_t *map, uint16_t x, uint16_t y)
{
    uint8_t i = 0, j = 0;

    // 🔒 CLAMP
    if (x <= map->x_bins[0]) x = map->x_bins[0];
    if (x >= map->x_bins[map->x_size - 1]) x = map->x_bins[map->x_size - 1] - 1;

    if (y <= map->y_bins[0]) y = map->y_bins[0];
    if (y >= map->y_bins[map->y_size - 1]) y = map->y_bins[map->y_size - 1] - 1;

    // 🔍 Buscar índices
    for (i = 0; i < map->x_size - 1; i++)
        if (x < map->x_bins[i + 1]) break;

    for (j = 0; j < map->y_size - 1; j++)
        if (y < map->y_bins[j + 1]) break;

    uint16_t x1 = map->x_bins[i];
    uint16_t x2 = map->x_bins[i + 1];
    uint16_t y1 = map->y_bins[j];
    uint16_t y2 = map->y_bins[j + 1];

    uint16_t Q11 = TABLE_AT(map, i, j);
    uint16_t Q21 = TABLE_AT(map, i + 1, j);
    uint16_t Q12 = TABLE_AT(map, i, j + 1);
    uint16_t Q22 = TABLE_AT(map, i + 1, j + 1);

    uint16_t fx = ((uint32_t)(x - x1) * 256) / (x2 - x1);
    uint16_t fy = ((uint32_t)(y - y1) * 256) / (y2 - y1);

    uint32_t R1 = ((uint32_t)Q11 * (256 - fx)) + ((uint32_t)Q21 * fx);
    uint32_t R2 = ((uint32_t)Q12 * (256 - fx)) + ((uint32_t)Q22 * fx);

    return (uint16_t)(((R1 * (256 - fy)) + (R2 * fy)) >> 16);
}
