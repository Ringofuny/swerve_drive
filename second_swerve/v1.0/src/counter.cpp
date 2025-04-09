#include "counter.h"

int Exceeded::Count::Adjustment(int pre, int current)
{
    error = current - pre;
    if (error > 4096) error -= 8192;
    if (error < -4096) error += 8192;
    return error;
}