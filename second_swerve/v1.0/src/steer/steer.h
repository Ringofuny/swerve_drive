#pragma once
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "math.h"
#include "PID/PID.h"

struct wheel {
    float angle;
    float speed;
};
extern wheel my_wheels[2];

class Steer {
    public:
        Steer();
        void SetData(float stX, float stY, float stY_L);
        float update(float CAN_Data_Count); 
    private:    
        float X[3];
        float Y[3];
        float L;
};