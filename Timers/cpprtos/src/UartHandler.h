//
// Created by mirap on 9/10/2024.
//

#ifndef EXERCISE3_UARTHANDLER_H
#define EXERCISE3_UARTHANDLER_H

#include "PicoOsUart.h"
#include <iostream>

class UartHandler{
public:
    UartHandler(PicoOsUart *inst, uint8_t *buf, uint8_t buf_size);
    void read_task();
private:
    static void runner(void *params);
    PicoOsUart *u_inst;
    TaskHandle_t handle;
    uint8_t *buffer; // Buffer for storing incoming data
    size_t size; // Index for the buffer
};

#endif //EXERCISE3_UARTHANDLER_H
