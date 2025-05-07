#pragma once

#include "mbed.h"
#include "rtos.h"
#include "SerialCtrl.h"
using namespace NITSC;
#include "steer.h"
Steer Steer_move[2];
#include "Neutral.h"
Neutral ctrl[3]; // 0->RX 1->RY 2->LY 
#include "func.h"
#include "my_pin.h"
#include "proto.h"
#include <math.h>