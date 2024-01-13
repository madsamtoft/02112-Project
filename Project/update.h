#ifndef UPDATE_H
#define UPDATE_H

#include "config.h"
#include "console.h"
#include "screens.h"

void updateAir(Info *air);
void updateSoil(Info *soil);
void updateLight(Info *light);
void updateInfo(Info *info);
void periodicRead(SSD1306_t *dev, int time);

#endif