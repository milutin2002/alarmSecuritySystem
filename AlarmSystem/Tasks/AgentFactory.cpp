#include "AgentFactory.h"
#include "AlarmAgent.h"
#include "MqttAgent.h"
#include "ServoAgent.h"
#include "WifiAgent.h"

Agent* AgentFactory::createAlarmAgent(bool &status, SemaphoreHandle_t &mutexStatus) {
    return new AlarmAgent(status, mutexStatus);
}

Agent* AgentFactory::createMqttAgent(bool &status, SemaphoreHandle_t &mutexStatus, QueueHandle_t queue) {
    return new MqttAgent(status, mutexStatus, queue);
}

Agent* AgentFactory::createServoAgent(QueueHandle_t queue) {
    return new ServoAgent(queue);
}

Agent* AgentFactory::createWifiAgent(Agent &mqttAgent) {
    return new WifiAgent(mqttAgent);
}