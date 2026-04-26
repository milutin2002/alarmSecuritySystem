#pragma once

#ifndef WIFI_SSID
#define WIFI_SSID "Change me"
#endif

#ifndef WIFI_PASS
#define WIFI_PASS "Change me"
#endif

#include "Agent.h"
#include "AlarmShared.h"

#include "lwip/apps/mqtt.h"

class MqttAgent : public Agent {
public:
    MqttAgent(bool &status, SemaphoreHandle_t &mutexStatus, QueueHandle_t queue);
protected:
    void run() override;
private:
    bool &status;
    SemaphoreHandle_t &mutexStatus;
    QueueHandle_t queue;
    mqtt_client_t *mqttClient;
    char currentTopic[64];

    void publishData(const char *data);
    static void onTopic(void *arg, const char *topic, u32_t len);
    static void onData(void *arg, const u8_t *data, u16_t len, u8_t flags);
    static void onConnect(mqtt_client_t *client, void *arg, mqtt_connection_status_t st);
};


