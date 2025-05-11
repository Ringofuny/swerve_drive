#pragma once

CAN can(PA_11, PA_12); // CANピン設定（STM32F446REなど）
UnbufferedSerial uart(PC_10, PC_11, 38400);
SerialCtrl fep(uart);

DigitalIn enc1_a(PA_8);
DigitalIn enc1_b(PA_9);

InterruptIn swi1_a(PA_8);
InterruptIn swi1_b(PA_9);