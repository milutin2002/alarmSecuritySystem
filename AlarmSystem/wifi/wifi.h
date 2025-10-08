#ifndef WIFI_H
#define WIFI_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "FreeRTOS.h"
#include  "task.h"
#include "queue.h"
#include "event_groups.h"

#define NET_READY_BIT (1U << 0)

static EventGroupHandle_t netEvents;

#ifndef WIFI_SSID
#define WIFI_SSID "Change me"
#endif

#ifndef WIFI_PASS
#define WIFI_PASS "Change me"
#endif


void wifiTask(void * param);
void mqttTask( void * _);
#endif