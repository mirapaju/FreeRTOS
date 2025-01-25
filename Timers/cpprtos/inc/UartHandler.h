//
// Created by mirap on 9/10/2024.
//

#ifndef EXERCISE3_UARTHANDLER_H
#define EXERCISE3_UARTHANDLER_H

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "PicoOsUart.h"
#include <iostream>
#include "semphr.h"
#include <cstring>

//timer callback (inactive)
#define TMR_PERIOD 10000

class UartHandler{
public:
    UartHandler(PicoOsUart *inst, uint8_t *buf, uint8_t buf_size, QueueHandle_t q);
    void read_task();
    static void timer_callback(TimerHandle_t xTimer);
    void print(uint8_t *buff);
private:
    static void read_runner(void *params);
    PicoOsUart *u_inst;
    TaskHandle_t r_handle;
    TimerHandle_t inactivity_timer;
    uint8_t *buffer;
    int size;
    QueueHandle_t queue;
};

#endif //EXERCISE3_UARTHANDLER_H
