//
// Created by mirap on 9/4/2024.
//

#include "LED.h"


LED::LED(int gpio) : pin(gpio), state(false){
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
}

void LED::on() const{
    gpio_put(pin, true);
}

void LED::off() const{
    gpio_put(pin, false);
}

void LED::toggle(){
    state = !state;
    gpio_put(pin, state);
}

void LED::blink(int delay) const {
    gpio_put(pin, true);
    vTaskDelay(pdMS_TO_TICKS(delay));
    gpio_put(pin, false);
    vTaskDelay(pdMS_TO_TICKS(delay));
}