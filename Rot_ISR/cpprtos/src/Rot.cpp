//
// Created by mirap on 9/3/2024.
//

#include "Rot.h"

RotaryEncoder *RotaryEncoder::inst = nullptr;

RotaryEncoder::RotaryEncoder(int SW, int ROTA, int ROTB, QueueHandle_t queue1, QueueHandle_t queue2)
: sw_pin(SW), rotA_pin(ROTA), rotB_pin(ROTB), raw_events(queue1), filtered(queue2){
    init();
    inst = this;
    xTaskCreate(RotaryEncoder::runner, "fix this", 512, (void *) this, tskIDLE_PRIORITY +1, &handle);
}

void RotaryEncoder::init() const{
    gpio_init(sw_pin);
    gpio_set_dir(sw_pin, GPIO_IN);
    gpio_set_pulls(sw_pin, true, false);

    gpio_init(rotA_pin);
    gpio_set_dir(rotA_pin, GPIO_IN);
    gpio_set_pulls(rotA_pin, true, false);

    gpio_init(rotB_pin);
    gpio_set_dir(rotB_pin, GPIO_IN);
    gpio_set_pulls(rotB_pin, true, false);

    gpio_set_irq_enabled_with_callback(sw_pin, GPIO_IRQ_EDGE_RISE, true, irq_handler);
    gpio_set_irq_enabled_with_callback(rotA_pin, GPIO_IRQ_EDGE_RISE, true, irq_handler);
    gpio_set_irq_enabled_with_callback(rotB_pin, GPIO_IRQ_EDGE_RISE, true, irq_handler);
}


void RotaryEncoder::runner(void *params){
    auto *instance = static_cast<RotaryEncoder *>(params);
    instance ->filter_task();
}


void RotaryEncoder::irq_handler(uint gpio, uint32_t events) {
    std::cout << "in here" << std::endl;
    if (inst){
        BaseType_t higherPriorityTaskWoken = pdFALSE;
        xQueueSendFromISR(inst->raw_events, &gpio, &higherPriorityTaskWoken);
        portYIELD_FROM_ISR(higherPriorityTaskWoken);
    }
}



//RECEIVE FROM ISR
void RotaryEncoder::filter_task() {
    while (true) {
        int gpio;
        uint32_t current = to_ms_since_boot(get_absolute_time());
        if (xQueueReceive(raw_events, &gpio, portMAX_DELAY == pdTRUE)) {
            std::cout << "Received" << std::endl;
            //odotellaanko indefinitely?
            if ((current - last_event) > DELAY) { //aikaa kulunu tarpeeks
                last_event = current; //resettaa timer
                std::cout << "Filtered" << std::endl;
                xQueueSend(filtered, &gpio, portMAX_DELAY);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

