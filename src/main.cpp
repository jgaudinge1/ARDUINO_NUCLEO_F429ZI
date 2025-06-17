// Copyright (c) 2024 Antoine TRAN TAN

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"
#include "analog.h"
#include "HardwareTimer.h"
#include "event_groups.h"

#define push PF14


int v = 1;

uint32_t duree;

EventGroupHandle_t eflag;

void my_isr_rise_fall()
{

    static uint32_t start = 0;

    if (digitalRead(push))
    {
        start = millis();
    }
    if (!digitalRead(push))
    {
        duree = millis() - start;

        if (duree > 100)
        {
            v = v * (-1);
            xEventGroupSetBitsFromISR(eflag, 0x01, NULL);
        }
        else if (duree < 100)
        {
             xEventGroupSetBitsFromISR(eflag, 0x01, NULL);
        }
    }
}

void my_hwtimer_isr()
{

    int16_t sample;

    float vpot;

    vpot = analogRead(A0);

    sample = analogRead(PF7);

    sample = sample - 2048;
    sample = (float)sample * (vpot / 4096.0) * (float)v;
    sample = sample + 2048;

    if (sample > 4095)
    {
        sample = 4095;
    }
    else if (sample < 0)
    {
        sample = 0;
    }

    analogWrite(PA4, sample);
}

void my_consommateur(void *)
{
    uint32_t flag_read;

    while(true)
    {
        flag_read = xEventGroupWaitBits(eflag, 0x01, true, false, -1);

        lcd_cls();
        lcd_locate(0,20);
        lcd_printf("duree: %d ms", duree);
        duree = 0;
    }
}

void my_main(void *)
{
    HardwareTimer tim(TIM1);

    tim.attachInterrupt(&my_hwtimer_isr);
    tim.setOverflow(1000, MICROSEC_FORMAT);
    tim.resume();

    analogReadResolution(12);
    analogWriteResolution(12);

    // Setup the LCD display
    lcd_begin();

    while (true)
    {

        lcd_locate(0, 10);
        lcd_printf("TP numero 6");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup()
{

    pinMode(PF7, INPUT_ANALOG);
    pinMode(A0, INPUT_ANALOG);
    pinMode(PF14, INPUT_PULLDOWN);
    pinMode(PA4, OUTPUT);

    attachInterrupt(push, &my_isr_rise_fall, CHANGE);

    eflag = xEventGroupCreate();

    xTaskCreate(my_main, "my_main", 1024, NULL, 1, NULL);
    xTaskCreate(my_consommateur, "my_consommateur", 1024, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}

void loop()
{
    // Empty. Things are done in my_main
}
