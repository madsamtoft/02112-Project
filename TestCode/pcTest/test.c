#include <stdio.h>
#include <stdlib.h> // malloc

struct Info{
    float airTmp;
    float airHum;

    short soilHum;
    float soilTmp;

    short lightVal;

    struct Info *next;
};

struct Info *createInfo(struct Info *info) {
    struct Info *newInfo;
    newInfo = (struct Info *) malloc(sizeof(struct Info));
    info->next = newInfo;
    newInfo->next = NULL;
    newInfo->airTmp = 0;
    newInfo->airHum = 0;
    newInfo->soilHum = 0;
    newInfo->soilTmp = 0;
    newInfo->lightVal = 0;
    return newInfo;
}

int main() {
    struct Info *info = (struct Info *) malloc(sizeof(struct Info)); // Initialize info pointer

    struct Info *newInfo;
    newInfo = createInfo(info);

    printf("airTmp: %f\n", newInfo->airTmp);
    printf("airHum: %f\n", newInfo->airHum);
    printf("soilHum: %d\n", newInfo->soilHum);
    printf("soilTmp: %f\n", newInfo->soilTmp);
    printf("lightVal: %d\n", newInfo->lightVal);

    return 0;
}
