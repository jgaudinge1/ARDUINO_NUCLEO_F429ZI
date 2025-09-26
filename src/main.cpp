// Copyright (c) 2024 Antoine TRAN TAN

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"
#include <event_groups.h>
#include <SPI.h>

EventGroupHandle_t eflags;

SPIClass my_spi(PE6, PE5, PE2);

uint8_t buf_mosi[122];
uint8_t buf_miso[122];

int16_t pixels[60];
float new_pixels;

char read_status_MOSI[3]= {0x00, 0x80, 0xFF}; 
char read_status_MISO[3];
char setup_array[12][3] = { {0x01, 0x00, 0x04}, {0x02, 0x00, 0x02}, {0x03, 0x00, 0x76}, {0x04, 0x00, 0x90}, {0x05, 0x00, 0x08}, {0x06, 0x00, 0x0F}, {0xA5, 0x00, 0x88}, {0xA6, 0x00, 0x88}, {0xA7, 0x00, 0x88}, {0xA8, 0x00, 0x88}, {0xA9, 0x00, 0x88}, {0xC1, 0x00, 0x02}};


void my_callback(void)
{
    xEventGroupSetBitsFromISR(eflags, 0x01, NULL);
}


void my_main(void *)
{
    attachInterrupt(PD12, my_callback, FALLING);

    lcd_begin();

    for(int i = 0; i<122; i++)
        {
            buf_mosi[i] = 0xFF;
        }
    buf_mosi[0] = 0x10;
    buf_mosi[1] = 0x80;

    for(int i = 0; i<12; i++)
        {
            digitalWrite(PE4, LOW);
            my_spi.transfer(setup_array[i], 3, SPI_TRANSMITONLY);
            digitalWrite(PE4, HIGH);
        }
    vTaskDelay(100 / portTICK_PERIOD_MS);

    digitalWrite(PE4, LOW);
    my_spi.transfer(read_status_MOSI, read_status_MISO, 3);
    digitalWrite(PE4, HIGH);

    while (true)
    {
        xEventGroupWaitBits(eflags, 0x01, true, false, -1);

        digitalWrite(PE4, LOW);
        my_spi.transfer(buf_mosi, buf_miso, 122);
        digitalWrite(PE4, HIGH);

        for(int i = 0; i<60; i++)
        {
            pixels[i] = (int16_t)buf_miso[2*i+2]<<8 | buf_miso[2*i+3];
        }
        
        Serial.println("st\n");

        for(int i = 0; i<60; i++)
        {
            // if(pixels[i] < 0)
            // {
            //     Serial.println("0.000\n");
                
            // }
            // else if(pixels[i] > 0)
            // {
            //     Serial.println("1.000\n");
            // }

            new_pixels = (((float)pixels[i]/32768)+1)/2;

            Serial.printf("%0.3f \n", new_pixels);
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

// 

void setup()
{
    pinMode(PE4, OUTPUT);
    digitalWrite(PE4, HIGH);
    pinMode(PD12, INPUT);
    
    Serial.begin(115200);

    eflags = xEventGroupCreate();

    SPISettings settings (100000, MSBFIRST, SPI_MODE0);

    my_spi.begin();
    my_spi.beginTransaction(settings);


    xTaskCreate(my_main, "my_main", 1024, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}

void loop()
{
    // Empty. Things are done in my_main
}
