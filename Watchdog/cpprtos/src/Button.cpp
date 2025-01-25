//
// Created by mirap on 8/30/2024.
//

#include <iostream>
#include "button.h"

Button::Button(int btn, EventGroupHandle_t egroup, Debug *dbg)
        : pin(btn), name("btn" + std::to_string(btn)), event_group(egroup), last_pressed(0), logger(dbg) {
    gpio_init(btn);
    gpio_set_dir(btn, GPIO_IN);
    gpio_set_pulls(btn, true, false);

    task_ID = ++next_ID;
    std::cout << "Creating task for pin " << pin << std::endl;
    xTaskCreate(Button::runner, name.c_str(), 512,
                (void *) this, tskIDLE_PRIORITY +2, &handle);
}

int Button::next_ID =0;

void Button::runner(void *params){
    auto *inst = static_cast<Button *>(params);
    inst ->button_task();
}


//. Tasks 1 – 3 implement a loop that waits for button presses. When a
//button is pressed and released the task sets a bit in the event group, prints a debug message, and goes
//back to wait for another button press. Holding the button without releasing must block the main loop from
//running. Each task monitors one button.

//NAPPI PITÄÄ OLLA YLHÄÄLLÄ ENNENKU LÄHETETÄÄN MITÄÄN TAI KÄÄNNELLÄÄN BITTEJÄ
void Button::button_task() {
    while (true) {
        if(!(gpio_get(pin))){
            while (!(gpio_get(pin))){
                vTaskDelay(pdMS_TO_TICKS(10));
            }
            xEventGroupSetBits(event_group, 1 << (task_ID-1)); //set the event bit
            logger->debug("Task %d here, hello!\n", task_ID, 0, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}




