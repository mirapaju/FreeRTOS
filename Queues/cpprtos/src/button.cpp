//
// Created by mirap on 8/30/2024.
//

#include <iostream>
#include "button.h"
#define DELAY 300

Button::Button(int btn, QueueHandle_t queue)
    : pin(btn), name("btn" + std::to_string(btn)), btnQ(queue), last_pressed(0) {
    gpio_init(btn);
    gpio_set_dir(btn, GPIO_IN);
    gpio_set_pulls(btn, true, false);
    // Debug statement to verify task creation
    std::cout << "Creating task for pin " << pin << std::endl;
    xTaskCreate(Button::runner, name.c_str(), 512, (void *) this, tskIDLE_PRIORITY +1, &handle);
}

int Button::get_pin() const {
    return pin;
}

void Button::runner(void *params){
    auto *inst = static_cast<Button *>(params);
    inst ->button_task();
}

void Button::button_task() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
       while (true) {
      // state = gpio_get(pin);  // Read the current button state
       current = to_ms_since_boot(get_absolute_time());
       if (!(gpio_get(pin))) { //eli nyt on painettu (state ==0), edellinen state 1
           // Check if the debounce delay has passed
           if ((current - last_pressed) > DELAY) { //aikaa kulunu tarpeeks
               last_pressed = current; //resettaa timer
               //send tthings to queue
               std::cout << "Pressed" << std::endl;
               gpio_put(LED_PIN, true);
               xQueueSend(btnQ, &pin, portMAX_DELAY);
           }
       }
       gpio_put(LED_PIN, false);
       vTaskDelay(pdMS_TO_TICKS(50));
   }
}
/*
    void button_task() {
        while(true) {
            if(gpio_get(pin) == 0) {
                vTaskDelay(pdMS_TO_TICKS(300));
                if(gpio_get(btn) == 0){
                //send stuff to queue
                }
            }
            vTaskDelay(pdMS_TO_TICKS(50));
        }
}*/


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


