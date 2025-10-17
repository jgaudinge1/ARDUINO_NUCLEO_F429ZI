// Copyright (c) 2024 Antoine TRAN TAN

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"
#include <STM32_CAN.h>
#include "event_groups.h"

#define RIGHT 0x10
#define CENTRAL 0x04
#define LEFT 0x02
#define UP 0x08
#define DOWN 0x01

EventGroupHandle_t eflags;

STM32_CAN my_can(CAN1, ALT_2);

typedef enum { Button_right, Button_central, Button_left, Button_up, Button_down, start } Button;



void tx_task(void *)
{

    CAN_message_t tx_message_jog;
    tx_message_jog.id = 0x790;
    tx_message_jog.flags.remote = 1;
    tx_message_jog.len = 0;
    tx_message_jog.buf[0] = 0x00;   


    while (true)
    {
        
        my_can.write(tx_message_jog);

        vTaskDelay(25 / portTICK_PERIOD_MS);
    }
}

void rx_task(void *)
{

    Button bout_jog = start;

    CAN_message_t rx_message_jog;
    rx_message_jog.id = 0x791;
    rx_message_jog.flags.remote = 0;
    rx_message_jog.len = 0;
    rx_message_jog.buf[1];

    my_can.write(rx_message_jog);

    while (true)
    {
        if(my_can.read(rx_message_jog))
        {
            lcd_cls();
            lcd_locate(0,10);

            if(rx_message_jog.buf[0] == RIGHT) 
            {
                bout_jog = Button_right;
            }
            else if(rx_message_jog.buf[0] == CENTRAL) 
            {
                bout_jog = Button_central;
                                }
            else if(rx_message_jog.buf[0] == LEFT) 
            {
                bout_jog = Button_left;
            }
            else if(rx_message_jog.buf[0] == UP) 
            {
                bout_jog = Button_up;
            }
            else if(rx_message_jog.buf[0] == DOWN) 
            {
                bout_jog = Button_down;
            }

            switch(bout_jog)
            {
                case start: 
                    lcd_printf("Start");

                    break;
                case Button_right:
                    lcd_printf("Right");
                    xEventGroupSetBits(eflags, 0x01);
                    bout_jog = start;
                    break;  
                case Button_central:
                    lcd_printf("Central");
                    bout_jog = start;
                    break;
                case Button_left:
                    lcd_printf("Left"); 
                    xEventGroupSetBits(eflags, 0x10);
                    bout_jog = start;
                    break;
                case Button_up:
                    lcd_printf("Up");
                    bout_jog = start;
                    break;  
                case Button_down:
                    lcd_printf("Down"); 
                    bout_jog = start;
                    break;
                default:
                    bout_jog = start;
                    break;

            }

            // lcd_printf("Received: 0x %X", rx_message_jog.buf[0]);
        }
        vTaskDelay(25 / portTICK_PERIOD_MS);
    }
}

void my_main(void *)
{

    uint16_t flag;

    int8_t j = 0;

    CAN_message_t tx_message_bar;
    tx_message_bar.id = 0x7B0;
    tx_message_bar.flags.remote = 0;
    tx_message_bar.len = 2; 
    tx_message_bar.buf[2];

    lcd_begin();

    while (true)
    {
        uint16_t motif = 0x0001;

        flag = xEventGroupWaitBits(eflags, 0x11, pdTRUE, pdFALSE, -1);

        if ((flag & 0x01)==0x01)
        {
            j = j -1;
            if(j<0)
            {
                j=9;
            }
        }
        if ((flag & 0x10)==0x10)
        {
            j = j +1;
            if (j>9)
            {
                j=0;
            }
        }

        motif = motif << j;

        tx_message_bar.buf[0] = (uint8_t)(motif >> 8);
        tx_message_bar.buf[1] = (uint8_t)(motif & 0x00FF);

        my_can.write(tx_message_bar);


    }
}

void setup()
{

    my_can.begin();
    my_can.setBaudRate(1000000);

    eflags = xEventGroupCreate();

    xTaskCreate(my_main, "my_main", 1024, NULL, 1, NULL);
    xTaskCreate(tx_task, "tx_task", 1024, NULL, 1, NULL);
    xTaskCreate(rx_task, "rx_task", 1024, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}

void loop()
{
    // Empty. Things are done in my_main
}
