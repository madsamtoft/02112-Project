#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//Driver libraries
#include "driver/gpio.h" // For GPIO (Buttons and LEDs)
#include "driver/adc.h" // For ADC (Light)
#include "driver/i2c.h" // For I2C (Soil and Temp/Humidity)

//Temperature/humidity sensor library
#include <am2320.h>

//Stemma soil sensor library
#include "Adafruit_Stemma_soil_sensor.h"

#define I2C_MASTER_FREQ_HZ 75000 //Reduce it to 50000 if the temperature/umidity sensor fails
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_SDA_GPIO 2
#define I2C_MASTER_SCL_GPIO 3
#define I2C_NUM 0

#define DELAY(ms) ms / portTICK_PERIOD_MS

typedef struct {
    float airTmp;
    float airHum;

    uint16_t soilHum;
    float soilTmp;

    int light;
} info;

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

void initAir() {
    //Initialize the sensor (shared i2c) only once after boot.
    i2c_dev_t dev = {0};
    ESP_ERROR_CHECK(am2320_shared_i2c_init(&dev, I2C_NUM));
}

void initSoil() {
    //Initialize the sensor (shared i2c) only once after boot.
    ESP_ERROR_CHECK(adafruit_stemma_soil_sensor_shared_i2c_init());
}

void initLight() {
    //Configuring the ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); //ADC1_CHANNEL_0 is on GPIO0 (GPIOzero)
}

void updateAir(info *air) {
    i2c_dev_t dev = {0};

    //Initialize the sensor (shared i2c) only once after boot.
    ESP_ERROR_CHECK(am2320_shared_i2c_init(&dev, I2C_NUM));

    float temperature, humidity;
    
    esp_err_t res = am2320_get_rht(&dev, &temperature, &humidity);
    if (res == ESP_OK) {
        air -> airTmp = temperature;
        air -> airHum = humidity;
    } else {
        printf("Error reading air temperature: %d (%s)\n", res, esp_err_to_name(res));
    }
}

void updateSoil(info *info) {
    uint16_t moisture_value = 0;
    float temperature_value = 0;
    int ret_moist = adafruit_stemma_soil_sensor_read_moisture(I2C_NUM, &moisture_value);
    int ret_temp = adafruit_stemma_soil_sensor_read_temperature(I2C_NUM, &temperature_value);

    ESP_ERROR_CHECK(adafruit_stemma_soil_sensor_shared_i2c_init());

    if (ret_moist == ESP_OK && ret_temp == ESP_OK) {
        info -> soilHum = moisture_value;
        info -> soilTmp = temperature_value;
    } else {
        printf("Error reading soil data\n");
    }    
}

void updateLight(info *info) {
    info -> light = adc1_get_raw(ADC1_CHANNEL_0);
}

void printInfo(info *info) {
    // Print info
    printf("Air Temperature: %.1f°C, Air Humidity: %.1f%%\n", info -> airTmp, info -> airHum);
    printf("Soil Temperature: %.1f°C, Soil Moisture: %u\n", info -> soilTmp, info -> soilHum);
    printf("Light value: %d\n", info -> light);
    printf("\n");
}

void updateInfo(info *info) {
    updateAir(info);
    updateSoil(info);
    updateLight(info);
}

void app_main(void) {
    ic2Config();
    initSoil();
    initLight();

    info info;

    while (1) {
        updateInfo(&info);
        printInfo(&info);
        vTaskDelay(DELAY(1000));
    }
}