#include "mqtt.h"

bool status=true;


static void onTopic(void *arg,const char *topic,u32_t len){
    printf("MQTT incoming topic %s\n",topic);
}
static void onData(void *arg,const u8_t *data,u16_t len,u8_t flags){
    char buf[8];
    status=!status;
    publishData(status?"ON":"OFF");
}
static void publishData(const char *data){
    if(mqtt_client_is_connected(mq)){
        mqtt_publish(mq,TOPIC_STATUS,data,strlen(data),0,1,NULL,NULL);
    }
}
static void onConnect(mqtt_client_t *client,void *arg,mqtt_connection_status_t st){
    if(st==MQTT_CONNECT_ACCEPTED){
        printf("MQTT connected\n");
        mqtt_set_inpub_callback(mq,onTopic,onData,NULL);
        mqtt_subscribe(mq,TOPIC_SET,0,NULL,NULL);
    }
    else{
        printf("Mqtt connection failed\n");
    }
}

void mqttTask( void * _){
    xEventGroupWaitBits(netEvents,NET_READY_BIT,pdFALSE,pdTRUE,portMAX_DELAY);
    ip_addr_t broker_ip;
    err_t de=netconn_gethostbyname(MQTT_HOST,&broker_ip);
    if(de!=ERR_OK){
        while(true){
            printf("DNS failed (%d)\n",(int)de);
        }
        vTaskDelete(NULL);
        return;
    }
    printf("Broker %s -> %s\n",MQTT_HOST,ipaddr_ntoa(&broker_ip));
    mq=mqtt_client_new();
    struct mqtt_connect_client_info_t ci;
    memset(&ci,0,sizeof(ci));
    ci.client_id="pico-led-1";
    ci.keep_alive=30;
    ci.will_topic=TOPIC_STATUS;
    ci.will_msg="OFF";
    ci.will_qos=0;
    ci.will_retain=1;
    while(true){
        if(!mqtt_client_is_connected(mq)){
            printf("MQTT connecting\n");
           // cyw43_arch_lwip_begin();
            mqtt_client_connect(mq,&broker_ip,MQTT_PORT,onConnect,NULL,&ci);
            //cyw43_arch_lwip_end();
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}