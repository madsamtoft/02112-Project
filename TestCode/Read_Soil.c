#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "esp_chip_info.h"
// #include "esp_flash.h"
// #include <string.h>
// #include "esp_log.h"

//Driver libraries
#include "driver/i2c.h"
#include "driver/gpio.h"

//Stemma soil sensor library
#include "Adafruit_Stemma_soil_sensor.h"

#define I2C_MASTER_FREQ_HZ 75000 //Reduce it to 50000 if the temperature/umidity sensor fails
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_SDA_GPIO 2
#define I2C_MASTER_SCL_GPIO 3
#define I2C_NUM 0

#define DELAY(ms) ms / portTICK_PERIOD_MS

void stemma_soil_demo(){
    int ret_moist = ESP_OK;
    int ret_temp = ESP_OK;
    uint16_t moisture_value = 0;
    float temperature_value = 0;

    //Initialize the sensor (shared i2c) only once after boot.
    ESP_ERROR_CHECK(adafruit_stemma_soil_sensor_shared_i2c_init());

    for (int i = 0; i < 10; i++) {
        ret_moist = adafruit_stemma_soil_sensor_read_moisture(I2C_NUM, &moisture_value);
        ret_temp = adafruit_stemma_soil_sensor_read_temperature(I2C_NUM, &temperature_value);
        if (ret_moist == ESP_OK && ret_temp == ESP_OK) {
            printf("Moisture value = %u, Temperature value = %.2f\n", moisture_value, temperature_value);
        }
        //1000 ms delay
        vTaskDelay(DELAY(1000));
    }
}

void app_main(void) {
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_GPIO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_GPIO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = 0;
    i2c_param_config(I2C_NUM, &conf);
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0));

    printf("\nRunning STEMMA soil sensor demo:\n");
    while (1) {
        stemma_soil_demo();
    }
}
