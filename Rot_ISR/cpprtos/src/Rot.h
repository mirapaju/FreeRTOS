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
#include "events.h"
#include <string>
#include <iostream>

#define DELAY 250


class RotaryEncoder{
public:
    RotaryEncoder(int sw, int rota, int rotb, QueueHandle_t q);
    void init() const;
    static void irq_handler(uint gpio, uint32_t events);
    void debounce_task();
private:
    static void runner(void *params);
    int sw_pin;
    int rotA_pin;
    int rotB_pin;
    QueueHandle_t filtered;
    TaskHandle_t handle;
    uint32_t last_event;
    RotaryEvents events;
};

#endif //EXERCISE2_ROT_H
