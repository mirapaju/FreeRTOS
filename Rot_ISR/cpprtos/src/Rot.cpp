//
// Created by mirap on 9/3/2024.
//

#include "Rot.h"
#include "events.h"


//static queue to pass rotary encoder events in
static QueueHandle_t encoderQ = NULL;


RotaryEncoder::RotaryEncoder(int SW, int ROTA, int ROTB, QueueHandle_t q)
: sw_pin(SW), rotA_pin(ROTA), rotB_pin(ROTB), filtered_events(q), previous_push(0){
    init();
    //priority pitä olla idle +3/4 ettei normitaskit pääse tän ohi, gpio pitää päästä lukee mahdollisimman nopeesti
    xTaskCreate(RotaryEncoder::runner, "debounce_task", 512, (void *) this, tskIDLE_PRIORITY +3, &handle);
}

void RotaryEncoder::init() const{
    gpio_init(sw_pin);
    gpio_set_dir(sw_pin, GPIO_IN);
    gpio_set_pulls(sw_pin, true, false);

    gpio_init(rotA_pin);
    gpio_set_dir(rotA_pin, GPIO_IN);

    gpio_init(rotB_pin);
    gpio_set_dir(rotB_pin, GPIO_IN);

    encoderQ = xQueueCreate(50, sizeof(uint32_t));
    vQueueAddToRegistry(encoderQ, "RotaryEncoderEvents");
    gpio_set_irq_enabled_with_callback(sw_pin, GPIO_IRQ_EDGE_RISE, true, irq_handler);
    gpio_set_irq_enabled_with_callback(rotA_pin, GPIO_IRQ_EDGE_RISE, true, irq_handler);
}


void RotaryEncoder::runner(void *params){
    auto *instance = static_cast<RotaryEncoder *>(params);
    instance->debounce_task();
}

//interrupt handler, send pin to queue
void RotaryEncoder::irq_handler(uint gpio, uint32_t events) {
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(encoderQ, &gpio, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}


//RECEIVE FROM ISR
//MUISTA KATTOA TARKKAAN MINNE TULEE DELAY ->tänne ei ollenkaan koska pitää käsitellä heti
void RotaryEncoder::debounce_task() {
    int gpio;
    while (true) {
        if (xQueueReceive(encoderQ, &gpio, portMAX_DELAY == pdTRUE)) {
            uint32_t current = to_ms_since_boot(get_absolute_time());
            RotaryEvents event;

            int rotA_state = gpio_get(rotA_pin);
            int rotB_state = gpio_get(rotB_pin);

            if (gpio==sw_pin){
                if ((current - previous_push > DELAY)){ //debounce push button
                    previous_push = current;
                    event.event_type = BUTTON_PRESS;
                    xQueueSend(filtered_events, &event, portMAX_DELAY);
                }
            }
            else {
                //determine the direction based on the state of rotA and rotB
                if (rotA_state != rotB_state) event.event_type = CLOCKWISE;
                else event.event_type = COUNTERCLOCKWISE;
                xQueueSend(filtered_events, &event, portMAX_DELAY);
            }
        }
    }
}


