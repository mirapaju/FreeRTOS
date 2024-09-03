#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "pico/stdlib.h"
#include <cstdio>
#include <iostream>
#include "Rot.h"
#include "LEDControl.h"
#include "hardware/timer.h"
extern "C" {
uint32_t read_runtime_ctr(void) {
    return timer_hw->timerawl;
}
}

#define LED_PIN 21
#define ROT_A 10
#define ROT_B 11
#define ROT_SW 12
#define QUEUE_LENGHT 50
#define ITEM_SIZE sizeof(int)

//Switching LED on/off and changing the blinking frequency
//Rot_SW is on/off button, when pushed, led state is toggled, presses <250ms ignored
//Encoder used to control blinking frequency:
//-->turn clockwise: increase frequency (max 200 Hz)
//-->turn counterclockwise: decrease frequency (min 2Hz)
//Print frequency change!
//When LED toggled ON, use same freq as was when turned off
//Use interrupts to button, send events to queue
//task 1: receive and filter gpio events
//task2: blink led


using namespace std;


int main(){
    stdio_init_all();
   // cout << "Hoping everything works.." << endl;

    QueueHandle_t e = xQueueCreate(QUEUE_LENGHT, ITEM_SIZE);
    QueueHandle_t ev = xQueueCreate(QUEUE_LENGHT, ITEM_SIZE);

    RotaryEncoder rot(ROT_SW, ROT_A, ROT_B, e, ev);
    Led_control led(LED_PIN, e);
    //create tasks, no parameters so passing a nullptr
   // xTaskCreate(read_task, "ReadTask", 512, nullptr, tskIDLE_PRIORITY + 1, nullptr);
   // xTaskCreate(indicator_task, "IndicatorTask", 512, nullptr, tskIDLE_PRIORITY +1, nullptr);

    vTaskStartScheduler();

    while(true){};
}
