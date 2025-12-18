#include <stdio.h>
#include "pico/stdlib.h"
#include "servo.h"
#include "mqtt/mqtt.h"


EventGroupHandle_t netEvents;
int main()
{
    stdio_init_all();
    setServo(SERVO_PIN1,currentMills1);
    //setServo(SERVO_PIN2,currentMills2);
    netEvents=xEventGroupCreate();
    queue=xQueueCreate(COUNT_QUEUE_LEN,sizeof(enum Action));
    xTaskCreate(wifiTask,"Wifi task",256,NULL,tskIDLE_PRIORITY+1,NULL);
    xTaskCreate(mqttTask,"Mqtt task",256,NULL,tskIDLE_PRIORITY+1,NULL);
    xTaskCreate(servoTask,"Servo task",256,NULL,tskIDLE_PRIORITY+1,NULL);
    vTaskStartScheduler();
    while(true){}
}
