#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "FreeRTOS.h"
#include  "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"

class Agent {
public:
    Agent(const char* name, UBaseType_t priority, uint16_t stackSize);
    void start();
    static void taskEntry(void *param);
protected:
    virtual void run() = 0;
    TaskHandle_t getTask();
private:
    const char* name;
    UBaseType_t priority;
    uint16_t stackSize;
    TaskHandle_t taskHandle;
};