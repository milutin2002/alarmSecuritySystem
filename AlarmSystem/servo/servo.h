#ifndef SERVO_H
#define SERVO_H
#include "mqtt/mqtt.h"

extern int currentMills1,currentMills2;
#define SERVO_PIN1 12
#define SERVO_PIN2 11

void setMills(int pin,float mills);
void setServo(int pin,float mills);
void servoTask(void *);

#endif