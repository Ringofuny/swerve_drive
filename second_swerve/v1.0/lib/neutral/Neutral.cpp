#include "Neutral.h"

float Neutral::setNeutral(float data) {
    if (data >= 122 && data <= 132) {
        ctrlNewtral = 0; // ニュートラルの設定
    } else {
        ctrlNewtral = (data - 128) * / 128 ; // 回転速度の設定
    }
    return ctrlNewtral;
}