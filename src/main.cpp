// Copyright (c) 2024 Antoine TRAN TAN

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"

#define CCA PC8
#define CCB PC9

int count = 0;

void my_CCA()
{
    if((!digitalRead(CCB))^(digitalRead(CCA)))
    {
        count=count-1;
    }
    else
    {
        count++;
    }
}



void my_main(void *)
{
    int j = 0;

    // Setup the LCD display
    lcd_begin();

    while (true)
    {
        lcd_cls();
        lcd_locate(0,10);
        lcd_printf("compteur: %d", count);      

        vTaskDelay(75 / portTICK_PERIOD_MS);
    }
}

void setup()
{

    pinMode(CCA, INPUT_PULLDOWN);
    pinMode(CCB, INPUT_PULLDOWN);

    attachInterrupt(CCA, my_CCA, CHANGE);

    xTaskCreate(my_main, "my_main", 1024, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}

void loop()
{
    // Empty. Things are done in my_main
}
