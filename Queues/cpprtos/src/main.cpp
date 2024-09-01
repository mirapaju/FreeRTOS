#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "pico/stdlib.h"
#include "button.h"
#include "lock.h"
#include <iostream>

#include "hardware/timer.h"
extern "C" {
uint32_t read_runtime_ctr(void) {
    return timer_hw->timerawl;
}
}

#define SW_0 9
#define SW_1 8
#define SW_2 7
#define LED_PIN 20

#define BUTTON_QUEUE_LENGTH 10
#define BUTTON_QUEUE_ITEM_SIZE sizeof(int)

//struct to hold the Button class instance & queue handle
struct btn_params{
    Button *button;
    QueueHandle_t queue;
};

void button_task(void *params){
    auto *button_params = static_cast<btn_params *>(params); //cast the parameters back to the correct type, use auto
    Button *button = button_params->button; //extract the Button pointer from the struct
    QueueHandle_t queue = button_params->queue; //extract the queue from struct

    while (true){
        //if button is pressed
        if (button-> get()){
            int pin = button->get_pin(); //get the button pin from class
            //send the pin number to queue
            //portMAX_DELAY max amount pf time task can be blocked or delayed,
            //it will wait indefinitely until requested action can be performed
            //-> xQueueSend will block the task until data is send to queue, with no timeout CHANGE
            xQueueSend(queue, &pin, portMAX_DELAY);
        }
        //pauses the task so that others can run
        //tick is the smallest unit of time in freertos and defined by system tick rate (configured in FreeRTOSConfig.h)
        //pDMS_TO_TICKS(ms) converts ms to ticks
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}


void processing_task(void *params) {
    auto buttonQueue = static_cast<QueueHandle_t>(params); // Cast the parameter back to the expected type

    while(true) {
        int button=0;
        // Initialize LED pin
        gpio_init(LED_PIN);
        gpio_set_dir(LED_PIN, GPIO_OUT);
        if (xQueueReceive(buttonQueue, &button, pdMS_TO_TICKS(5000)) == pdTRUE) {
            if (button==SW_0)
            std::cout << "Button pressed" << std::endl;
            gpio_put(LED_PIN, true);
          //  vTaskDelay(pdMS_TO_TICKS(100));
          //  gpio_put(LED_PIN, false);
            vTaskDelay(pdMS_TO_TICKS(100));
        } else {
            std::cout << "Something is wrong" << std::endl;
        }
    }
}
/*
int main()
{
    std::cout << "Starting..." << std::endl;
    // Create the queue for button press events
    QueueHandle_t buttonQueue = xQueueCreate(BUTTON_QUEUE_LENGTH, BUTTON_QUEUE_ITEM_SIZE);
    if (buttonQueue == NULL) {
        // Handle queue creation failure
        while (1); // Enter an infinite loop to halt execution
    }

    static Button btn1(SW_0);
    static Button btn2(SW_1);
    static Button btn3(SW_2);

    // Prepare parameters for the button task
    static btn_params btn1_params = { &btn1, buttonQueue };
    static btn_params btn2_params = { &btn2, buttonQueue };
    static btn_params btn3_params = { &btn3, buttonQueue };

    xTaskCreate(button_task, "ButtonTask", 512, (void *) &btn1_params, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(button_task, "ButtonTask", 512, (void *) &btn2_params, tskIDLE_PRIORITY +1, NULL);
    xTaskCreate(button_task, "ButtonTask", 512, (void *) &btn3_params, tskIDLE_PRIORITY +1, NULL);
    // Create the processing task
    xTaskCreate(processing_task, "ProcessingTask", 512, (void *) buttonQueue, tskIDLE_PRIORITY + 1, NULL);

    vTaskStartScheduler();

}
 */

int main(){
    stdio_init_all();

    std::cout << "Starting..." << std::endl;
    // Create the queue for button press events
    QueueHandle_t buttonQueue = xQueueCreate(BUTTON_QUEUE_LENGTH, BUTTON_QUEUE_ITEM_SIZE);
    if (buttonQueue == NULL) {
        // Handle queue creation failure
        while (1); // Enter an infinite loop to halt execution
    }

    // Define the sequence and pin-to-sequence mapping
    std::vector<int> sequence = {0, 0, 2, 1, 2}; // Example sequence
    std::unordered_map<int, int> pin_mapping = {
            {SW_0, 0}, // Map pin 7 to sequence value 0
            {SW_1, 1}, // Map pin 8 to sequence value 1
            {SW_2, 2}  // Map pin 9 to sequence value 2
    };

    new Button(SW_0, buttonQueue);  // Create a Button object for pin 9
    new Button(SW_1, buttonQueue); // Create another Button object for pin 10
    new Button(SW_2, buttonQueue); // And another for pin 11
    Lock lock(buttonQueue, pin_mapping, sequence);

    vTaskStartScheduler();

    while(true){};
}
