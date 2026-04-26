#include "WifiAgent.h"

WifiAgent::WifiAgent(MqttAgent &mqttAgent)
    : Agent("Wifi Task", tskIDLE_PRIORITY + 1, 256), mqttAgent(mqttAgent) {}

void WifiAgent::run() {
    printf("Starting wifi connection process\n");
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_SPAIN)) {
        while (true) {
            printf("Wifi init failed\n");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
    cyw43_arch_enable_sta_mode();
    cyw43_wifi_pm(&cyw43_state, CYW43_PM2_POWERSAVE_MODE);

    int rc;
    while ((rc = cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_MIXED_PSK, 20000)) != 0) {
        printf("Retrying wifi %s, error %d\n", WIFI_SSID, rc);
        vTaskDelay(pdMS_TO_TICKS(3000));
    }

    uint8_t *ip = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
    printf("IP address %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);

    mqttAgent.start();
    vTaskDelete(NULL);
}
