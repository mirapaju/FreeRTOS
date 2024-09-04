#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "pico/stdlib.h"
#include <cstdio>
#include <iostream>
#include "hardware/timer.h"
extern "C" {
uint32_t read_runtime_ctr(void) {
    return timer_hw->timerawl;
}
}

#define LED_PIN 21

//Two tasks, one for reading character from serial port,
//other for indicating received characters on the serial port
//BINARY SEMAPHORE notifies serial port activity to the indicator
//blink after last char

using namespace std;

SemaphoreHandle_t smphr;  //binary semaphore handle

//task 1: read characters from serial port and signal when a line is complete (is the EOL ok?)
void read_task(void *param){
    while(true){
        if(int ch = getchar_timeout_us(0); ch != PICO_ERROR_TIMEOUT){
            putchar(ch);
            //if charachter is EOL, signal the indicator task
            if(ch == '\n') xSemaphoreGive(smphr);
        }
        vTaskDelay(pdMS_TO_TICKS(10)); //kauan saa olla pois?
    }
}


//task2: blinks the LED after last character is received
void indicator_task(void *param){
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, true);

    while(true) {
        //wait indefinitely for the semaphore to be given by the read task
        if (xSemaphoreTake(smphr, portMAX_DELAY) == pdTRUE) {
            cout << "Semaphore is miiiiiinneeeeeee" << endl;
            gpio_put(LED_PIN, true);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_put(LED_PIN, false);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

int main(){
    stdio_init_all();
    cout << "Hoping everything works.." << endl;

    //create binary semaphore
    smphr = xSemaphoreCreateBinary();

    //create tasks, no parameters so passing a nullptr
    xTaskCreate(read_task, "ReadTask", 512, nullptr, tskIDLE_PRIORITY + 1, nullptr);
    xTaskCreate(indicator_task, "IndicatorTask", 512, nullptr, tskIDLE_PRIORITY +1, nullptr);

    vTaskStartScheduler();

    while(true){};
}
