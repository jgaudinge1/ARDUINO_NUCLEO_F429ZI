// Copyright (c) 2024 Antoine TRAN TAN

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"
#include <Wire.h>
#include "HardwareSerial.h"
#include "event_groups.h"

EventGroupHandle_t eflag;


bool celsius_ou_kelvin;

void serialEvent()
{
    char recu;

    recu = Serial.read();

    if (recu == 'c')
    {
        celsius_ou_kelvin = !celsius_ou_kelvin;
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
    char paquet_rx[2] = {0x01, 0x00};

    int16_t temperature = 0;
    float temp;

    Wire.beginTransmission(0x48);
    Wire.write(paquet_rx, 2);
    Wire.endTransmission(true);

    // Setup the LCD display
    lcd_begin();

    while (true)
    {
        lcd_cls();

        Wire.beginTransmission(0x48);
        Wire.write(0x00);
        Wire.endTransmission(false);

        Wire.requestFrom(0x48, 2);

        paquet_rx[0] = Wire.read();
        paquet_rx[1] = Wire.read();

        temperature = (int16_t)paquet_rx[0]<<8;
        temperature = temperature|(int16_t)paquet_rx[1];
        temperature = temperature >> 5;

        temp = (float)temperature * 0.125;

        if(celsius_ou_kelvin==1)
        {
            temp = temp+ 271.15;
            lcd_cls();
            lcd_locate(0, 10);
            lcd_printf("temp = %.2f dK", temp);
            Serial.printf("temp = %.2f dK", temp);
            Serial.printf("\n");
        }
        else
        {
            lcd_cls();
            lcd_locate(0, 10);
            lcd_printf("temp  = %.2f dC", temp);
            Serial.printf("temp = %.2f dC", temp);
            Serial.printf("\n");
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup()
{

    eflag = xEventGroupCreate();

    Wire.begin();
    Wire.setClock(100000);

    xTaskCreate(my_main, "my_main", 1024, NULL, 1, NULL);
    xTaskCreate(main_producteur, "main_producteur", 1024, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}

void loop()
{
    // Empty. Things are done in my_main
}
