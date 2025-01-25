//
// Created by mirap on 9/12/2024.
//

#include "CommandProcess.h"


CommandProcess::CommandProcess(QueueHandle_t q, LedControl *lc) : queue(q), led_inst(lc){
    xTaskCreate(CommandProcess::runner,"process_task",512,
                (void *) this,tskIDLE_PRIORITY +1,&handle);
}


void CommandProcess::runner(void *params){
    auto instance = static_cast<CommandProcess *>(params);
    instance->process_task();
}


//processes the command received from the UartHandler read task
void CommandProcess::process_task(){
    char buffer[BUFFER_SIZE];

    while (true) {
        if (xQueueReceive(queue, buffer, portMAX_DELAY)){
            //create a string stream from the received buffer for parsing
            std::istringstream iss(buffer);
            std::string cmd;
            // extract first word
            iss >> cmd;

            if (cmd == "help") print_instructions();
            else if (cmd == "time") led_inst->previous_toggle(); //print time of previous toggle
            else if (cmd == "interval") {
                uint32_t new_interval; //if interval successfully extracted, update it
                if (iss >> new_interval) led_inst->update_interval(new_interval);
                else printf("Invalid interval value. Type \"help\" to get the usage instructions.\n");
            }
            else std::cout << "Unknown command. Type \"help\" to get the usage instructions.\n";
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


void CommandProcess::print_instructions() {
    std::cout << "Usage instructions:" << std::endl;
    std::cout << "  help - display usage instructions" << std::endl;
    std::cout << "  interval <number> - set the LED toggle interval (default is 5 seconds)" << std::endl;
    std::cout << "  time - print the number of seconds with 0.1s accuracy since the last LED toggle" << std::endl;
}




