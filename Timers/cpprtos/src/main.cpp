#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "pico/stdlib.h"
#include "PicoOsUart.h"
#include <iostream>
#include "LedControl.h"
#include "UartHandler.h"
#include <cstdio>
#include "CommandProcess.h"

#include "hardware/timer.h"
extern "C" {
uint32_t read_runtime_ctr(void) {
    return timer_hw->timerawl;
}
}

#define LED_PIN 21

#define UART 0
#define UART_TX 0
#define UART_RX 1
#define BAUD_RATE 115200

#define BUFFER_SIZE 64
#define QUEUE_LENGTH 10
#define ITEM_SIZE sizeof(char[64]) // Assuming the maximum command length is 64 bytes



int main(){
    stdio_init_all();

    PicoOsUart u(UART, UART_TX, UART_RX, BAUD_RATE); //PITÄÄKÖ TÄÄ TEHÄ SIINÄ TASKIN ALUSSA?
    QueueHandle_t q = xQueueCreate(QUEUE_LENGTH, ITEM_SIZE);

    uint8_t buf[BUFFER_SIZE];
    printf("Works?");
    UartHandler uart(&u, buf, BUFFER_SIZE, q);
    LedControl lc(LED_PIN);
    CommandProcess cmd(q, &lc);

    vTaskStartScheduler();

    while(true){};

}
