//
// Created by mirap on 8/30/2024.
//

#include <iostream>
#include "button.h"


Button::Button(int btn, EventGroupHandle_t egroup)
        : pin(btn), name("btn" + std::to_string(btn)), event_group(egroup), last_pressed(0) {
    gpio_init(btn);
    gpio_set_dir(btn, GPIO_IN);
    gpio_set_pulls(btn, true, false);
    std::cout << "Creating task for pin " << pin << std::endl;
    xTaskCreate(Button::runner, name.c_str(), 512,
                (void *) this, tskIDLE_PRIORITY +4, &handle);
}


void Button::runner(void *params){
    auto *inst = static_cast<Button *>(params);
    inst ->button_task();
}

//tänne ei varmaan enää tulla ku nappia on painettu?
void Button::button_task() {
    while (true) {
        current = to_ms_since_boot(get_absolute_time());
        if (!(gpio_get(pin))) {
            //check if the debounce delay has passed
            if ((current - last_pressed) > DELAY) {
                last_pressed = current;
               // std::cout << "Pressed" << std::endl;
                xEventGroupSetBits(event_group, 1 << 0);
                //eli pitäskö täällä sitten suspendaa taski?
                vTaskSuspend(handle);
            }
        }
    }
}




