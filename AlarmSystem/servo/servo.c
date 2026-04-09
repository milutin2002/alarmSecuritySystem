#include "pico/stdlib.h"
#include<stdio.h>
#include"hardware/pwm.h"
#include "servo.h"

int currentMills1=1600,currentMills2=400;

void setMills(int pin,float mills){
    pwm_set_gpio_level(pin,(mills/20000.f)*39062.f);
}

void setServo(int pin,float mills){
    gpio_set_function(pin,GPIO_FUNC_PWM);
    uint slice=pwm_gpio_to_slice_num(pin);

    pwm_config config=pwm_get_default_config();
    pwm_config_set_clkdiv(&config,64.f);
    pwm_config_set_wrap(&config,39062.f);


    pwm_init(slice,&config,true);
    setMills(pin,mills);
}

void servoTask(void *){
    while(true){
        enum Action cmd=LEFT;
        BaseType_t res=xQueueReceive(queue,&cmd,0);
        if(res==pdTRUE){
            if(cmd==LEFT){
                if(currentMills1<2400){
                    currentMills1+=50;
                }
                setMills(SERVO_PIN1,currentMills1);
            }
            else if(cmd==RIGHT){
                if(currentMills1>400){
                    currentMills1-=50;
                }
                setMills(SERVO_PIN1,currentMills1);
            }
            else if(cmd==UP){
                printf("Increasing height\n");
                /*if(currentMills2<2400){
                    currentMills2+=50;
                }*/
                //setMills(SERVO_PIN2,currentMills2);
            }
            else{
                /*if(currentMills2>400){
                    currentMills2-=50;
                }
                setMills(SERVO_PIN2,currentMills2);*/
            }
            
        }
        sleep_ms(100);
    }
}