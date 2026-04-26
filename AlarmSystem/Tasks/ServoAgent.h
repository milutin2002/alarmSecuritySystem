#pragma once

#include "Agent.h"
#include "AlarmShared.h"
#include "queue.h"

#define SERVO_PIN1 12
#define SERVO_PIN2 11

class ServoAgent : public Agent {
public:
    ServoAgent(QueueHandle_t queue);
protected:
    void run() override;
private:
    QueueHandle_t queue;
    int mills1;
    int mills2;
    void setMills(int pin, float mills);
};
