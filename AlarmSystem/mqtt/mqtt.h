#include"../wifi/wifi.h"

#include "lwip/api.h"
#include "lwip/err.h"
#include "lwip/apps/mqtt.h"


#ifndef MQTT_H
#define MQTT_H

#define MQTT_HOST "broker.emqx.io"
#define MQTT_PORT 1883
#define TOPIC_SET "pico/led/set"
#define TOPIC_STATUS "pico/led/status"

static mqtt_client_t *mq=NULL;

static void onTopic(void *arg,const char *topic,u32_t len);
static void onData(void *arg,const u8_t *data,u16_t len,u8_t flags);
static void publishData(const char *data);
static void onConnect(mqtt_client_t *client,void *arg,mqtt_connection_status_t st);
#endif

