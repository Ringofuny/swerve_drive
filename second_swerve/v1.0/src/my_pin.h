#pragma once

CAN can(PA_11, PA_12); // CANピン設定（STM32F446REなど）
UnbufferedSerial uart(PC_10, PC_11, 38400);
SerialCtrl fep(uart);

