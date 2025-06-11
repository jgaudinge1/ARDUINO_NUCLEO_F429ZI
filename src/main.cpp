// Copyright (c) 2024 Antoine TRAN TAN

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"
#include "event_groups.h"
#include <map>
#include <string>
#include <iostream>

#define push PF14
#define led PE11

uint32_t start = 0;

char morse[7];

void dico()
{ 
    std::map <std::string ,char > dico =
    {
        {"01", ’A’},
        {"1000", ’B’},
        {"1010", ’C’}
        {"100", ’D’},
        {"0", ’E’},
        {"0010", ’F’},
        {"110", ’G’},
        {"0000", ’H’},
        {"00", ’I’},
        {"0111", ’J’},
        {"101", ’K’},
        {"0100", ’L’},
        {"11", ’M’},
        {"10", ’N’},
        {"111", ’O’},
        {"0110", ’P’},
        {"1101", ’Q’},
        {"010", ’R’},
        {"000", ’S’},
        {"1", ’T’},
        {"001", ’U’},
        {"0001", ’V’},
        {"011", ’W’},
        {"1001", ’X’},
        {"1011", ’Y’},
        {"1100", ’Z’}
    };
}

EventGroupHandle_t eflags;

void my_isr_rise_fall()
{
    uint32_t duree;

    if(digitalRead(push))
    {
        start=millis();
    }
    else
    {
        duree = millis() - start;

        if(duree > 500)
        {
            xEventGroupSetBitsFromISR(eflags, 0x02, NULL);
        }
        else if(duree>100)
        {
            xEventGroupSetBitsFromISR(eflags, 0x01, NULL);
        }
         else if(duree>2000)
        {
            xEventGroupSetBitsFromISR(eflags, 0x04, NULL);
        }
        else
        {

        }
        
    }


}

void main_consommateur(void *)
{
    uint32_t flag_read;
    int n =0;

    while(true)
    {
        flag_read = xEventGroupWaitBits(eflags, 0x07, true, false, -1);

        if((flag_read&0x01) == 0x01)
        {
            morse[n] = '0';
            n= n+1;
        }

        if((flag_read&0x02) == 0x02)
        {
            morse[n] = '1';
            n= n+1;
        }
        if(((flag_read&0x04) == 0x04) | (n==5))
        {
            morse[n] = '\0';

            xEventGroupSetBits(eflags, 0x08);
        }

    }

}

void main_producteur(void *)
{

    attachInterrupt(push, &my_isr_rise_fall, CHANGE);

    while(true)
    {
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }

}

void main(void *)
{
    uint32_t flag_read;

    while(true)
    {
        flag_read = xEventGroupWaitBits(eflags, 0x08, true, false, -1);
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

        lcd_locate(0, 10);
        lcd_printf("Mbed App Shield!");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup()
{

    pinMode(push, INPUT_PULLDOWN);
    pinMode(led, OUTPUT);

    eflags = xEventGroupCreate();

    xTaskCreate(my_main, "my_main", 1024, NULL, 1, NULL);
    xTaskCreate(main_producteur, "main_producteur", 1024, NULL, 1, NULL);
    xTaskCreate(main_consommateur, "main_consommateur", 1024, NULL, 1, NULL);
    xTaskCreate(main, "main", 1024, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}

void loop()
{
    // Empty. Things are done in my_main
}
