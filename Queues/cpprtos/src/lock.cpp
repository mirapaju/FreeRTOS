//
// Created by mirap on 8/31/2024.
//

#include "lock.h"


Lock::Lock(QueueHandle_t queue, LED *led, const std::unordered_map<int,int> &mapping, const std::vector<int> &seq)
    : lockQ(queue), led(led), pin_to_seq(mapping), sequence(seq), index(0){
    std::cout << "Creating processing task" << std::endl;
    xTaskCreate(Lock::runner, "ProcessingTask", 512, this, tskIDLE_PRIORITY +1, &handle);
}


void Lock::runner(void *params){
    Lock *inst= static_cast<Lock*>(params);
    inst->process_task();
}


void Lock::process_task() {
    int pin;
    while(true){
        if(xQueueReceive(lockQ, &pin, pdMS_TO_TICKS(DELAY)) == pdTRUE){
            auto it = pin_to_seq.find(pin);
            if (it != pin_to_seq.end()){
                int val = it->second;
                std::cout << "Received pin: " << pin << ", mapped value: " << val << std::endl;

                if(val == sequence[index]){
                    ++index;
                    std::cout << "Found one!" << std::endl;
                    if (index == sequence.size()){
                        std::cout << "Correct sequence inserted!" << std::endl;
                        led->blink(DELAY);
                        std::cout << "5sec passed" << std::endl;
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

