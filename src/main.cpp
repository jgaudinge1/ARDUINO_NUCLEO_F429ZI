// Copyright (c) 2024 Antoine TRAN TAN

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"

#define button digitalRead(PC13)

static int cpt_appui = 0;

void TaskCpt(void *)
{
    typedef enum {relache, attente_appui, appui, attente_relache} etat_type;
    static etat_type etat =relache;
    pinMode(PC13, INPUT_PULLDOWN);


    while(true)
    {
        
        switch(etat)
        {
            case relache:
                etat = attente_appui;
                break;

            case attente_appui:
                if(button)
                {
                    etat= appui;
                }
                break;

            case appui:
                cpt_appui++;
                etat = attente_relache;
                break;
            
            case attente_relache:
                if(!button)
                {
                    etat = relache;
                }
                break;

            default: break;

        }

        vTaskDelay(100/portTICK_PERIOD_MS);
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
        lcd_printf("Compteur avance");

        lcd_locate(0, 20);
        lcd_printf("Compteur : %d", cpt_appui);

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void setup()
{
 
    xTaskCreate(my_main, "my_main", 1024, NULL, 1, NULL);

    xTaskCreate(TaskCpt, "TaskCpt", 1024, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}

void loop()
{
    // Empty. Things are done in my_main
}
