#include "update.h"

void updateAir(Info *air) {
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

void updateSoil(Info *soil) {
    uint16_t moisture_value = 0;
    float temperature_value = 0;
    int ret_moist = adafruit_stemma_soil_sensor_read_moisture(I2C_NUM, &moisture_value);
    int ret_temp = adafruit_stemma_soil_sensor_read_temperature(I2C_NUM, &temperature_value);

    ESP_ERROR_CHECK(adafruit_stemma_soil_sensor_shared_i2c_init());

    if (ret_moist == ESP_OK && ret_temp == ESP_OK) {
        soil -> soilHum = moisture_value;
        soil -> soilTmp = temperature_value;
    } else {
        printf("Error reading soil data\n");
        soil -> soilHum = -1;
        soil -> soilTmp = -1;
    }    
}

void updateLight(Info *light) {
    light -> lightVal = adc1_get_raw(ADC1_CHANNEL_0);
}

void updateInfo(Info *info) {
    updateAir(info);
    updateSoil(info);
    updateLight(info);
}

void periodicRead(SSD1306_t *dev, int time) { // Read and get average over a period of time
    printf("Reading data for %d seconds:\n", time);
    Info *data = (Info *) malloc(sizeof(Info) * time);
    for (int i = 0; i < time; i++) {
        updateInfo(&(data[i]));
        printInfo(&(data[i]));
        displayInfo(dev, &(data[i]));
        vTaskDelay(DELAY(735)); // The time it takes to execute one iteration is 265ms
    }
    printData(data, time);
    free(data); // Maybe needs to be moved if we want to use the array more
}

