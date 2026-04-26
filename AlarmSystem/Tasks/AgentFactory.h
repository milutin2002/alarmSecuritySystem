#pragma once

#include "Agent.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

class AgentFactory {
public:
    Agent* createAlarmAgent(bool &status, SemaphoreHandle_t &mutexStatus);
    Agent* createMqttAgent(bool &status, SemaphoreHandle_t &mutexStatus, QueueHandle_t queue);
    Agent* createServoAgent(QueueHandle_t queue);
    Agent* createWifiAgent(Agent &mqttAgent);
};