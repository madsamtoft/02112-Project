#include <stdio.h>
#include <stdlib.h> // malloc
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

//Driver libraries
#include "driver/gpio.h" // For GPIO (Buttons and LEDs)
#include "driver/adc.h" // For ADC (Light)
#include "driver/i2c.h" // For I2C (Soil and Temp/Humidity)

//Temperature/humidity sensor library
#include <am2320.h>

//Stemma soil sensor library
#include "Adafruit_Stemma_soil_sensor.h"

//Display libraies
#include "ssd1306.h"
//#include "font8x8_basic.h"

#define I2C_MASTER_FREQ_HZ 75000 //Reduce it to 50000 if the temperature/umidity sensor fails
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_SDA_GPIO 2
#define I2C_MASTER_SCL_GPIO 3
#define I2C_NUM 0

#define DELAY(ms) (ms) / portTICK_PERIOD_MS

//Buzzer

#include <math.h>
#include "esp_chip_info.h"
#include "esp_flash.h"
#include <string.h>
#include "esp_log.h"

//Driver libraries
#include "driver/i2c.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/adc.h"


#define RED_LED_GPIO 8
#define GREEN_LED_GPIO 7

#define BUZZ_TIMER              LEDC_TIMER_1
#define BUZZ_MODE               LEDC_LOW_SPEED_MODE
#define BUZZ_OUTPUT_IO          (9) // Define the output GPIO for red
#define BUZZ_CHANNEL            LEDC_CHANNEL_4
#define BUZZ_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define BUZZ_DUTY               (4096) // Set duty to 50%. (2 ** 13) * 50% = 4096
#define BUZZ_FREQUENCY          (1000) // Frequency in Hertz. Set frequency at 1 kHz

//Defines for notes. Made by Sebastian
#define C 16.352
#define D 18.354
#define Eb 19.445
#define E 20.602
#define F 21.827
#define Fb 23.124
#define G 24.5
#define Gb 25.957
#define A 27.5
#define B 29.135
#define H 30.868

#define TEMPO 200
#define OCTAVESHIFT 2

struct Info{
    float airTmp;
    float airHum;

    int soilHum;
    float soilTmp;

    int lightVal;
};

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

void initDisplay(SSD1306_t *dev) {
    i2c_master_shared_i2c_init(dev);
    ssd1306_init(dev, 128, 64);
    printf("Screen width: %d\n", ssd1306_get_width(dev));
    printf("Screen height: %d\n", ssd1306_get_height(dev));

    ssd1306_clear_screen(dev, false);

    ssd1306_contrast(dev, 0xff);
}

void updateAir(struct Info *air) {
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

void updateSoil(struct Info *soil) {
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

void updateLight(struct Info *light) {
    light -> lightVal = adc1_get_raw(ADC1_CHANNEL_0);
}

void updateInfo(struct Info *info) {
    updateAir(info);
    updateSoil(info);
    updateLight(info);
}

void printInfo(struct Info *info) {
    // Print info
    printf("Air Temperature: %.1f°C, Air Humidity: %.1f%%\n", info -> airTmp, info -> airHum);
    printf("Soil Temperature: %.1f°C, Soil Moisture: %u\n", info -> soilTmp, info -> soilHum);
    printf("Light value: %d\n", info -> lightVal);
    printf("\n");
}

void printData(struct Info data[], int length) {
    printf("airTMP\tairHUM\tsoilTMP\tsoilHUM\tlight\n");
    float airTmpSum = 0;
    float airHumSum = 0;
    float soilTmpSum = 0;
    float soilHumSum = 0;
    float lightValSum = 0;
    for (int i = 0; i < length; i++) {
        printf("%.1f\t", data[i].airTmp);
        printf("%.1f\t", data[i].airHum);
        printf("%.1f\t", data[i].soilTmp);
        printf("%d\t", data[i].soilHum);
        printf("%d\t\n", data[i].lightVal);
        airTmpSum += data[i].airTmp;
        airHumSum += data[i].airHum;
        soilTmpSum += data[i].soilTmp;
        soilHumSum += data[i].soilHum;
        lightValSum += data[i].lightVal;
    }
    printf("\nAverage readings:\n");
    printf("%.1f\t", airTmpSum / length);
    printf("%.1f\t", airHumSum / length);
    printf("%.1f\t", soilTmpSum / length);
    printf("%.1f\t", soilHumSum / length);
    printf("%.1f\t\n", lightValSum / length);
}

void displayInfo(SSD1306_t *dev, struct Info *info) {
    char airTemp[17];
    char soilTemp[17];
    char airHumidity[17];
    char soilHumidity[17];
    sprintf(airTemp, "Air: %10.1fC", info -> airTmp);
    sprintf(soilTemp, "Soil: %9.1fC", info -> soilTmp);
    sprintf(airHumidity, "Air: %10.1f%%", info -> airHum);
    sprintf(soilHumidity, "Soil: %10d", info -> soilHum);

    ssd1306_display_text(dev, 2, "Temperature", 12, false);
    ssd1306_display_text(dev, 3, airTemp, 16, false);
    ssd1306_display_text(dev, 4, soilTemp, 16, false);
    ssd1306_display_text(dev, 5, "Humidity", 9, false);
    ssd1306_display_text(dev, 6, airHumidity, 16, false);
    ssd1306_display_text(dev, 7, soilHumidity, 16, false);
}

// LED
void LED_Config(){
    gpio_config_t io_conf;

    // Configure RED LED GPIO
    io_conf.pin_bit_mask = (1ULL << RED_LED_GPIO);
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);   

    // Configure RED LED GPIO
    io_conf.pin_bit_mask = (1ULL << GREEN_LED_GPIO);
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);   
}

