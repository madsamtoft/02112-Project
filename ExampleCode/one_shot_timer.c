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
#define DELAY(s) s * 1000000

static const char *TAG = "";

// Predefine functions
void wait(int s);
void initOut(int pin);
void setLevel(int pin, int state);

void app_main(void) {
    esp_task_wdt_deinit();
    const int delay = 5; // Choose delay in seconds

    ESP_LOGI(TAG, "One-shot timer of %d seconds started.", delay);
    initOut(LED_GPIO);
    setLevel(LED_GPIO, 1);
    
    wait(delay);

    ESP_LOGI(TAG, "One-shot timer of %d seconds finished.", delay);

    setLevel(LED_GPIO, 0);
}

void setLevel(int pin, int state) {
    gpio_set_level(pin, state);
}

void initOut(int pin) {
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
}

void wait(int s) {
    int64_t t = esp_timer_get_time();
    int64_t cT = t; //Saving CPU-time at this point in the code
    while (t < DELAY(s) + cT) { //Delay function
        t = (float) esp_timer_get_time();
    }
}