// Copyright (c) 2024 Antoine TRAN TAN

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"
#include <Wire.h>

void my_main(void *)
{
    char paquet_tx1[2] = {0x07, 0x00};
    char paquet_tx2[2] = {0x08, 0x21};
    char paquet_tx3[2] = {0x07, 0x01};
    char paquet_rx[3];

    int8_t x;
    int8_t y;
    int8_t z;

    float xed;
    float yed;
    float zed;

    Wire.beginTransmission(0x4C);
    Wire.write(paquet_tx1, 2);
    Wire.endTransmission(true);

    Wire.beginTransmission(0x4C);
    Wire.write(paquet_tx2, 2);
    Wire.endTransmission(true);

    Wire.beginTransmission(0x4C);
    Wire.write(paquet_tx3, 2);
    Wire.endTransmission(true);

    lcd_begin();

    while (true)
    {

        lcd_cls();

        Wire.beginTransmission(0x4C);
        Wire.write(0x00);
        Wire.endTransmission(false);

        Wire.requestFrom(0x4C, 3);

        
        paquet_rx[0] = Wire.read();

        // Wire.beginTransmission(0x4C);
        // Wire.write(0x01);
        // Wire.endTransmission(false);

        // Wire.requestFrom(0x4C, 1);
        paquet_rx[1] = Wire.read();

        // Wire.beginTransmission(0x4C);
        // Wire.write(0x02);
        // Wire.endTransmission(false);

        // Wire.requestFrom(0x4C, 1);
        paquet_rx[2] = Wire.read();


        x = paquet_rx[0];
        y = paquet_rx[1];
        z = paquet_rx[2];

        x = x << 2;
        x = x >> 2;
        xed = (float)x*(1.5/32);

        y = y << 2;
        y = y >> 2;
        yed = (float)y*(1.5/32);

        z = z << 2;
        z = z >> 2;
        zed = (float)z*(1.5/32);

        lcd_locate(0, 10);
        lcd_printf("val_x : %.2f ", xed); 
        lcd_locate(0, 20);
        lcd_printf("val_y : %.2f ", yed);
        lcd_locate(0, 30);
        lcd_printf("val_z : %.2f ", zed);


        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup()
{

    Wire.begin();
    Wire.setClock(100000);


    xTaskCreate(my_main, "my_main", 1024, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}

void loop()
{
    // Empty. Things are done in my_main
}
