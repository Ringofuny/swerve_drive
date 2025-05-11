#pragma once
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "math.h"
#include "PID.h"

typedef struct {
    float R[2];
    float L;
} In;

typedef struct {
    In Data;
    float angle;
    float rad;
    float speed;
} Ctrl;

extern Ctrl my_wheels[2];

class Steer {
    public:
        Steer();
        void SetData(float stX, float stY, float stY_L);
        float update(int CAN_Data_Coun, float angle); 
        float speed(int rpm, int goal);
        float normalize_angle(float angle_rad);
        float goal_speed;
    private:    
        float X[3];
        float Y[3];
        float L;
};