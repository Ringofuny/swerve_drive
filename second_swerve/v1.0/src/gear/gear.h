#pragma once 

class Gear {
    public:
        float GearAngle_2(float input_angle, int teethinput, int teeth_output);
        float GearAngle_3(float input_angle, int t1, int t2, int t3);
        float GearAngle_4(float input_angle, int t1, int t2, int t3, int t4);
        float GearAngle_5(float input_angle, int t1, int t2, int t3, int t4, int t5);
};