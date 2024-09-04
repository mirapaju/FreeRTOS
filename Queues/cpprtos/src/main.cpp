#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "pico/stdlib.h"
#include "button.h"
#include "lock.h"
#include <iostream>
#include "LED.h"

#include "hardware/timer.h"
extern "C" {
uint32_t read_runtime_ctr(void) {
    return timer_hw->timerawl;
}
}

#define SW_0 9
#define SW_1 8
#define SW_2 7
#define LED_PIN 20

#define BUTTON_QUEUE_LENGTH 10
#define BUTTON_QUEUE_ITEM_SIZE sizeof(int)


int main(){
    stdio_init_all();

    std::cout << "Starting..." << std::endl;
    //queue for button press events
    QueueHandle_t buttonQueue = xQueueCreate(BUTTON_QUEUE_LENGTH, BUTTON_QUEUE_ITEM_SIZE);


    //sequence and pin-to-sequence mapping
    std::vector<int> sequence = {0, 0, 2, 1, 2};
    std::unordered_map<int, int> pin_mapping = {
            {SW_0, 0},
            {SW_1, 1},
            {SW_2, 2}
    };

    new Button(SW_0, buttonQueue);
    new Button(SW_1, buttonQueue);
    new Button(SW_2, buttonQueue);
    LED led(LED_PIN);
    Lock lock(buttonQueue, &led, pin_mapping, sequence);

    vTaskStartScheduler();

    while(true){};
}
