#include "AlarmAgent.h"

AlarmAgent::AlarmAgent(bool &status, SemaphoreHandle_t &mutexStatus)
    : Agent("Alarm Task", tskIDLE_PRIORITY + 1, 256),
      status(status), mutexStatus(mutexStatus) {
    gpio_init(PIR_PIN);
    gpio_set_dir(PIR_PIN, GPIO_IN);
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(ALARM_PIN);
    gpio_set_dir(ALARM_PIN, GPIO_OUT);
    gpio_init(SIGNAL_PIN);
    gpio_set_dir(SIGNAL_PIN, GPIO_OUT);
    gpio_put(SIGNAL_PIN, 0);
}

void AlarmAgent::run() {
    while (true) {
        if (gpio_get(PIR_PIN)) {
            if (xSemaphoreTake(mutexStatus, portMAX_DELAY) == pdTRUE) {
                if (status) {
                    gpio_put(ALARM_PIN, 1);
                    gpio_put(LED_PIN, 1);
                    gpio_put(SIGNAL_PIN, 0);
                    xSemaphoreGive(mutexStatus);
                    vTaskDelay(pdMS_TO_TICKS(2000));
                    gpio_put(ALARM_PIN, 0);
                    gpio_put(LED_PIN, 0);
                    gpio_put(SIGNAL_PIN, 1);
                } else {
                    xSemaphoreGive(mutexStatus);
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
