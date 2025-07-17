#include <stdio.h>
#include "pico/stdlib.h"


#define PIR_PIN 17
#define LED_PIN 13
#define ALARM_PIN 15
#define SIGNAL_PIN 1
absolute_time_t lastTriger;

void initGpio(){
    gpio_init(PIR_PIN);
    gpio_set_dir(PIR_PIN,GPIO_IN);
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN,GPIO_OUT);
    gpio_init(ALARM_PIN);
    gpio_set_dir(ALARM_PIN,GPIO_OUT);
    gpio_init(SIGNAL_PIN);
    gpio_set_dir(SIGNAL_PIN,GPIO_OUT);
    gpio_put(SIGNAL_PIN,0);
}
void detectMotion(){
    absolute_time_t now=get_absolute_time();
    if(gpio_get(PIR_PIN) && absolute_time_diff_us(lastTriger,now)>2*1000 *10){
        gpio_put(ALARM_PIN,1);
        gpio_put(LED_PIN,1);
        gpio_put(SIGNAL_PIN,1);
        sleep_ms(50);
        lastTriger=now;
        gpio_put(ALARM_PIN,0);
        gpio_put(LED_PIN,0);
        gpio_put(SIGNAL_PIN,0);
    }
}
int main()
{
    stdio_init_all();

    initGpio();
    lastTriger=get_absolute_time();
    while(true){
        tight_loop_contents();
        detectMotion();
        sleep_ms(10);
    }
}
