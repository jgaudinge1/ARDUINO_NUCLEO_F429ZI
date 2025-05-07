// Copyright (c) 2024 Antoine TRAN TAN
#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"
bool btn_UP = 0;
bool btn_DOWN = 0;
bool btn_LEFT = 0;
bool btn_RIGHT = 0;
typedef enum {appui, relache} etat_type;


void task_UP(void *)
{

    while(true) 
    {
            static etat_type etat = relache;
        switch (etat)
        {
            case relache:
                if (digitalRead(PC3) == 0)
                {
                    etat = appui;
                }
            break;
            case appui:
                if (digitalRead(PC3) == 1)
                {
                    btn_UP = 1;
                    etat = relache;
                }
            break;
            vTaskDelay(100/portTICK_PERIOD_MS);
        }
    }
    
}
void task_DOWN(void *)
{

    
    while(true) 
    {
        static etat_type etat = relache;
        switch (etat)
        {
            case relache:
                if (digitalRead(PD11) == 0)
                {
                    etat = appui;
                }
            break;
            case appui:
                if (digitalRead(PD11) == 1)
                {
                    btn_DOWN  = 1;
                    etat = relache;
                }
            break;
            
        }

        vTaskDelay(100/portTICK_PERIOD_MS);
    }
    
}
void task_DROITE(void *)
{
    while(true) 
    {
            static etat_type etat = relache;
        switch (etat)
        {
            case relache:
                if (digitalRead(PF10) == 0)
                {
                    etat = appui;
                }
            break;
            case appui:
                if (digitalRead(PF10) == 1)
                {
                    btn_RIGHT  = 1;
                    etat = relache;
                }
            break;
        
        }
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
    
}
void task_GAUCHE(void *)
{
    while(true) 
    {
        static etat_type etat = relache;
        switch (etat)
        {
            case relache:
                if (digitalRead(PF5) == 0)
                {
                    etat = appui;
                }
            break;
            case appui:
                if (digitalRead(PF5) == 1)
                {
                    btn_LEFT  = 1;
                    etat = relache;
                }
            break;
            
        }
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}



void my_main(void *)
{
    typedef enum {depart, d1, d2, d3, d4, d5, victoire, attd0, attd1, attd2, attd3, attd4, reset} etat_jeu;
    static etat_jeu etat = depart;


    digitalWrite(PB14, 1);

    // Setup the LCD display
    lcd_begin();
    while (true)
    {
       
            switch(etat)
            {
                case depart:
                    digitalWrite(PB14, 0);
                    digitalWrite(PB0, 1);
                    if (btn_LEFT == 1)
                    {
                        digitalWrite(PB14, 1);
                        btn_LEFT = 0;
                        etat = attd0;
                    }
                    break;
                case attd0:
                    if (btn_RIGHT == 0 && btn_LEFT == 0 && btn_UP == 0 && btn_DOWN == 0)
                    {
                        etat = d1;
                    }
                    break;
                case d1:
                    if (btn_RIGHT == 1)
                    {
                        etat = attd1;
                        btn_RIGHT = 0;
                    }
                    else if (btn_LEFT == 1 || btn_UP == 1 || btn_DOWN == 1)
                    {
                        etat = reset;
                    }
                    break;
                case attd1:
                    if (btn_RIGHT == 0 && btn_LEFT == 0 && btn_UP == 0 && btn_DOWN == 0)
                    {
                        etat = d2;
                    }
                    break;
                case d2:
                    if (btn_UP == 1)
                    {
                        etat = attd2;
                        btn_UP = 0;
                    }
                    else if (btn_LEFT == 1 || btn_RIGHT == 1 || btn_DOWN == 1)
                    {
                        etat = reset;
                        btn_LEFT = 0;
                        btn_RIGHT = 0;
                        btn_DOWN = 0;
                    }
                    break;
                case attd2:
                    if (btn_RIGHT == 0 && btn_LEFT == 0 && btn_UP == 0 && btn_DOWN == 0)
                    {
                        etat = d3;
                    }
                    break;
                case d3:
                    if (btn_DOWN == 1)
                    {
                        etat = attd3;
                        btn_DOWN = 0;
                    }
                    else if (btn_LEFT == 1 || btn_UP == 1 || btn_RIGHT == 1)
                    {
                        etat = reset;
                        btn_LEFT = 0;
                        btn_RIGHT = 0;
                        btn_UP = 0;
                    }
                    break;
                case attd3:
                    if (btn_RIGHT == 0 && btn_LEFT == 0 && btn_UP == 0 && btn_DOWN == 0)
                    {
                        etat = d4;
                    }
                    break;
                case d4:
                    if (btn_RIGHT == 1)
                    {
                        etat = attd4;
                        btn_RIGHT = 0;
                    }
                    else if (btn_LEFT == 1 || btn_UP == 1 || btn_DOWN == 1)
                    {
                        etat = reset;
                        btn_LEFT = 0;
                        btn_DOWN = 0;
                        btn_DOWN = 0;
                    }
                    break;
                case attd4:
                    if (btn_RIGHT == 0 && btn_LEFT == 0 && btn_UP == 0 && btn_DOWN == 0)
                    {
                        etat = victoire;
                    }
                    break;
                case reset:
                    if (btn_RIGHT == 0 && btn_LEFT == 0 && btn_UP == 0 && btn_DOWN == 0)
                    {
                        etat = depart;
                    }
                    break;
                case victoire:
                    digitalWrite(PB0, 1);
                    digitalWrite(PB7, 1);
                    digitalWrite(PB14, 1);
                    vTaskDelay(5000);
                    digitalWrite(PB0, 0);
                    digitalWrite(PB7, 0);
                    digitalWrite(PB14, 0);
                    etat = depart;
                    break;
               
            }
       
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

void setup()
{

    pinMode (PC3, INPUT_PULLDOWN);
    pinMode (PD11, INPUT_PULLDOWN);
    pinMode (PF10, INPUT_PULLDOWN);
    pinMode (PF14, INPUT_PULLDOWN);
    pinMode (PF5, INPUT_PULLDOWN);
    pinMode (PB0, OUTPUT);
    pinMode (PB7, OUTPUT);
    pinMode (PB14, OUTPUT);

    xTaskCreate(my_main, "my_main", 1024, NULL, 1, NULL);
    xTaskCreate(task_DOWN, "task_DOWN", 1024, NULL, 1, NULL);
    xTaskCreate(task_DROITE, "task_Droite", 1024, NULL, 1, NULL);
    xTaskCreate(task_GAUCHE, "task_GAUCHE", 1024, NULL, 1, NULL);
    xTaskCreate(task_UP, "task_UP", 1024, NULL, 1, NULL);
    // Start the scheduler
    vTaskStartScheduler();
}
void loop()
{
    // Empty. Things are done in my_main
}