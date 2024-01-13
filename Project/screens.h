#ifndef SCREENS_H
#define SCREENS_H

#include "config.h"
#include "update.h"

void clearScreen(SSD1306_t *dev);
void displayMenu(SSD1306_t *dev, int select);
void displayMenuAverage(SSD1306_t *dev, int select);
void displayInfo(SSD1306_t *dev, Info *info);
void displayAverage(SSD1306_t *dev, Info *info);
void averageSelect(SSD1306_t *dev);
void menuSelect(SSD1306_t *dev);

#endif