//
// Created by mirap on 9/10/2024.
//

#include "UartHandler.h"


#define BUFFER_SIZE 64

static SemaphoreHandle_t inactivity = nullptr;

UartHandler::UartHandler(PicoOsUart *inst, uint8_t *buf, uint8_t buf_size, QueueHandle_t q)
    : u_inst(inst), buffer(buf), size(buf_size), queue(q) {
    std::cout << "Creating read task" << std::endl;
    xTaskCreate(UartHandler::read_runner,"read_task",512,
                (void *) this,tskIDLE_PRIORITY +2,&r_handle);
    inactivity_timer =  xTimerCreate("inactivity_timer",
                                     pdMS_TO_TICKS(TMR_PERIOD),pdTRUE,(void *) 0,timer_callback);
    inactivity = xSemaphoreCreateBinary();
}

void UartHandler::read_runner(void *params){
    auto *instance = static_cast<UartHandler *>(params);
    instance->read_task();
}


void UartHandler::read_task() {
    xTimerStart(inactivity_timer, 0); //start the timer when the task starts
    uint8_t buf[64];
    std::string str;

    while (true) {
        if (int count = u_inst->read(buf, size-1, 30); count > 0) {
            //print(buf);
            xTimerReset(inactivity_timer, 0); //reset the timer after user input
            buf[count] =0;
            str.append((const char*)buf, count); //append the buffer contents to string
            std::cout <<  str << std::endl;

            //if contains linefeed, send the string to queue + erase the contents
            if (str.find_first_of("\n\r") != std::string::npos) {
                xQueueSend(queue, str.c_str(), portMAX_DELAY);
                str.clear();
            }
        }
        //if inactivity semaphore received, clear the string
        else if(xSemaphoreTake(inactivity, 0) == pdTRUE){
            str.clear();   //clear the string in case there is characters
            printf("[Inactive]\n");
        }
    }
}

//when inactivity timer expires, give the inactivity semaphore
void UartHandler::timer_callback(TimerHandle_t xTimer) {
    xSemaphoreGive(inactivity);
}

//for debugging
void UartHandler::print(uint8_t *buff) {
    printf("Local buf contents: ");
    for (size_t i = 0; i < size; i++) {
        if (buff[i] == '\n') {
            std::cout << "\\n";
        } else if (buff[i] == '\r') {
            std::cout << "\\r";
        } else {
            std::cout << buff[i];
        }
    }

    std::cout << std::endl;
}

