#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "pico/stdlib.h"
#include "Button.h"
#include <iostream>
#include <ctime>
#include "Debug.h"
#include "hardware/timer.h"
extern "C" {
uint32_t read_runtime_ctr(void) {
    return timer_hw->timerawl;
}
}

#define SW_1 8
#define Q_LEN 10
#define Q_SIZE sizeof(debugEvent)


struct Parameters{
    Debug *debug;
    EventGroupHandle_t event;
    int task_number;
};


void task(void *params){
    auto *p = (Parameters *)params;
    EventGroupHandle_t eg = p->event;
    Debug *logger = p->debug;
    int num = p->task_number;

    xEventGroupWaitBits(eg, 1 << 0, pdFALSE, pdTRUE, portMAX_DELAY);
    //printf("Task %d launched\n", num);
    srand(xTaskGetTickCount());
    TickType_t last_print_time = xTaskGetTickCount();

    while(true) {
        TickType_t current_time = xTaskGetTickCount();
        TickType_t elapsed_time = current_time - last_print_time;
        //printf("sending data from task %d at %lu\n", num, elapsed_time);
        logger->debug("Task %d - Elapsed ticks: %lu\n", num, elapsed_time, 0);
        last_print_time = current_time;
        vTaskDelay(pdMS_TO_TICKS(1000 + rand() % 1000));
    }
}

int main(){
    stdio_init_all();

    // srand(time(NULL));
     std::cout << "Starting..." << std::endl;

     EventGroupHandle_t event_group = xEventGroupCreate();
     Button sw1 = Button(SW_1, event_group);
     QueueHandle_t syslog = xQueueCreate(Q_LEN, Q_SIZE);
     Debug logger = Debug(syslog);

     Parameters params_task2{&logger, event_group, 2};
     Parameters params_task3{&logger, event_group, 3};

     xTaskCreate(task, "task_2", 512, &params_task2,
                 tskIDLE_PRIORITY + 2, NULL);
     xTaskCreate(task, "task_3", 512, &params_task3,
                 tskIDLE_PRIORITY + 2, NULL);

    vTaskStartScheduler();

    while(true){};
}
