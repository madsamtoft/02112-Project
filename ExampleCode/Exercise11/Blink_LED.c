#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_task_wdt.h"
#include "led_strip.h"
#include "sdkconfig.h"

#define LED_GPIO 1 // Definition for GPIO port
#define DELAY(s) s * 1000 / portTICK_PERIOD_MS // Delay in seconds

void app_main(void) {
    const double s = 0.5; // Choose delay in seconds
    int level = 0; // Set HIGH/LOW
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    while(1) {
        gpio_set_level(LED_GPIO, level);
        vTaskDelay(DELAY(s));
        level = !level; // Toggle LED
        printf("LED is %s \n", level ? "ON" : "OFF"); // Print LED status
    }
}