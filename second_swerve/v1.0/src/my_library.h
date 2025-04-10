#pragma once

#include "mbed.h"
#include "rtos.h"
#include "SerialCtrl_OS6/SerialCtrl.h"
using namespace NITSC;
#include "gear.h"
Gear tooth[2]; // 0→角度　1→速度
#include "steer.h"
Steer Steer_move;
#include "counter.h"
using namespace Exceeded;
Count enc;
#include "Neutral.h"
Neutral ctrl[3]; // 0->RX 1->RY 2->LY 
#include "func.h"
#include "my_pin.h"
#include "proto.h"