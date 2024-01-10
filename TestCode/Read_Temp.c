#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//Driver libraries
#include "driver/i2c.h"

//Temperature/humidity sensor library
#include <am2320.h>

#define I2C_MASTER_FREQ_HZ 75000 //Reduce it to 50000 if the temperature/umidity sensor fails
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_SDA_GPIO 2
#define I2C_MASTER_SCL_GPIO 3
#define I2C_NUM 0

void temperaure_humidity_demo() {
    i2c_dev_t dev = {0};

    //Initialize the sensor (shared i2c) only once after boot.
    ESP_ERROR_CHECK(am2320_shared_i2c_init(&dev, I2C_NUM));

    float temperature, humidity;

    for (int i = 0; i < 20; i++)
    {
        esp_err_t res = am2320_get_rht(&dev, &temperature, &humidity);
        if (res == ESP_OK)
            printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temperature, humidity);
        else
            printf("Error reading data: %d (%s)\n", res, esp_err_to_name(res));

        //1000 ms delay
        vTaskDelay((1000) / portTICK_PERIOD_MS);
    }
}

void ic2Config() {
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
}

void app_main(void) {
    ic2Config();

    printf("\nRunning temperature/humidity sensor:\n");
    while (1) {
        temperaure_humidity_demo();
    }
}
