#include "gear.h"

float Gear::GearAngle_2(float input_angle, int teeth_input, int teeth_output) {
    return input_angle * ((float)teeth_input / (float)teeth_output);
}

float Gear::GearAngle_3(float input_angle, int t1, int t2, int t3) {
    float ratio = ((float)t1 / t2) * ((float)t2 / t3); // 結局 t1 / t3
    int direction = -1;  // 偶数段階で戻るから、3段だと逆回転
    return input_angle * ratio * direction;
}


float Gear::GearAngle_4(float input_angle, int t1, int t2, int t3, int t4) {
    float ratio = ((float)t1 / t2) * ((float)t2 / t3) * ((float)t3 / t4); // = t1 / t4
    int direction = 1; // 偶数段 → 正転
    return input_angle * ratio * direction;
}

float Gear::GearAngle_5(float input_angle, int t1, int t2, int t3, int t4, int t5) {
    float ratio = ((float)t1 / t2) * ((float)t2 / t3) * ((float)t3 / t4) * ((float)t4 / t5); // = t1 / t5
    int direction = -1; // 奇数段 → 逆転
    return input_angle * ratio * direction;
}

