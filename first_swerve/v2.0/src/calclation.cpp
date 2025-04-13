#include "calclation.h"
PID PD[2];

calclation_main::Calclation::Calclation(){
    real_pwm = 0;
    digital = 2;
    G = 0;
    rad = 0;
    goal = 0;
    output = 0;
}

void  calclation_main::Calclation::my_output(float G, float rad) {
    output = PD[0].PD_move(G, rad);
}

int calclation_main::Calclation::my_digital(float G, float rad) {
    return digitalWrite_f(G, rad);
}

float calclation_main::Calclation::go_mygoal() {
    return MAXIN::fmin((output / 1.58), 1.0);
}

float calclation_main::Calclation::calclated_PWM() {
    return MAXIN::fmin(fabs(go_mygoal()), 1.0);
}


calclation_sub::Calclation::Calclation() {
    real_pwm = 0;
    digital = 2;
    g_speed = 0;
    c_speed = 0;
    goal = 0;
    output = 0;   
}

void calclation_sub::Calclation::my_output(float G, float rad) {
    output = PD[1].PID_move(g_speed, c_speed);
}

float calclation_sub::Calclation::go_mygoal()
{
    return MAXIN::fmin(MAXIN::fmax(output, 0.0), 1.0);
}

float calclation_sub::Calclation::calclated_PWM()
{
    return -go_mygoal();
}
