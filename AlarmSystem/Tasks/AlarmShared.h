#pragma once

enum Action { LEFT, RIGHT, UP, DOWN };

#define COUNT_QUEUE_LEN  10

#define MQTT_HOST    "broker.emqx.io"
#define MQTT_PORT    1883
#define TOPIC_SET    "pico/alarm/set"
#define TOPIC_STATUS "pico/alarm/status"
#define TOPIC_LEFT   "pico/servo/left"
#define TOPIC_RIGHT  "pico/servo/right"
#define TOPIC_UP     "pico/servo/up"
#define TOPIC_DOWN   "pico/servo/down"
