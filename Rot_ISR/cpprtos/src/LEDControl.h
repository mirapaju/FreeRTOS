//
// Created by mirap on 9/3/2024.
//

#ifndef EXERCISE2_LEDCONTROL_H
#define EXERCISE2_LEDCONTROL_H

#include "pico/stdlib.h"
#include "pico/time.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <iostream>

class Led_control{
public:
    Led_control(int gpio, QueueHandle_t q);
    void led_task();
private:
    static void runner(void *params);
    int pin;
    TaskHandle_t handle;
    QueueHandle_t events;
    const std::string name;
};

#endif //EXERCISE2_LEDCONTROL_H
