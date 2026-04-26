#pragma once

#include "Agent.h"
#include "MqttAgent.h"
#include "wifi/wifi.h"

class WifiAgent : public Agent {
public:
    WifiAgent(MqttAgent &mqttAgent);
protected:
    void run() override;
private:
    MqttAgent &mqttAgent;
};
