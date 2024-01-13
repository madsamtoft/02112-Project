#include "screens.h"

void clearScreen(SSD1306_t *dev) { // Method to clear the screen
    ssd1306_clear_screen(dev, false);
    ssd1306_contrast(dev, 0xff);
}

void displayMenu(SSD1306_t *dev, int select) { // Method to display the menu
    ssd1306_display_text(dev, 1, "Menu:", 5, false);
    ssd1306_display_text(dev, 2, "Overview", 8, (select == 0));
    ssd1306_display_text(dev, 3, "Get Average", 11, (select == 1));
    ssd1306_display_text(dev, 4, "Soil Sensor", 11, (select == 2));
    ssd1306_display_text(dev, 5, "Air Sensor", 10, (select == 3));
    ssd1306_display_text(dev, 6, "Light Sensor", 12, (select == 4));
}

void displayMenuAverage(SSD1306_t *dev, int select) {
    ssd1306_display_text(dev, 1, "Get average values over:", 5, false);
    ssd1306_display_text(dev, 2, "5 minutes", 8, (select == 0));
    ssd1306_display_text(dev, 3, "30 minutes", 11, (select == 1));
    ssd1306_display_text(dev, 4, "1 hour", 11, (select == 2));
    ssd1306_display_text(dev, 5, "2 hours", 10, (select == 3));
}

void displayInfo(SSD1306_t *dev, Info *info) { // Method to display current info values
    char airTemp[17];
    char soilTemp[17];
    char airHumidity[17];
    char soilHumidity[17];
    sprintf(airTemp, "Air: %10.1fC", info -> airTmp);
    sprintf(soilTemp, "Soil: %9.1fC", info -> soilTmp);
    sprintf(airHumidity, "Air: %10.1f%%", info -> airHum);
    sprintf(soilHumidity, "Soil: %10d", info -> soilHum);

    ssd1306_display_text(dev, 1, "Overview:", 9, false);
    ssd1306_display_text(dev, 2, "Temperature", 12, false);
    ssd1306_display_text(dev, 3, airTemp, 16, false);
    ssd1306_display_text(dev, 4, soilTemp, 16, false);
    ssd1306_display_text(dev, 5, "Humidity", 9, false);
    ssd1306_display_text(dev, 6, airHumidity, 16, false);
    ssd1306_display_text(dev, 7, soilHumidity, 16, false);
}

void displayAverage(SSD1306_t *dev, Info *info) {

}

void averageSelect(SSD1306_t *dev) {
    int select = 0;
    while (1) {
        displayMenuAverage(dev, select);
        if (gpio_get_level(GPIO_BTN_ENTER) == 0) {
            clearScreen(dev);
            if (select == 0) {
                periodicRead(dev, 300);
                return;
            } else if (select == 1) {
                periodicRead(dev, 1800);
                return;
            } else if (select == 2) {
                return;
                periodicRead(dev, 3600);
            } else if (select == 3) {
                periodicRead(dev, 7200);
                return;
            }
            
        }
        if (gpio_get_level(GPIO_BTN_SELECT) == 0) {
            select++;
            select %= 4;
            vTaskDelay(DELAY(100));
        }
    }
}

void menuSelect(SSD1306_t *dev) {
    int select = 0;
    while (1) {
        displayMenu(dev, select);
        if (gpio_get_level(GPIO_BTN_ENTER) == 0) {
            clearScreen(dev);
            if (select == 0) {
                return;
            } else if (select == 1) {
                displayMenuAverage(dev);
                return;
            } else if (select == 2) {
                return;
            } else if (select == 3) {
                return;
            } else if (select == 4) {
                return;
            
            }
        }
        if (gpio_get_level(GPIO_BTN_SELECT) == 0) {
            select++;
            select %= 5;
            vTaskDelay(DELAY(100));
        }
    }
}