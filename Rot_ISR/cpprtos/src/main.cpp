#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "pico/stdlib.h"
#include <cstdio>
#include <iostream>
#include "Rot.h"
#include "LEDControl.h"
#include "events.h"
#include "LED.h"
#include "hardware/timer.h"

extern "C" {
uint32_t read_runtime_ctr(void) {
    return timer_hw->timerawl;
}
}


#define QUEUE_LENGHT 50
#define ITEM_SIZE sizeof(RotaryEvents)

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


    QueueHandle_t filtered_events = xQueueCreate(QUEUE_LENGHT, ITEM_SIZE);
    vQueueAddToRegistry(filtered_events, "FilteredEvents");

    RotaryEncoder rot(ROT_SW, ROT_A, ROT_B, filtered_events);
    Led_control ctrl(filtered_events, 21);

    vTaskStartScheduler();

    while(true){};
}
