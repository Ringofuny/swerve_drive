#pragma once

Ticker canSend;
CANMessage canMsgSend;
CANMessage canMsgReceive;

float R[2];
float L;
void settingCtrl();
void sendV();

float output_current = 0;
float g_angle = 0;
void Angle_Speed();
Ticker PID_in;