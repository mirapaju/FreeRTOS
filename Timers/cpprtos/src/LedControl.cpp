//
// Created by mirap on 9/4/2024.
//

#include "LedControl.h"

LedControl::LedControl(int gpio) : pin(gpio), state(false), interval(DEFAULT_INTERVAL){
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    led_timer = xTimerCreate("LedTimer", pdMS_TO_TICKS(interval * 1000),
                             pdTRUE, this, tmr_callback);
    xTimerStart(led_timer, 0);
}


void LedControl::tmr_callback(TimerHandle_t xTimer) {
    auto inst = static_cast<LedControl*>(pvTimerGetTimerID(xTimer));
    if (inst) inst->toggle();
}

void LedControl::toggle(){
    state = !state;
    gpio_put(pin, state);
}

void LedControl::update_interval(uint32_t new_interval) {
    interval = new_interval;
    std::cout << "Interval changed to " << new_interval << "seconds." << std::endl;
    xTimerChangePeriod(led_timer, pdMS_TO_TICKS(interval * 1000), 0);
}

//pitäskö vaihtaa tää niin et se palauttaa sen kuluneen ajan ja sit printtailee sen muualla?
void LedControl::previous_toggle() {
    //calculates the ticks remaining to next expiration time
    TickType_t ticks_remaining = xTimerGetExpiryTime( led_timer) - xTaskGetTickCount();
    //std::cout << "remaining time in ticks: " << ticks_remaining << std::endl;

    //gets the timer period (in ticks), subtracts remaining ticks -> tick count since last expired
    TickType_t ticks_elapsed = xTimerGetPeriod(led_timer) - ticks_remaining;

    //gets ticks/second from FreeRTOSConfig.h (could just be looked up, it's 1000Hz)
    const TickType_t tick_rate = configTICK_RATE_HZ;

     //convert elapsed ticks to seconds
    double seconds_elapsed = static_cast<double>(ticks_elapsed) / tick_rate;
 /*   std::cout << "last toggled in ticks: " << ticks_elapsed << std::endl;
    std::cout << "last toggled in seconds: " << seconds_elapsed << std::endl;*/

   //print at a one decimal precision
    printf("Time since last toggle: %.1f seconds\n", seconds_elapsed);
}



