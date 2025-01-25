//
// Created by mirap on 9/10/2024.
//

#ifndef EXERCISE1_DEBUG_H
#define EXERCISE1_DEBUG_H

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include <iostream>
#include <cstring>


#define SIZE 64

struct debugEvent {
    uint32_t timestamp;
    const char *format;  // Format string for the message
    uint32_t data[3];    // Data to be formatted into the message
};

class Debug{
public:
    Debug(QueueHandle_t q);
    void debug_task();
    void debug(const char *format, uint32_t d1, uint32_t d2, uint32_t d3);
private:
    static void runner(void *params);
    QueueHandle_t syslog_q;
    TaskHandle_t handle;
};

#endif //EXERCISE1_DEBUG_H
