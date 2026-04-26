#include "Tasks/AgentFactory.h"
#include "Tasks/AlarmShared.h"

int main() {
    stdio_init_all();
    sleep_ms(5000);
    printf("Starting app\n");

    bool status = true;
    SemaphoreHandle_t mutexStatus = xSemaphoreCreateMutex();
    QueueHandle_t queue = xQueueCreate(COUNT_QUEUE_LEN, sizeof(Action));

    if (mutexStatus == nullptr || queue == nullptr) {
        printf("Failed to create shared resources\n");
        while (true) { vTaskDelay(pdMS_TO_TICKS(1000)); }
    }

    AgentFactory factory;

    Agent *mqtt  = factory.createMqttAgent(status, mutexStatus, queue);
    Agent *wifi  = factory.createWifiAgent(*mqtt);
    Agent *alarm = factory.createAlarmAgent(status, mutexStatus);
    Agent *servo = factory.createServoAgent(queue);

    if (mqtt == nullptr || wifi == nullptr || alarm == nullptr || servo == nullptr) {
        printf("Failed to create agents\n");
        while (true) { vTaskDelay(pdMS_TO_TICKS(1000)); }
    }

    wifi->start();
    alarm->start();
    servo->start();

    vTaskStartScheduler();
    while (true) {}
}