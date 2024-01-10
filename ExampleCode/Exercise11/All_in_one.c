/* All 3 parts of exercise 11,
The program will execute:
- One-shot timer
- Blinker
- High-resolution timer
In that order, which can be observed
both in the terminal and with the board. */
#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

#define LED_GPIO GPIO_NUM_1 // Definition for GPIO port
#define DELAY(s) s * 1000 / portTICK_PERIOD_MS // Delay in seconds

static const char *TAG = "Example";

void timer_demo() {
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

void blink_demo(int count) {
    printf("\n");
    const double s = 0.5; // Choose delay in seconds
    int level = 0; // Set HIGH/LOW
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    for(int i = 0; i < count*2+1; i ++) {
        printf("LED is %s \n", level ? "ON" : "OFF"); // Print LED status
        gpio_set_level(LED_GPIO, level);
        vTaskDelay(DELAY(s));
        level = !level; // Toggle LED
    }
    printf("\n");
}

// Prime Factor function which can take a long time depending on input
void primeFactor(long long num) { 
    printf("\nPrimeFactors of %llu: ", num);
    long long i = 2;
    long long squareRoot = sqrt(num); // Predefining for efficiency
    while (i <= squareRoot){
        if(num % i == 0) {
            num /= i;
            squareRoot = sqrt(num);
            printf("%llu, ", i);
        } else {
            i++;
        }
    }
    printf("%llu\n", num);
}

void high_res_timer_demo(void) {
    int64_t tS = esp_timer_get_time(); // Timestamp 1

    primeFactor(9223372036854775807);

    int64_t tE = esp_timer_get_time(); // Timestamp 2

    int64_t diff = tE - tS; // Time taken to execute a function

    printf("Elapsed time: %llu microseconds\n\n", (long long) diff);
}

void app_main(void) {
    ESP_LOGI(TAG, "Running Timer Demo:");
    timer_demo();

    ESP_LOGI(TAG, "Running Blink Demo. Blinking 5 times");
    blink_demo(5);

    ESP_LOGI(TAG, "Running High Resolution Timer Demo");
    high_res_timer_demo();
}