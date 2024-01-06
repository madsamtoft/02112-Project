#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "sdkconfig.h"

static const char *TAG = "";

void primeFactor(long num);

void app_main(void) {
    int64_t tS = esp_timer_get_time(); 

    primeFactor(12);

    int64_t tE = esp_timer_get_time();

    int64_t diff = tE - tS;

    ESP_LOGI(TAG, "It took %lu us to execute primeFactor()", (long) diff);
}

void primeFactor(long num) {
    ESP_LOGI(TAG, "PrimeFactors of %lu", num);
    long i = 2;

    while (i <= sqrt(num)){
        if(num % i == 0) {
            num /= i;
            ESP_LOGI(TAG, "%lu is a factor", i);
        } else {
            i++;
        }
    }
}
