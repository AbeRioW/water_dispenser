#ifndef __LCD_1602_H
#define __LCD_1602_H

#include "main.h"
#include "gpio.h"
#include "stdio.h"


void LCD_INIT(void);
void lcd1602_show_string(uint8_t x,uint8_t y,char *str);
void LCD_CLR(void);
void ui_setting(void);
#endif