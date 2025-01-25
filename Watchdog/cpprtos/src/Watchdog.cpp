//
// Created by mirap on 9/19/2024.
//

#include "Watchdog.h"
//Task 4 prints “OK” and number of elapsed ticks from last “OK” when all (other) tasks have
// notified that they have run the loop.
// If some of the tasks does not run within 30 seconds Task 4 prints “Fail” and the number
//of the task(s) not meeting the deadline and then Task 4 suspends itself

Watchdog::Watchdog(EventGroupHandle_t eg, Debug d) : event_group(eg), logger(d) {
    xTaskCreate(Watchdog::runner, "Watchdog_task", 512,
                (void*) this, tskIDLE_PRIORITY +2, &handle);
}

void Watchdog::runner(void *params){
    auto inst = static_cast<Watchdog *>(params);
    inst->watchdog_task();
}


//Nyt tickien määrä edellisestä ok on aina tietty melkeinpä TIMEOUTIN verran, koska se määrittelee
//millä intervallilla bitit tarkistetaan
//pitäiskö vahdata timeoutilla milloin eventgroupissa joku muuttuu, sit hakea bitit ja tarkistella?
void Watchdog::watchdog_task() {
    TickType_t check_passed = xTaskGetTickCount(); //track the last time everything was ok
    bool bits_set=true;
    const EventBits_t uxBitsToWaitFor = (1 << TASK_COUNT) - 1;

    while (true) {
        EventBits_t bits = xEventGroupWaitBits(event_group,uxBitsToWaitFor,
                pdTRUE,pdTRUE,pdMS_TO_TICKS(TIMEOUT));

        TickType_t current = xTaskGetTickCount();
        //loop through the bits (corresponding to each task) and check if it was set
        for (int taskID = 0; taskID < TASK_COUNT; ++taskID) {
            if (!(bits & (1 << taskID))) {  //if the bit for this task is not set
                logger.debug("Fail: Task %d did not run\n", (taskID + 1), 0 ,0);
                bits_set = false;
            }
        }

        if (!bits_set) vTaskSuspend(handle);
        //all tasks ran successfully
        else {
            TickType_t elapsed = current - check_passed;
            logger.debug("OK. Ticks since previous OK: %lu\n", elapsed, 0, 0);
            check_passed = current;
        }
    }
}


