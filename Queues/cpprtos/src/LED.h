//
// Created by mirap on 9/4/2024.
//

#ifndef EXERCISE1_LED_H
#define EXERCISE1_LED_H

#include "pico/stdlib.h"
#include "pico/time.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

class LED {
public:
    explicit LED(int gpio);
    void on() const;
    void off() const;
    void blink(int delay) const;

private:
    int pin;
};

#endif //EXERCISE1_LED_H