// BUZZER COMMANDS
void set_buzzer_volume(float volume) {
    //volume between 0 and 1
    volume = volume < 0 ? 0 : volume;
    volume = volume > 1 ? 1 : volume;
    int duty = volume * pow(2,13);
    ESP_ERROR_CHECK(ledc_set_duty(BUZZ_MODE, BUZZ_CHANNEL, duty)); //50% duty
    ESP_ERROR_CHECK(ledc_update_duty(BUZZ_MODE, BUZZ_CHANNEL));
}

void play_tone(int freq_hz, int time_ms) {
    if (freq_hz != 0) {
        set_buzzer_volume(0.5);
        ESP_ERROR_CHECK(ledc_set_freq(BUZZ_MODE, BUZZ_TIMER, freq_hz));
        vTaskDelay((time_ms - 10) / portTICK_PERIOD_MS);
        set_buzzer_volume(0);
        vTaskDelay((10) / portTICK_PERIOD_MS);
    } else {
        set_buzzer_volume(0);
        vTaskDelay((time_ms) / portTICK_PERIOD_MS);
    }
}

float get_freq(float note, double octave) {
    return note * pow(2,octave + OCTAVESHIFT);
}

void play_note(double note, double octave, double beats) {
    // milliseconds = Beats / Beats per Minute * seconds per minute * milliseconds per second
    float time_ms = beats/TEMPO*60*1000;
    play_tone(get_freq(note,octave),time_ms);
}

//Function content from "ESP32-C3 Components Demo" on DTU-Learn
void configure_buzzer() {
    // Prepare and then apply the LEDC PWM timer configuration (we use it for the buzzer)
    ledc_timer_config_t ledc_timer_buzz = {
        .speed_mode       = BUZZ_MODE,
        .duty_resolution  = BUZZ_DUTY_RES,
        .timer_num        = BUZZ_TIMER,
        .freq_hz          = BUZZ_FREQUENCY,  // Set output frequency at 1 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_buzz));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel_buzz = {
        .speed_mode     = BUZZ_MODE,
        .channel        = BUZZ_CHANNEL,
        .timer_sel      = BUZZ_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = BUZZ_OUTPUT_IO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_buzz));
}

void melody_load() {
    float notes[] =   {C,D,E,F,G,A,H,C,H,A,G,F,E,D,C,0};
    int octaves[] =   {3,3,3,3,3,3,3,4,3,3,3,3,3,3,3,0};
    float lengths[] = {4,4,4,4,4,4,4,1,1,1,1,1,1,1,4,4};

    int length = sizeof(octaves)/sizeof(int);
    for (int i = 0; i < length; i ++) {
        play_note(notes[i],octaves[i],lengths[i]/4);
    }
}

void melody_load2() {
    float notes[] =   {C,D,E,0,C};
    int octaves[] =   {3,3,3,0,4};
    float lengths[] = {2,2,2,4,2};

    int length = sizeof(octaves)/sizeof(int);
    for (int i = 0; i < length; i ++) {
        play_note(notes[i],octaves[i],lengths[i]/4);
    }
}

void melody_amogus() {
    float notes[] =   {C,Eb,F,Fb,F,Eb,C,0,Gb,Eb,C,0};
    int octaves[] =   {3,3, 3,3, 3,3, 3,0,2, 3, 3,0};
    float lengths[] = {4,4, 4,4, 4,4, 4,8,2, 2, 4,4};

    int length = sizeof(octaves)/sizeof(int);
    for (int i = 0; i < length; i ++) {
        play_note(notes[i],octaves[i],lengths[i]/4);
    }
}

void melody_error() {
    float notes[] =   {C,Fb};
    int octaves[] =   {4,4};
    float lengths[] = {1,1};

    int length = sizeof(octaves)/sizeof(int);
    int repeats = 4; 
    for (int n = 0; n < repeats; n ++) {
        for (int i = 0; i < length; i ++) {
            play_note(notes[i],octaves[i],lengths[i]/4);
        }
    }
}

void experiment(SSD1306_t *dev) {
    
    gpio_set_level(RED_LED_GPIO, 1);
    gpio_set_level(GREEN_LED_GPIO, 0);

    const int dataSize = 100;
    struct Info *data = (struct Info *) malloc(sizeof(struct Info) * dataSize);
    
    melody_load2();

    int blink = 0;

    for (int i = 0; i < dataSize; i++) {
        updateInfo(&(data[i]));
        printInfo(&(data[i]));
        displayInfo(dev, &(data[i]));
        //vTaskDelay(DELAY(730));
        gpio_set_level(RED_LED_GPIO, blink);
        blink = !blink;
        vTaskDelay((100) / portTICK_PERIOD_MS);
    }

    gpio_set_level(RED_LED_GPIO, 0);
    gpio_set_level(GREEN_LED_GPIO, 1);

    printData(data, dataSize);

    melody_amogus();
}

void app_main(void) {
    ic2Config();
    initSoil();
    initLight();
    //VERY important to configure LED before buzzer
    LED_Config();
    configure_buzzer();

    SSD1306_t dev;
    
    initDisplay(&dev);

    experiment(&dev);
}