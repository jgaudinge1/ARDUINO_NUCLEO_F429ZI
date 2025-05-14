// Copyright (c) 2024 Antoine TRAN TAN

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"
#include "event_groups.h"

#define BP PC13
#define LED PE11

EventGroupHandle_t eflag;

void main_bp(void *)
{
    typedef enum {relache, appuie} etat_type;

    static etat_type etat;

    while(true)
    {
        switch(etat)
        {

            case relache:
                if(digitalRead(BP))
                {
                    etat = appuie;
                }
                break;

            case appuie:
                if(!digitalRead(BP))
                {
                    etat = relache;
                    xEventGroupSetBits(eflag, 0x04);
                }
                break;
            
            default: 
                etat = relache; 
                break;

        }

        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

void main_led(void *)
{
    uint32_t flags;
    while(true)
    {
        flags = xEventGroupWaitBits(eflag, 0x04, pdTRUE, pdFALSE, -1);
        if((flags &0x04)==0x04)
        {
            digitalToggle(LED);
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
        lcd_printf("Mbed App Shield!");

        lcd_locate(0, 20);
        lcd_printf("Counting : %d", j++);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup()
{

    pinMode(BP, INPUT_PULLDOWN);
    pinMode(LED, OUTPUT);

    eflag = xEventGroupCreate();


    xTaskCreate(my_main, "my_main", 1024, NULL, 1, NULL);
    xTaskCreate(main_bp, "main_bp", 1024, NULL, 1, NULL);
    xTaskCreate(main_led, "main_led", 1024, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}

void loop()
{
    // Empty. Things are done in my_main
}
