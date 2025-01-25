//
// Created by mirap on 9/3/2024.
//

#include "LEDControl.h"


Led_control::Led_control(QueueHandle_t q, int gpio )
            : events(q), pin(gpio), state(false), frequency(MIN_FREQUENCY){
    std::cout << "Creating tasks" << std::endl;
    xTaskCreate(Led_control::filter_runner, "LedControl", 512, (void*) this, tskIDLE_PRIORITY +1, &filter_handle);
    xTaskCreate(Led_control::blink_runner, "BlinkTask", 512, (void*) this, tskIDLE_PRIORITY + 1, &blink_handle);

    mutex = xSemaphoreCreateMutex();
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
}


void Led_control::filter_runner(void *params){
    auto *inst = static_cast<Led_control *>(params);
    inst->events_task();
}

void Led_control::blink_runner(void *params){
    auto *inst = static_cast<Led_control *>(params);
    inst ->blink_task();
}


//receive rotary encoder events
//based on the event, determine the actions to take
void Led_control::events_task() {
    RotaryEvents event;
    while (true) {
        if (xQueueReceive(events, &event, portMAX_DELAY == pdTRUE)) {
            xSemaphoreTake(mutex, portMAX_DELAY);
                switch (event.event_type) {
                    case BUTTON_PRESS: //toggle led if the event is button push
                        state = !state;
                        gpio_put(pin, state);
                        break;
                    case CLOCKWISE: //increase frequency if rotary encoder was turned clockwise & state is on
                        if (state && frequency >= MIN_FREQUENCY && frequency < MAX_FREQUENCY) {
                            ++frequency;
                            std::cout << "New frequency: " << frequency << std::endl;
                        }
                        break;
                    case COUNTERCLOCKWISE: //decrease frequency if rotary encoder was turned counterclockwise & state is on
                        if (state && frequency > MIN_FREQUENCY && frequency <= MAX_FREQUENCY) {
                            --frequency;
                            std::cout << "New frequency: " << frequency << std::endl;
                        }
                        break;
            }
            xSemaphoreGive(mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


//if led state is on, blink according to the current frequency
void Led_control::blink_task() {

    while (true) {
        xSemaphoreTake(mutex, portMAX_DELAY);
        bool on = state;
        int period_ms = (1000 / frequency) / 2;
        xSemaphoreGive(mutex);

        if (on){
            gpio_put(pin, true);
            vTaskDelay(pdMS_TO_TICKS(period_ms));
            gpio_put(pin, false);
            vTaskDelay(pdMS_TO_TICKS(period_ms));
        }
        else{
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}
