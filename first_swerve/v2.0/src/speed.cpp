#include "speed.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void Control::Speed::ctrl_Speed(int g_count, int c_count) {
    g_speed = ((2 * M_PI) * g_count) / (8192.0 * 0.01);
    c_speed = ((2 * M_PI) * c_count) / (8192.0 * 0.01);
}

