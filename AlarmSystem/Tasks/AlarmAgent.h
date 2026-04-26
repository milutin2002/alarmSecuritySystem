#pragma once

#include "Agent.h"
#include "semphr.h"

#define PIR_PIN    17
#define LED_PIN    13
#define ALARM_PIN  15
#define SIGNAL_PIN  1

class AlarmAgent : public Agent {
public:
    AlarmAgent(bool &status, SemaphoreHandle_t &mutexStatus);
protected:
    void run() override;
private:
    bool &status;
    SemaphoreHandle_t &mutexStatus;
};


