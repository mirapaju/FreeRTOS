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

#define LED_PIN 21
#define ROT_A 10
#define ROT_B 11
#define ROT_SW 12
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

    QueueHandle_t rotaryQ = xQueueCreate(QUEUE_LENGHT, ITEM_SIZE);

    LED led(LED_PIN);
    RotaryEncoder rot(ROT_SW, ROT_A, ROT_B, rotaryQ);
    Led_control ctrl(rotaryQ, &led);

    vTaskStartScheduler();

    while(true){};
}
