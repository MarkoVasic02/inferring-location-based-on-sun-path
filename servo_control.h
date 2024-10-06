#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <ESP32Servo.h>

extern int h0, v0;

void initServos();
void move(int v, int h);
void scanTable(int vstart, int vend, int hstart, int hend);
void scanTable2(int vstart, int vend, int hstart, int hend);

#endif
