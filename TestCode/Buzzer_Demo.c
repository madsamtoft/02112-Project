#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Driver libraries
#include "driver/ledc.h"

// GPIO for the buzzer
#define BUZZ_TIMER              LEDC_TIMER_1
#define BUZZ_MODE               LEDC_LOW_SPEED_MODE
#define BUZZ_OUTPUT_IO          (9) // Define the output GPIO for red
#define BUZZ_CHANNEL            LEDC_CHANNEL_4
#define BUZZ_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define BUZZ_DUTY               (4096) // Set duty to 50%. (2 ** 13) * 50% = 4096
#define BUZZ_FREQUENCY          (1000) // Frequency in Hertz. Set frequency at 1 kHz

#define DELAY(ms) (ms) / portTICK_PERIOD_MS

// Defines for notes
#define C 16.352
#define D 18.354
#define DS 19.445
#define E 20.602
#define F 21.827
#define FS 23.124
#define G 24.5
#define GS 25.957
#define A 27.5
#define B 29.135
#define H 30.868

#define TEMPO 150
#define OCTAVESHIFT 2

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
        vTaskDelay(DELAY(time_ms - 10));
        set_buzzer_volume(0);
        vTaskDelay(DELAY(10));
    } else {
        set_buzzer_volume(0);
        vTaskDelay(DELAY(time_ms));
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

void melody_amogus() {
    float notes[] =   {C,DS,F,FS,F,DS,C,0,GS,DS,C,0};
    int octaves[] =   {3,3, 3,3, 3,3, 3,0,2, 3, 3,0};
    float lengths[] = {4,4, 4,4, 4,4, 4,8,2, 2, 4,4};

    int length = sizeof(octaves)/sizeof(int);
    for (int i = 0; i < length; i ++) {
        play_note(notes[i],octaves[i],lengths[i]/4);
    }
}

void melody_error() {
    float notes[] =   {C,FS};
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

void app_main(void)
{
    //Configure first. IMPORTANT
    configure_buzzer();

    //ERROR MESSAGE
    printf("\nError:\n");
    melody_error();

    vTaskDelay(DELAY(500));

    //FIRST
    printf("\nRunning the load demo:\n");
    melody_load();

    //SECOND
    printf("\nAmoguuuuus:\n");
    melody_amogus();

}
