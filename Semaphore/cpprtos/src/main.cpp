#include <cstdio>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hardware/timer.h"
extern "C" {
uint32_t read_runtime_ctr(void) {
    return timer_hw->timerawl;
}
}

#define QUEUE_LEN 64
#define LED_PIN 21
#define BLINK_DELAY 100

QueueHandle_t char_q;
SemaphoreHandle_t xBinarySemaphore;

//read serial port, echo character, send to blink
void read_task(void *params) {
    while (true) {
        int ch = getchar_timeout_us(0);
        if (ch!=PICO_ERROR_TIMEOUT && ch!='\n') {
            putchar(ch);
            xSemaphoreGive(xBinarySemaphore);
            vTaskDelay(10);
        }
    }
}

//blink led: one char = one blink
void led_task(void *params) {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE) {
            gpio_put(LED_PIN, true);
            vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY));
            gpio_put(LED_PIN, false);
            vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY));
        }
    }
}

int main() {
    stdio_init_all();

    char_q = xQueueCreate(QUEUE_LEN, sizeof(char));
    xBinarySemaphore = xSemaphoreCreateBinary();

    xTaskCreate(read_task, "serial_read", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(led_task, "blink_led", 256, NULL, tskIDLE_PRIORITY + 1, NULL);

    vTaskStartScheduler();
}



