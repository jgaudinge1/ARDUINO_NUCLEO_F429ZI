// Copyright (c) 2024 Antoine TRAN TAN

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"
#include "couleur.h"
#include "RGB.h"
#include "event_groups.h"

EventGroupHandle_t eflag;

T_couleur couleur_lue;

RGB rgb;

T_couleur LedSequence[5];

void serialEvent()
{
    char recu;

    recu = Serial.read();

    if (recu == 'r')
    {
        couleur_lue = rouge;
    }
    else if (recu == 'g')
    {
        couleur_lue = vert;
    }
    else if (recu == 'b')
    {
        couleur_lue = bleu;
    }

    if (recu == 'r' || recu == 'g' || recu == 'b')
    {
        xEventGroupSetBits(eflag, 0x01);
    }
}

void main_producteur(void *)
{

    Serial.begin(115200, SERIAL_8N1);

    while (true)
    {
        serialEventRun();

        vTaskDelay(50);
    }
}

void my_main(void *)
{

    uint32_t flags;

    int numero_tour = 1;

    lcd_begin();

    for (int i = 0; i < 5; i++)
    {
        LedSequence[i] = (T_couleur)(rand() % 3);
    }

    while (true)
    {

        for (int i = 0; i < numero_tour; i++)
        {
            rgb.LEDon(LedSequence[i]);
            vTaskDelay(400);
            rgb.LEDoff();
            vTaskDelay(400);
        }

        for (int j = 0; j < numero_tour; j++)
        {
            flags = xEventGroupWaitBits(eflag, 0x01, pdTRUE, pdFALSE, -1);
            if (flags == 0x01)
            {
                if (couleur_lue != LedSequence[j])
                {
                    numero_tour = 5;
                    break;
                }
            }
        }

        if (numero_tour < 5)
        {
            numero_tour++;
        }
        else
        {
            numero_tour = 1;
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void setup()
{

    eflag = xEventGroupCreate();

    xTaskCreate(my_main, "my_main", 1024, NULL, 1, NULL);
    xTaskCreate(main_producteur, "main_producteur", 1024, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}

void loop()
{
    // Empty. Things are done in my_main
}
