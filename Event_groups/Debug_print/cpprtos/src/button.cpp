//
// Created by mirap on 8/30/2024.
//

#include <iostream>
#include "button.h"


Button::Button(int btn, QueueHandle_t queue)
    : pin(btn), name("btn" + std::to_string(btn)), btnQ(queue), last_pressed(0) {
    gpio_init(btn);
    gpio_set_dir(btn, GPIO_IN);
    gpio_set_pulls(btn, true, false);
    std::cout << "Creating task for pin " << pin << std::endl;
    xTaskCreate(Button::runner, name.c_str(), 512, (void *) this, tskIDLE_PRIORITY +1, &handle);
}


void Button::runner(void *params){
    auto *inst = static_cast<Button *>(params);
    inst ->button_task();
}


void Button::button_task() {
       while (true) {
       current = to_ms_since_boot(get_absolute_time());
       if (!(gpio_get(pin))) {
           //check if the debounce delay has passed
           if ((current - last_pressed) > DELAY) {
               last_pressed = current;
               std::cout << "Pressed" << std::endl;
               xQueueSend(btnQ, &pin, portMAX_DELAY);
           }
       }
       vTaskDelay(pdMS_TO_TICKS(50));
   }
}



/* while(true) {
     if(gpio_get(pin) == 0) {
         std::cout << "Checking pin: " << pin << std::endl;
         std::cout << gpio_get(pin) << std::endl;
         vTaskDelay(pdMS_TO_TICKS(300));
         if(gpio_get(pin) == 0){
             std::cout << "Pressed" << std::endl;
         }
     }
     vTaskDelay(pdMS_TO_TICKS(50));
 }*/


