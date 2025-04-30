// Copyright (c) 2024 Antoine TRAN TAN

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"

#define BP PC13


typedef enum {relache, appui, att_appui, att_relache} etat_type;

void automate(void)
{

  static etat_type etat = relache;

  static uint j=0;

  switch(etat)
  {

    case relache:
        
        break;

    case appui:
        j++;
        lcd_locate(10,10);
        lcd_printf("%d", j);
        break;

    case att_appui:
        break;

    case att_relache:
        break;    


  }

  switch(etat)
  {

    case relache:
        etat = att_appui;
        break;

    case appui:
       
        etat= att_relache;
        break;

    case att_appui:
        if(digitalRead(BP) == 1)
        {
            etat = appui;
            
        }
        break;

    case att_relache:
        if(digitalRead(BP) == 0)
        {
            etat = relache;
            
        }
        break;    


  }

}




void my_main(void *)
{

    pinMode(BP, INPUT_PULLDOWN );

    int j = 0;

    // Setup the LCD display
    lcd_begin();

    while (true)
    {
        automate();
        vTaskDelay(10);
       
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
