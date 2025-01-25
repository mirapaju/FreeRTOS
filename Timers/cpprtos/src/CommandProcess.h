//
// Created by mirap on 9/12/2024.
//

#ifndef EXERCISE3_COMMANDPROCESS_H
#define EXERCISE3_COMMANDPROCESS_H

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include <iostream>
#include "semphr.h"
#include <cstring>
#include <sstream>
#include "LedControl.h"

#define BUFFER_SIZE 64

class CommandProcess{
public:
    CommandProcess(QueueHandle_t q, LedControl *lc);
    void process_task();
    static void print_instructions();
private:
    static void runner(void *params);
    QueueHandle_t queue;
    TaskHandle_t handle;
    LedControl *led_inst;
};

#endif //EXERCISE3_COMMANDPROCESS_H
