#include "func.h"

int digitalWrite_f(double goal, double current) {
    double diff = goal - current;

    // 角度の差分を -π ~ π に補正
    if (diff > M_PI) {
        diff -= 2 * M_PI;
    } else if (diff < -M_PI) {
        diff += 2 * M_PI;
    }

    // 目標角度と現在角度がほぼ一致
    if (fabs(fmod(diff + M_PI, 2 * M_PI) - M_PI) < 0.34) {
        return 2;  // 停止
    } else {
        return 0;
    }

    // // 目標角度と現在角度がほぼ一致
    // if (fabs(diff) <= 0.34) {
    //     return 2;  // 停止
    // }

    // 最短回転方向を選択
    // return (diff > 0) ? 1 : 0;  // 反時計回りなら 1, 時計回りなら 0
}

namespace MAXIN {
    double fmin(double x, double y) {
        return (x < y) ? x : y;
    }

    double fmax(double x, double y) {
        return (x > y) ? x : y;
    }
}
