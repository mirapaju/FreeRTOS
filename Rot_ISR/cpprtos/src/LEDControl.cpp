//
// Created by mirap on 9/3/2024.
//

#include "LEDControl.h"

Led_control::Led_control(int gpio, QueueHandle_t q)
            : pin(gpio), events(q), name(std::to_string(gpio)){
    gpio_init(gpio);
    gpio_set_dir(gpio, true);
    xTaskCreate(Led_control::runner, name.c_str(), 512, (void*) this, tskIDLE_PRIORITY +1, &handle);
}


void Led_control::runner(void *params){
    auto *inst = static_cast<Led_control *>(params);
    inst ->led_task();
}


void Led_control::led_task() {
    int gpio;
    if (xQueueReceive(events, &gpio, portMAX_DELAY == pdTRUE)){
        std::cout << "Received event: " << gpio << " from somewhere over the rainbow" << std::endl;
    }
}
