//
// Created by mirap on 8/31/2024.
//

#ifndef RP2040_FREERTOS_CPP_TEMPLATE_LOCK_H
#define RP2040_FREERTOS_CPP_TEMPLATE_LOCK_H

#include "pico/stdlib.h"
#include "pico/time.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

#define LED_PIN 21
#include "LED.h"

class Lock{
public:
    Lock(QueueHandle_t queue, LED *led, const std::unordered_map<int,int> &mapping, const std::vector<int> &seq);
    void process_task();
private:
    static void runner(void *params);
    QueueHandle_t lockQ;
    LED *led;
    std::vector<int> sequence;
    std::unordered_map<int,int> pin_to_seq;
    int index;

    TaskHandle_t handle;
};

#endif //RP2040_FREERTOS_CPP_TEMPLATE_LOCK_H
