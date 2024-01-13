//Own libraries
#include "config.h"
#include "screens.h"
#include "update.h"
#include "console.h"

void app_main(void) {
    ic2Config();
    initSoil();
    initLight();
    buttonConfig();

    Info current;
    SSD1306_t dev;
    initDisplay(&dev);

    while (1) {
        if (gpio_get_level(GPIO_BTN_ENTER) == 0) {
            clearScreen(&dev);
            menuSelect(&dev);
        }
        updateInfo(&current);
        printInfo(&current);
        displayInfo(&dev, &current);
        vTaskDelay(DELAY(735));
    }
}