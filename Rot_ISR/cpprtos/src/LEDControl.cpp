//
// Created by mirap on 9/3/2024.
//

#include "LEDControl.h"

Led_control::Led_control(QueueHandle_t q, LED *led)
            : events(q), led(led){
    std::cout << "Creating led control task" << std::endl;
    xTaskCreate(Led_control::runner, "LedControl", 512, (void*) this, tskIDLE_PRIORITY +1, &handle);
}


void Led_control::runner(void *params){
    auto *inst = static_cast<Led_control *>(params);
    inst ->led_task();
}


void Led_control::led_task() {
    RotaryEvents event;  //ONKO TÄÄ PAREMPI TEHÄ JOKA KERTA UUS? EIKS NIIT TUU KAUHEEST
    while (true) {
        if (xQueueReceive(events, &event, portMAX_DELAY == pdTRUE)) {
            std::cout << "Received event from somewhere over the rainbow" << std::endl;

            switch (event.event_type) {
                case BUTTON_PRESS:
                    std::cout << "Button pressed" << std::endl;
                    led->toggle();
                    break;
                case CLOCKWISE:
                    std::cout << "Turned clockwise" << std::endl;
                    break;
                case COUNTERCLOCKWISE:
                    std::cout << "Turned counterclockwise" << std::endl;
                    break;
            }
         /*   if (event.pressed){
                //toggle led
                std::cout << "Received info from press" << std::endl;
            }
            else if(event.direction !=0){
                //frequency change
                std::cout << "Adjust frequency" << std::endl;
            }*/
        }
    }
}
