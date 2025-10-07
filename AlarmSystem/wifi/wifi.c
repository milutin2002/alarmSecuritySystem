#include"wifi.h"

void wifiTask(void * param){
    if(cyw43_arch_init()){
        printf("Wifi task problem\n");
        while(true){
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
    cyw43_arch_enable_sta_mode();
    cyw43_wifi_pm(&cyw43_state, CYW43_PM2_POWERSAVE_MODE);
    int rc;
    while((rc=cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID,WIFI_PASS,CYW43_AUTH_WPA2_MIXED_PSK,20000))!=0){
        printf("Retrying wifi passwords with ");
        printf(WIFI_SSID);
        printf(" ");
        printf(WIFI_PASS);
        printf("\n");
        printf("Some other error happened with code %d\n",rc);
        int st = cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA);
        printf("Wifi connected with rc=%d",rc);

        vTaskDelay(pdMS_TO_TICKS(3000));  
    }
     uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
    printf("IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    xEventGroupSetBits(netEvents,NET_READY_BIT);
    vTaskDelete(NULL);
}