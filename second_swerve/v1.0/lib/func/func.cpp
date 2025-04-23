#include "func.h"

int fmap(double x, double in_min, double in_max, int out_min, int out_max) {
    // 安全にクランプも入れる
    if (x < in_min) x = in_min;
    if (x > in_max) x = in_max;
    return (int)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}