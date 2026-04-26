#ifndef ALARM_H
#define ALARM_H

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define PIR_PIN    17
#define LED_PIN    13
#define ALARM_PIN  15
#define SIGNAL_PIN  1

void initAlarm();
void alarmTask(void *_);

#endif
