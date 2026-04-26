#include "Agent.h"

Agent::Agent(const char *name,UBaseType_t priority,uint16_t stackSize)
    : name(name), priority(priority), stackSize(stackSize),taskHandle(nullptr) {}

void Agent::start(){
    xTaskCreate(taskEntry,name,stackSize,this,priority,&taskHandle);
}
void Agent::taskEntry(void *param){
    Agent *agent=static_cast<Agent*>(param);
    agent->run();
}
TaskHandle_t Agent::getTask(){
    return taskHandle;
}