#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//Driver libraries
#include "driver/gpio.h"
#include "driver/adc.h"

#define DELAY(ms) ms / portTICK_PERIOD_MS

void light_adc_demo() {
    //Configuring the ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); //ADC1_CHANNEL_0 is on GPIO0 (GPIOzero)

    for (int i = 0; i < 20; i++)
    {
        int val = adc1_get_raw(ADC1_CHANNEL_0);
        printf("Light value: %d\n", val);
        vTaskDelay(DELAY(1000));  // Delay for 2 seconds
    }
}

void app_main(void) {
    while(1) {
        light_adc_demo();
    }
}
