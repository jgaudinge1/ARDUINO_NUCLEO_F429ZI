// Copyright (c) 2024 Antoine TRAN TAN

#ifndef C12832_H
#define C12832_H

#include <cstdarg>
#include "U8g2lib.h"


/** Create a C12832 object connected to SPI1
 *
 */
void lcd_begin();

/** draw a circle
 *
 * @param x0,y0 center
 * @param r radius
 * @param colour ,1 set pixel ,0 erase pixel
 *
 */
void lcd_circle(int x, int y, int r, int colour);

/** draw a filled circle
 *
 * @param x0,y0 center
 * @param r radius
 * @param color ,1 set pixel ,0 erase pixel
 *
 * use circle with different radius,
 * can miss some pixel
 */
void lcd_fillcircle(int x, int y, int r, int colour);

void lcd_cls(void);

/** setup cursor position
 *
 * @param x x-position (top left)
 * @param y y-position
 */
void lcd_locate(int x, int y);


int lcd_printf(const char *format, ...);

#endif
