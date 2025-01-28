// Copyright (c) 2024 Antoine TRAN TAN

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"

void my_main(void *)
{
    int j = 0;

    // Setup the LCD display
    lcd_begin();

    while (true)
    {
        lcd_cls();

        lcd_locate(0, 10);
        lcd_printf("Mbed App Shield!");

        lcd_locate(0, 20);
        lcd_printf("Counting : %d", j++);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup()
{
    xTaskCreate(my_main, "my_main", 1024, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}

void loop()
{
    // Empty. Things are done in my_main
}
