
#ifndef __MAPCREATE_H__ // do not include more than once
#define __MAPCREATE_H__
#include <stdint.h>

//used for printing unique levels and filling the live map

void setMapPixel(uint8_t x, uint8_t y);
void fillLiveMap(void);
void printLevelOne(void);
void printLevelTwo(void);
void printLevelThree(void);
void intro(void);
void restartLevel(uint8_t level);
void outro(void);
void gameOver(void);
#endif
