//
// Created by mirap on 9/4/2024.
//

#ifndef EXERCISE1_LED_H
#define EXERCISE1_LED_H

#include "pico/stdlib.h"
#include "pico/time.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include <string>
#include <iostream>
#include <cstdio>

#define DEFAULT_INTERVAL 5

class LedControl {
public:
    LedControl(int gpio);
    static void tmr_callback(TimerHandle_t xTimer);
    void update_interval(uint32_t new_interval);
    void toggle();
    void previous_toggle();
private:
    int pin;
    bool state;
    TimerHandle_t led_timer;
    uint32_t interval;
};

#endif //EXERCISE1_LED_H
