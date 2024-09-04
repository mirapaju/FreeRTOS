//
// Created by mirap on 9/4/2024.
//

#ifndef EXERCISE2_EVENTS_H
#define EXERCISE2_EVENTS_H

typedef enum {
    BUTTON_PRESS,
    CLOCKWISE,
    COUNTERCLOCKWISE
} Event_type;

struct RotaryEvents{
    int pin;
    Event_type event_type;
};



#endif //EXERCISE2_EVENTS_H
