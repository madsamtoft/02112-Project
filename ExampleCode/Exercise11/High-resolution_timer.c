#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "sdkconfig.h"

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

void app_main(void) {
    int64_t tS = esp_timer_get_time(); // Timestamp 1

    primeFactor(9223372036854775807);

    int64_t tE = esp_timer_get_time(); // Timestamp 2

    int64_t diff = tE - tS; // Time taken to execute a function

    printf("Elapsed time: %llu microseconds\n\n", (long long) diff);
}


