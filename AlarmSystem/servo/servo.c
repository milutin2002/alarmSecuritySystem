#include "pico/stdlib.h"
#include<stdio.h>
#include"hardware/pwm.h"

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