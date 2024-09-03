//
// Created by mirap on 9/3/2024.
//

#ifndef EXERCISE2_ROT_H
#define EXERCISE2_ROT_H

#include "pico/stdlib.h"
#include "pico/time.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <string>
#include <iostream>

#define DELAY 250


class RotaryEncoder{
public:
    RotaryEncoder(int sw, int rota, int rotb, QueueHandle_t queue1, QueueHandle_t queue2);
    void init() const;
    static void irq_handler(uint gpio, uint32_t events);
    void filter_task();
private:
    static void runner(void *params);
    static RotaryEncoder *inst;
    int sw_pin;
    int rotA_pin;
    int rotB_pin;
    QueueHandle_t raw_events;
    QueueHandle_t filtered;
    TaskHandle_t handle;
    uint32_t last_event=0;
};

#endif //EXERCISE2_ROT_H
