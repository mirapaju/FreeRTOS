//
// Created by mirap on 9/19/2024.
//

#ifndef EXERCISE3_WATCHDOG_H
#define EXERCISE3_WATCHDOG_H

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "Debug.h"
#include <iostream>

#define TASK_COUNT 3
#define TIMEOUT 10000

class Watchdog {
public:
    Watchdog(EventGroupHandle_t eg, Debug d);
    void watchdog_task();
private:
    static void runner(void *params);
    EventGroupHandle_t event_group;
    TaskHandle_t handle;
    Debug logger;
};


#endif //EXERCISE3_WATCHDOG_H
