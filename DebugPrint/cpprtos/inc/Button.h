//
// Created by mirap on 8/30/2024.
//

#ifndef RP2040_FREERTOS_CPP_TEMPLATE_BUTTON_H
#define RP2040_FREERTOS_CPP_TEMPLATE_BUTTON_H

#include "pico/stdlib.h"
#include "pico/time.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include <string>
#include <iostream>


#define DELAY 250

class Button{
public:
    Button(int btn, EventGroupHandle_t egroup);
    void button_task();
private:
    static void runner(void *params);
    int pin;
    const std::string name;
    uint64_t current;
    uint64_t last_pressed;
    TaskHandle_t handle;
    EventGroupHandle_t event_group;
};

#endif //RP2040_FREERTOS_CPP_TEMPLATE_BUTTON_H
