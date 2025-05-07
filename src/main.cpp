// Copyright (c) 2024 Antoine TRAN TAN

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"

#define red PB14
#define green PB0
#define blue PB7

void TaskLed1(void *)
{
  while(true)
  {
    digitalToggle(green);
    vTaskDelay(250/ portTICK_PERIOD_MS);
    digitalToggle(green);
  vTaskDelay(250/ portTICK_PERIOD_MS);
  }
}

void TaskLed2(void *)
{
  while(true)
  {
    digitalToggle(blue);
    vTaskDelay(500/ portTICK_PERIOD_MS);
    digitalToggle(blue);
    vTaskDelay(500/ portTICK_PERIOD_MS);
  }
}

void TaskLed3(void *)
{
  while(true)
  {
    digitalToggle(red);
    vTaskDelay(1000/ portTICK_PERIOD_MS);
    digitalToggle(red);
    vTaskDelay(1000/ portTICK_PERIOD_MS);
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

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  digitalWrite(red, 0);
  digitalWrite(green, 0);
  digitalWrite(blue, 0);

    xTaskCreate(my_main, "my_main", 1024, NULL, 1, NULL);

    xTaskCreate(TaskLed1, "TaskLed1", 1024, NULL, 1, NULL);

    xTaskCreate(TaskLed2, "TaskLed2", 1024, NULL, 1, NULL);

    xTaskCreate(TaskLed3, "TaskLed3", 1024, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}

void loop()
{
    // Empty. Things are done in my_main
}
