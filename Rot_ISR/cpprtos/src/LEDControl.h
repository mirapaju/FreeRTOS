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
#include "events.h"
#include "LED.h"

class Led_control{
public:
    Led_control(QueueHandle_t q, LED *led);
    void led_task();
private:
    static void runner(void *params);
    TaskHandle_t handle;
    QueueHandle_t events;
    const std::string name;
    LED *led;
};

#endif //EXERCISE2_LEDCONTROL_H
