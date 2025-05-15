#include "Neutral.h"

float Neutral::setNeutral(uint8_t data) {
    if (data >= 122 && data <= 132) {
        ctrlNewtral = 0.0; // ニュートラルの設定
    } else {
        ctrlNewtral = (data - 128) / 128.0f * (-1.0f); // 回転速度の設定
    }
    return ctrlNewtral;
}