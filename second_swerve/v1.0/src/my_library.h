#pragma once

#include "mbed.h"
#include "rtos.h"
#include "SerialCtrl_OS6/SerialCtrl.h"
using namespace NITSC;
#include "gear/gear.h"
Gear tooth[2]; // 0→角度　1→速度
#include "steer/steer.h"
Steer Steer_move;
#include "counter/counter.h"
using namespace Exceeded;
Count enc;
#include "Neutral/Neutral.h"
Neutral ctrl[3]; // 0->RX 1->RY 2->LY 
#include "func/func.h"
#include "my_pin.h"
#include "proto.h"