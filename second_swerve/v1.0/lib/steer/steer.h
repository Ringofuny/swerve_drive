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
    int speed;
} Ctrl;

extern Ctrl my_wheels[2];

class Steer {
    public:
        Steer();
        void SetData(float stX, float stY, float stY_L);
        float update(int CAN_Data_Count); 
        float speed(int rpm);
        float normalize_angle(float angle_rad);
        int goal_speed;
    private:    
        float X[3];
        float Y[3];
        float L;
};