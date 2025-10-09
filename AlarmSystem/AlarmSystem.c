#include "hardware/uart.h"
#include "mqtt/mqtt.h"

#define PIR_PIN 17
#define LED_PIN 13
#define ALARM_PIN 15
#define SIGNAL_PIN 1

#define UART_PIN 0
#define BAUD_RATE 115200
#define UART_ID uart0

extern bool status;

absolute_time_t lastTriger;
bool motionDetecton=false;
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
void initUart(){
    uart_init(UART_ID,BAUD_RATE);
    gpio_set_function(UART_PIN,GPIO_FUNC_UART);
}
void detectMotion(uint gpio, uint32_t events){
    absolute_time_t now=get_absolute_time();
    if(absolute_time_diff_us(lastTriger,now)>2*1000*10){
        lastTriger=now;
        motionDetecton=true;
    }
}
void alarmTask(void * _){
    while(true){
        printf("Trying to detect\n");
        if(motionDetecton && status){
            absolute_time_t now=get_absolute_time();
            lastTriger=now;
            gpio_put(ALARM_PIN,1);
            gpio_put(LED_PIN,1);
            uart_puts(UART_ID,"Yes\n");
            sleep_ms(2000);
            gpio_put(ALARM_PIN,0);
            gpio_put(LED_PIN,0);
            motionDetecton=false;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void uartSendBlocking(char *s){
    while(*s){
        while(!uart_is_writable(UART_ID)){
            tight_loop_contents();
        }
        uart_putc_raw(UART_ID,*s);
        s++;
    }
    uart_tx_wait_blocking(UART_ID);
}
int main()
{
    stdio_init_all();
    initGpio();
    initUart();
    sleep_ms(5000);
    uartSendBlocking("\n");
    lastTriger=get_absolute_time();
    printf("Starting app\n");
    netEvents=xEventGroupCreate();
    gpio_set_irq_enabled_with_callback(PIR_PIN,GPIO_IRQ_EDGE_RISE,true,&detectMotion);
    xTaskCreate(wifiTask,"Wifi task",4096,NULL,tskIDLE_PRIORITY+4,NULL);
    xTaskCreate(alarmTask,"Alarm task",256,NULL,tskIDLE_PRIORITY+1,NULL);
    vTaskStartScheduler();
    while(true){}
    /*lastTriger=get_absolute_time();
    gpio_set_irq_enabled_with_callback(PIR_PIN,GPIO_IRQ_EDGE_RISE,true,&detectMotion);
    while(true){
        if(motionDetecton){
            absolute_time_t now=get_absolute_time();
            lastTriger=now;
            gpio_put(ALARM_PIN,1);
            gpio_put(LED_PIN,1);
            uart_puts(UART_ID,"Yes\n");
            sleep_ms(2000);
            gpio_put(ALARM_PIN,0);
            gpio_put(LED_PIN,0);
            motionDetecton=false;
            
        }
        tight_loop_contents();
        sleep_ms(10);
    }*/
}
