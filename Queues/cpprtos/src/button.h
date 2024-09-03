//
// Created by mirap on 8/30/2024.
//

#ifndef RP2040_FREERTOS_CPP_TEMPLATE_BUTTON_H
#define RP2040_FREERTOS_CPP_TEMPLATE_BUTTON_H

#include "pico/stdlib.h"
#include "pico/time.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <string>
#include <iostream>


#define LED_PIN 20

class Button{
public:
        Button(int btn, QueueHandle_t queue);
        void button_task();
private:
    static void runner(void *params);
    int pin;
    const std::string name;
    QueueHandle_t btnQ;
    uint64_t current;
    uint64_t last_pressed;
    TaskHandle_t handle;
};

#endif //RP2040_FREERTOS_CPP_TEMPLATE_BUTTON_H
