#include "ServoAgent.h"
#include "hardware/pwm.h"

ServoAgent::ServoAgent(QueueHandle_t queue)
    : Agent("Servo Task", tskIDLE_PRIORITY + 1, 256),
      queue(queue), mills1(1600), mills2(400) {
    gpio_set_function(SERVO_PIN1, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(SERVO_PIN1);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 64.f);
    pwm_config_set_wrap(&config, 39062.f);
    pwm_init(slice, &config, true);
    setMills(SERVO_PIN1, mills1);
}

void ServoAgent::setMills(int pin, float mills) {
    pwm_set_gpio_level(pin, (mills / 20000.f) * 39062.f);
}

void ServoAgent::run() {
    Action cmd;
    while (true) {
        if (xQueueReceive(queue, &cmd, portMAX_DELAY) == pdTRUE) {
            if (cmd == LEFT) {
                if (mills1 < 2400) mills1 += 50;
                setMills(SERVO_PIN1, mills1);
            } else if (cmd == RIGHT) {
                if (mills1 > 400) mills1 -= 50;
                setMills(SERVO_PIN1, mills1);
            } else if (cmd == UP) {
                printf("Increasing height\n");
            }
        }
    }
}
