#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "pico/stdlib.h"
#include "Button.h"
#include <iostream>
#include <ctime>
#include "Debug.h"
#include "Watchdog.h"
#include "hardware/timer.h"
extern "C" {
uint32_t read_runtime_ctr(void) {
    return timer_hw->timerawl;
}
}

#define SW_0 9
#define SW_1 8
#define SW_2 7

#define Q_LEN 10
#define Q_SIZE sizeof(debugEvent)


int main(){
    stdio_init_all();
    srand(xTaskGetTickCount());

     std::cout << "Starting..." << std::endl;

     EventGroupHandle_t event_group = xEventGroupCreate();

     QueueHandle_t syslog = xQueueCreate(Q_LEN, Q_SIZE); //queue for debugger class


    Debug logger = Debug(syslog);
    Watchdog wd = Watchdog(event_group, logger);

     Button sw1 = Button(SW_0, event_group, &logger);
    Button sw2 = Button(SW_1, event_group, &logger);
    Button sw3 = Button(SW_2, event_group, &logger);

    vTaskStartScheduler();

    while(true){};
}
