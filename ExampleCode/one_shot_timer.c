#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_task_wdt.h"
#include "sdkconfig.h"

#define LED_GPIO GPIO_NUM_1 // Definition for GPIO port
#define DELAY(s) s * 1000 / portTICK_PERIOD_MS // Delay in seconds

void app_main(void) {
    esp_task_wdt_deinit();
    const int s = 5; // Choose delay in seconds

    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT); // Set GPIO as output

    printf("\nOne-shot timer of %d seconds started.\n", s);
    gpio_set_level(LED_GPIO, 1); // Turning LED on
    
    int64_t t1 = esp_timer_get_time(); // Timestamp 1
    vTaskDelay(DELAY(s));
    int64_t t2 = esp_timer_get_time(); // Timestamp 2
    
    printf("One-shot timer of %d seconds finished.\n", s);
    gpio_set_level(LED_GPIO, 0); // Turning LED off

    float diff = (float) (t2-t1)/1000; // Time taken
    printf("Took exactly %f ms\n\n", diff);
}

