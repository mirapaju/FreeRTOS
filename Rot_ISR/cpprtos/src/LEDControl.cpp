//
// Created by mirap on 9/3/2024.
//

#include "LEDControl.h"

SemaphoreHandle_t xMutex;



Led_control::Led_control(QueueHandle_t q, LED *led)
            : events(q), led(led), state(false), frequency(MIN_FREQUENCY){
    std::cout << "Creating tasks" << std::endl;
    xTaskCreate(Led_control::filter_runner, "LedControl", 512, (void*) this, tskIDLE_PRIORITY +1, &filter_handle);
    xTaskCreate(Led_control::blink_runner, "BlinkTask", 512, (void*) this, tskIDLE_PRIORITY + 1, &blink_handle);
    semaphore = xSemaphoreCreateBinary();
    // Initialize mutex
    xMutex = xSemaphoreCreateMutex();
}


void Led_control::filter_runner(void *params){
    auto *inst = static_cast<Led_control *>(params);
    inst ->filter_task();
}

void Led_control::blink_runner(void *params){
    auto *inst = static_cast<Led_control *>(params);
    inst ->blink_task();
}


//min 2Hz max 200Hz
void Led_control::filter_task() {
    RotaryEvents event;
    while (true) {
        if (xQueueReceive(events, &event, portMAX_DELAY == pdTRUE)) {
          //  xSemaphoreTake(xMutex, portMAX_DELAY);
          //  std::cout << "Current period: " << (1000 / frequency) / 2 << std::endl;
                switch (event.event_type) {
                    case BUTTON_PRESS:
                        state = !state;
                        led->toggle();
                        break;
                    case CLOCKWISE:
                        std::cout << "Turned clockwise" << std::endl;
                        if (frequency >= MIN_FREQUENCY && frequency < MAX_FREQUENCY &&state) {
                            ++frequency;
                            std::cout << "New frequency: " << (1000 / frequency) / 2 << std::endl;
                        }
                        break;
                    case COUNTERCLOCKWISE:
                        std::cout << "Turned counterclockwise" << std::endl;
                        if (frequency > MIN_FREQUENCY && frequency <= MAX_FREQUENCY &&state) {
                            --frequency;
                            std::cout << "New frequency: " << (1000 / frequency) / 2 << std::endl;
                        }
                        break;
            }
          //  xSemaphoreGive(xMutex);  // Release the mutex
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void Led_control::blink_task() {
    while (true) {
        if (state) {
            xSemaphoreTake(xMutex, portMAX_DELAY);
            int period_ms = (1000 / frequency) / 2;
            //printf("delay : %d \n", period_ms);
            led->blink(period_ms);
            xSemaphoreGive(xMutex);  // Release the mutex
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
