//
// Created by mirap on 9/10/2024.
//

#include "UartHandler.h"

UartHandler::UartHandler(PicoOsUart *inst, uint8_t *buf, uint8_t buf_size)
    : u_inst(inst), buffer(buf), size(buf_size) {
    std::cout << "Creating read task" << std::endl;
    xTaskCreate(UartHandler::runner, "read_task", 512, (void *) this, tskIDLE_PRIORITY +1, &handle);
}

void UartHandler::runner(void *params){
    auto *instance = static_cast<UartHandler *>(params);
    instance->read_task();
}

void UartHandler::read_task() {
    while (true) {
        if(int count = u_inst->read(buffer, 64, 30); count > 0) {
            u_inst->write(buffer, count);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}