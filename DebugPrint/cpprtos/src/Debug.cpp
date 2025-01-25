//
// Created by mirap on 9/10/2024.
//

#include "Debug.h"

Debug::Debug(QueueHandle_t q) : syslog_q(q){
    printf("Creating debug task..\n");
    xTaskCreate(Debug::runner, "Debug_task", 512, (void *) this, tskIDLE_PRIORITY+1, &handle);
}

void Debug::runner(void *params){
    auto inst = static_cast<Debug *>(params);
    inst->debug_task();
}

void Debug::debug(const char *format, uint32_t d1, uint32_t d2, uint32_t d3){
    uint32_t timestamp_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;

    debugEvent e;
    e.timestamp = timestamp_ms;
    e.format = format;
    e.data[0] = d1;
    e.data[1] = d2;
    e.data[2] = d3;

    //send timestamp and event to the queue
    xQueueSend(syslog_q, &e, portMAX_DELAY);
}

void Debug::debug_task() {
    char buffer[SIZE];
    debugEvent e;

    while(true) {
        if (xQueueReceive(syslog_q, &e, portMAX_DELAY) == pdTRUE) {
            //printf("received\n");
            //format message with the provided format string and data
            snprintf(buffer, SIZE, "%lu: ", e.timestamp); //voiko tän vaan erikseen laittaa eteen?
            snprintf(buffer + strlen(buffer), SIZE - strlen(buffer), e.format, e.data[0], e.data[1], e.data[2]);

            printf("%s", buffer);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}