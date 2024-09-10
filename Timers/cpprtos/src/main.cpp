#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "pico/stdlib.h"
#include "PicoOsUart.h"
#include <iostream>
#include "LED.h"
#include "UartHandler.h"

#include "hardware/timer.h"
extern "C" {
uint32_t read_runtime_ctr(void) {
    return timer_hw->timerawl;
}
}

#define LED_PIN 20

#define UART 0
#define UART_TX 0
#define UART_RX 1
#define BAUD_RATE 115200

#define BUFFER_SIZE 64


//two timers, one for toggling led, one for clearing the input
// if you are inactive for too long everything is discarded what you have entered
//you can change the toggling interval with commands
//getting the number of seconds since last toggle w commands

//learn from timer api
//you can get interval, you can get how much time since last trigger/how much time left until next
//you need to handle line feeds etc, uart driver gives just bytes, doesnt know \n, backspace, etc
//they will appear as garbage ->handle


//one task for reading char, one task for processing command, one tasl for


int main(){
    stdio_init_all();

    std::cout << "Starting..." << std::endl;
    PicoOsUart u(UART, UART_TX, UART_RX, BAUD_RATE);
    uint8_t buf[BUFFER_SIZE];
    UartHandler uart(&u, buf, BUFFER_SIZE);

    vTaskStartScheduler();

    while(true){};

}
