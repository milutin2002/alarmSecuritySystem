#include "alarm/alarm.h"
#include "servo/servo.h"

extern bool status;
extern SemaphoreHandle_t mutexStatus;

EventGroupHandle_t netEvents;

int main()
{
    stdio_init_all();
    sleep_ms(5000);
    printf("Starting app\n");
    netEvents = xEventGroupCreate();
    setServo(SERVO_PIN1, currentMills1);
    initAlarm();
    mutexStatus = xSemaphoreCreateMutex();
    queue = xQueueCreate(COUNT_QUEUE_LEN, sizeof(enum Action));
    xTaskCreate(wifiTask, "Wifi task", 256, NULL, tskIDLE_PRIORITY+1, NULL);
    xTaskCreate(alarmTask, "Alarm task", 256, NULL, tskIDLE_PRIORITY+1, NULL);
    xTaskCreate(servoTask, "Servo task", 256, NULL, tskIDLE_PRIORITY+1, NULL);
    vTaskStartScheduler();
    while(true){}
}
