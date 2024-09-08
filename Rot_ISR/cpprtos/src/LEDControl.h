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
#include "semphr.h"


#define MAX_FREQUENCY 200
#define MIN_FREQUENCY 2


class Led_control{
public:
    Led_control(QueueHandle_t q, LED *led);
    void filter_task();
    void blink_task();
private:
    static void filter_runner(void *params);
    static void blink_runner(void *params);
    TaskHandle_t filter_handle;
    TaskHandle_t blink_handle;
    QueueHandle_t events;
    SemaphoreHandle_t semaphore;
    const std::string name;
    LED *led;
    bool state;
    int frequency;
};

#endif //EXERCISE2_LEDCONTROL_H
