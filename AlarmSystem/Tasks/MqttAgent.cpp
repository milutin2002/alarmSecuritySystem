#include "MqttAgent.h"
#include "lwip/api.h"
#include "lwip/err.h"
#include <string.h>

MqttAgent::MqttAgent(bool &status, SemaphoreHandle_t &mutexStatus, QueueHandle_t queue)
    : Agent("Mqtt Task", tskIDLE_PRIORITY + 2, 1024),
      status(status), mutexStatus(mutexStatus), queue(queue),
      mqttClient(nullptr) {
    currentTopic[0] = '\0';
}

void MqttAgent::onTopic(void *arg, const char *topic, u32_t len) {
    MqttAgent *self = static_cast<MqttAgent*>(arg);

    Action cmd;
    bool isServoCmd = true;
    if (strcmp(topic, TOPIC_SET) == 0) {
        if (xSemaphoreTake(self->mutexStatus, portMAX_DELAY) == pdTRUE) {
            self->status = !self->status;
            xSemaphoreGive(self->mutexStatus);
        }
        self->publishData(self->status ? "ON" : "OFF");
    }
    else if      (strcmp(topic, TOPIC_LEFT)  == 0) cmd = LEFT;
    else if (strcmp(topic, TOPIC_RIGHT) == 0) cmd = RIGHT;
    else if (strcmp(topic, TOPIC_UP)    == 0) cmd = UP;
    else if (strcmp(topic, TOPIC_DOWN)  == 0) cmd = DOWN;
    else isServoCmd = false;

    if (isServoCmd) {
        xQueueSendToBack(self->queue, &cmd, 0);
    }
}

void MqttAgent::onData(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    MqttAgent *self = static_cast<MqttAgent*>(arg);
    printf("Handling topic %s\n", self->currentTopic);

    if (strcmp(self->currentTopic, TOPIC_SET) == 0) {
        if (xSemaphoreTake(self->mutexStatus, portMAX_DELAY) == pdTRUE) {
            self->status = !self->status;
            xSemaphoreGive(self->mutexStatus);
        }
        self->publishData(self->status ? "ON" : "OFF");
    } else if (strcmp(self->currentTopic, TOPIC_LEFT) == 0) {
        Action cmd = LEFT;
        xQueueSendToBack(self->queue, &cmd, 0);
    } else if (strcmp(self->currentTopic, TOPIC_RIGHT) == 0) {
        Action cmd = RIGHT;
        xQueueSendToBack(self->queue, &cmd, 0);
    } else if (strcmp(self->currentTopic, TOPIC_UP) == 0) {
        Action cmd = UP;
        printf("Going up\n");
        xQueueSendToBack(self->queue, &cmd, 0);
    } else if (strcmp(self->currentTopic, TOPIC_DOWN) == 0) {
        Action cmd = DOWN;
        printf("Going down\n");
        xQueueSendToBack(self->queue, &cmd, 0);
    }
}

void MqttAgent::publishData(const char *data) {
    if (mqtt_client_is_connected(mqttClient)) {
        mqtt_publish(mqttClient, TOPIC_STATUS, data, strlen(data), 0, 1, NULL, NULL);
    }
}

void MqttAgent::onConnect(mqtt_client_t *client, void *arg, mqtt_connection_status_t st) {
    MqttAgent *self = static_cast<MqttAgent*>(arg);
    if (st == MQTT_CONNECT_ACCEPTED) {
        printf("MQTT connected\n");
        mqtt_set_inpub_callback(self->mqttClient, onTopic, onData, self);
        mqtt_subscribe(self->mqttClient, TOPIC_SET,   0, NULL, NULL);
        mqtt_subscribe(self->mqttClient, TOPIC_LEFT,  0, NULL, NULL);
        mqtt_subscribe(self->mqttClient, TOPIC_RIGHT, 0, NULL, NULL);
        mqtt_subscribe(self->mqttClient, TOPIC_DOWN,  0, NULL, NULL);
        mqtt_subscribe(self->mqttClient, TOPIC_UP,    0, NULL, NULL);
    } else {
        printf("MQTT connection failed\n");
    }
}

void MqttAgent::run() {
    printf("Entering MQTT task\n");
    ip_addr_t brokerIp;
    err_t de = netconn_gethostbyname(MQTT_HOST, &brokerIp);
    if (de != ERR_OK) {
        while (true) {
            printf("DNS failed (%d)\n", (int)de);
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
        return;
    }
    printf("Broker %s -> %s\n", MQTT_HOST, ipaddr_ntoa(&brokerIp));
    mqttClient = mqtt_client_new();

    struct mqtt_connect_client_info_t ci;
    memset(&ci, 0, sizeof(ci));
    ci.client_id   = "pico-led-1";
    ci.keep_alive  = 30;
    ci.will_topic  = TOPIC_STATUS;
    ci.will_msg    = "ON";
    ci.will_qos    = 0;
    ci.will_retain = 1;

    while (true) {
        if (!mqtt_client_is_connected(mqttClient)) {
            printf("MQTT connecting\n");
            mqtt_client_connect(mqttClient, &brokerIp, MQTT_PORT, onConnect, this, &ci);
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
