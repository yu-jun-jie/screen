#ifndef _OLED_H_
#define _OLED_H_
#include "main.h"
#include "i2c.h"

void oledinit(void);
void oledfontx8y8(uint8_t x,uint8_t y,char *str);
void oledfontx8y16(uint8_t x,uint8_t y,char *str);
void oleddat(uint8_t x,uint8_t y,uint8_t dat);

#endif
