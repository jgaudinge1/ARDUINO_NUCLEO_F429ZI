// Copyright (c) 2024 Antoine TRAN TAN

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"
#include "event_groups.h"

#define LED PE11
#define haut PC3
#define bas PD11
#define droite PF10
#define gauche PF5 
#define centre PF14

EventGroupHandle_t eflag;

typedef enum {relache, appui} etat_type;

void main_up(void *)
{

    static etat_type etat;

    while(true)
    {
        switch(etat)
        {
            case relache:
                if(digitalRead(haut))
                {
                    etat= appui;
                    xEventGroupSetBits(eflag, 0x01);
                }
                break;

            case appui:
                if(!digitalRead(haut))
                {
                    etat= relache;
                }
                break;

            default: break;

        }
        
        vTaskDelay(100/portTICK_PERIOD_MS);

    }
}

void main_down(void *)
{

    static etat_type etat;

    while(true)
    {
        switch(etat)
        {
            case relache:
                if(digitalRead(bas))
                {
                    etat= appui;
                    xEventGroupSetBits(eflag, 0x02);
                }
                break;

            case appui:
                if(!digitalRead(bas))
                {
                    etat= relache;
                }
                break;

            default: break;

        }
        
        vTaskDelay(100/portTICK_PERIOD_MS);

    }
}

void main_led(void *)
{

    uint32_t flags;

    while(true)
    {
        flags = xEventGroupWaitBits(eflag, 0x03, pdTRUE, pdFALSE, -1);
        if((flags & 0x01)== 0x01)
        {
            digitalWrite(LED, 1);
        }
        if((flags & 0x02)== 0x02)
        {
            digitalWrite(LED, 0);
        }
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
        lcd_printf("TP 4 : EXO 2");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup()
{

    pinMode(LED, OUTPUT);
    pinMode(haut, INPUT_PULLDOWN);
    pinMode(bas, INPUT_PULLDOWN);
    pinMode(gauche, INPUT_PULLDOWN);
    pinMode(droite, INPUT_PULLDOWN);
    pinMode(centre, INPUT_PULLDOWN);
    

    eflag = xEventGroupCreate();

    xTaskCreate(my_main, "my_main", 1024, NULL, 1, NULL);
    xTaskCreate(main_down, "main_down", 1024, NULL, 1, NULL);
    xTaskCreate(main_up, "main_up", 1024, NULL, 1, NULL);
    xTaskCreate(main_led, "main_led", 1024, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}

void loop()
{
    // Empty. Things are done in my_main
}
