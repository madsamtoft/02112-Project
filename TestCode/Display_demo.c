#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//Driver libraries
#include "driver/i2c.h"

//Display libraies
#include "ssd1306.h"
//#include "font8x8_basic.h"

#define I2C_MASTER_FREQ_HZ 75000 //Reduce it to 50000 if the temperature/umidity sensor fails
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_SDA_GPIO 2
#define I2C_MASTER_SCL_GPIO 3
#define I2C_NUM 0


void initDisplay(SSD1306_t *dev) {
    i2c_master_shared_i2c_init(dev);
    ssd1306_init(dev, 128, 64);
    printf("Screen width: %d\n", ssd1306_get_width(dev));
    printf("Screen height: %d\n", ssd1306_get_height(dev));

    ssd1306_clear_screen(dev, false);
}


void displayInfo(SSD1306_t *dev) {
    ssd1306_clear_screen(dev, false);
    ssd1306_contrast(dev, 0xff);
    ssd1306_display_text(dev, 0, "Line 1", 6, false);
    ssd1306_display_text(dev, 1, "Line 2", 6, false);

    char airTemp[17];
    char airHumidity[17];
    char soilTemp[17];
    char soilHumidity[17];
    sprintf(airTemp, "Air: %10.2fC", 100.00);
    sprintf(soilTemp, "Soil: %9.2fC", 25.00);
    sprintf(airHumidity, "Air: %10.2f%%", 50.00);
    sprintf(soilHumidity, "Soil: %9.2f%%", 10.00);

    ssd1306_display_text(dev, 2, "Temperature", 12, false);
    ssd1306_display_text(dev, 3, airTemp, 16, false);
    ssd1306_display_text(dev, 4, soilTemp, 16, false);
    ssd1306_display_text(dev, 5, "Humidity", 9, false);
    ssd1306_display_text(dev, 6, airHumidity, 16, false);
    ssd1306_display_text(dev, 7, soilHumidity, 16, false);
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

    SSD1306_t dev;
    initDisplay(&dev);
    displayInfo(&dev);
}
