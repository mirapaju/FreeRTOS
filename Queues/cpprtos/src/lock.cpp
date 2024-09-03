//
// Created by mirap on 8/31/2024.
//

#include "lock.h"

Lock::Lock(QueueHandle_t queue, const std::unordered_map<int,int> &mapping, const std::vector<int> &seq)
    : lockQ(queue), pin_to_seq(mapping), sequence(seq), index(0){
    std::cout << "Creating processing task" << std::endl;
    xTaskCreate(Lock::runner, "ProcessingTask", 512, this, tskIDLE_PRIORITY +2, &handle);
}

void Lock::runner(void *params){
    Lock *inst= static_cast<Lock*>(params);
    inst->process_task();
}

void Lock::process_task() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    int pin;
    while(true){
        if(xQueueReceive(lockQ, &pin, pdMS_TO_TICKS(5000)) == pdTRUE){
            auto it = pin_to_seq.find(pin);
            if (it != pin_to_seq.end()){
                int val = it->second;
                std::cout << "Received pin: " << pin << ", mapped value: " << val << std::endl;

                if(val == sequence[index]){
                    ++index;
                    std::cout << "Found one!" << std::endl;
                    if (index == sequence.size()){
                        std::cout << "Correct sequence inserted!" << std::endl;
                        gpio_put(LED_PIN, true);
                        vTaskDelay(pdMS_TO_TICKS(5000)); //keep led on for 5sec
                        gpio_put(LED_PIN, false);
                        index=0;
                    }
                }
                else{
                    std::cout << "Mismatch!" << std::endl;
                    index=0;
                }
            }
        }
        else{
            std::cout << "No value received on timeout" << std::endl;
            index=0;
        }
    }
}

