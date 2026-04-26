#pragma once

#include "Agent.h"


class WifiAgent : public Agent {
public:
    WifiAgent(Agent &mqttAgent);
protected:
    void run() override;
private:
    Agent &mqttAgent;
};


