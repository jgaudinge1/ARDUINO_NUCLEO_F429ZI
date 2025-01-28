// Copyright (c) 2024 Antoine TRAN TAN
// Copyright (c) 2016 olikraus@gmail.com

#include "Arduino.h"
#include "C12832.h"
#include "U8g2lib.h"
#include <cstdarg>
#include <STM32FreeRTOS.h>
#include "semphr.h"

char lcd_buffer[512];
U8G2_ST7565_NHD_C12832_F_4W_HW_SPI *lcd_u8g2_c12832_ptr;
int lcd_cursor_pos[2] = {0, 0};
SemaphoreHandle_t lcd_sem;

void lcd_begin()
{
    lcd_u8g2_c12832_ptr = new U8G2_ST7565_NHD_C12832_F_4W_HW_SPI(U8G2_R2, D10, D7, D12);
    lcd_u8g2_c12832_ptr->begin();
    lcd_u8g2_c12832_ptr->clearBuffer();
    lcd_u8g2_c12832_ptr->setFont(u8g2_font_b12_t_japanese2);
    lcd_sem = xSemaphoreCreateMutex();
}

void lcd_circle(int x, int y, int r, int colour)
{
    xSemaphoreTake(lcd_sem, -1);
    lcd_u8g2_c12832_ptr->drawCircle(x, y, r, U8G2_DRAW_ALL);
    lcd_u8g2_c12832_ptr->sendBuffer();
    xSemaphoreGive(lcd_sem);
}

void lcd_fillcircle(int x, int y, int r, int colour)
{
    xSemaphoreTake(lcd_sem, -1);
    lcd_u8g2_c12832_ptr->drawDisc(x, y, r, U8G2_DRAW_ALL);
    lcd_u8g2_c12832_ptr->sendBuffer();
    xSemaphoreGive(lcd_sem);
}

void lcd_cls(void)
{
    xSemaphoreTake(lcd_sem, -1);
    lcd_u8g2_c12832_ptr->clearBuffer();
    lcd_u8g2_c12832_ptr->clearDisplay();
    xSemaphoreGive(lcd_sem);
}

void lcd_locate(int x, int y)
{
    xSemaphoreTake(lcd_sem, -1);
    lcd_cursor_pos[0] = x;
    lcd_cursor_pos[1] = y;
    xSemaphoreGive(lcd_sem);
}

int lcd_printf(const char *format, ...)
{
    int ret;
    va_list args;

    xSemaphoreTake(lcd_sem, -1);
    va_start(args, format);
    ret = vsprintf(lcd_buffer, format, args);
    va_end(args);

    lcd_u8g2_c12832_ptr->drawStr(lcd_cursor_pos[0], lcd_cursor_pos[1], lcd_buffer);
    lcd_u8g2_c12832_ptr->sendBuffer();
    xSemaphoreGive(lcd_sem);

    return ret;
}
