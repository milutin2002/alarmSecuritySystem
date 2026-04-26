#ifndef WIFI_H
#define WIFI_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "FreeRTOS.h"
#include  "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"




#ifndef WIFI_SSID
#define WIFI_SSID "Change me"
#endif

#ifndef WIFI_PASS
#define WIFI_PASS "Change me"
#endif


void wifiTask(void * param);
void mqttTask( void * _);
#endif