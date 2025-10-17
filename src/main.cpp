// Copyright (c) 2024 Antoine TRAN TAN

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"
#include "STM32_CAN.h"
#include "event_groups.h"

STM32_CAN my_can(CAN1, ALT_2);
CAN_message_t rx_msg;

EventGroupHandle_t eflag;

uint8_t valeur_jog = 0;
int8_t valeur_cod = 0;
int8_t temp_cod;

void my_scrut(void *)
{

    my_can.setMBFilterProcessing(MB0, 0x781, 0x7CF, STD);

    while (true)
    {

        if(my_can.read(rx_msg))
        {

            if (rx_msg.id == 0x791)
            {
                valeur_jog = rx_msg.buf[0];
            }
            else if(rx_msg.id == 0x7A1)
            {

                temp_cod = rx_msg.buf[0];

                if(valeur_cod > temp_cod)
                {
                   xEventGroupSetBits(eflag, 0x08);
                }
                else if(valeur_cod < temp_cod)
                {
                   xEventGroupSetBits(eflag, 0x80);
                }

                valeur_cod = temp_cod;
            }
        }

       
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void my_trame(void *)
{
    bool j = true;

    CAN_message_t tx_msg_jog;
    tx_msg_jog.id = 0x790;
    tx_msg_jog.flags.remote = 1;
    tx_msg_jog.len = 0;
    
    CAN_message_t tx_msg_cod;
    tx_msg_cod.id = 0x7A0;
    tx_msg_cod.flags.remote = 1;
    tx_msg_cod.len = 0;

    while (true)
    {

        if(j==1)
        {
            my_can.write(tx_msg_jog);
        }
        else
        {

            my_can.write(tx_msg_cod);
        }

        j = !j;

        vTaskDelay(100);
    }
}

void my_main(void *)
{

    uint32_t flags;

    int n = 0;
    uint16_t motif = 0x0001;

    CAN_message_t tx_message_bar;
    tx_message_bar.id = 0x7B0;
    tx_message_bar.flags.remote = 0;
    tx_message_bar.len = 2;
    
    lcd_begin();

    while (true)
    {

        flags = xEventGroupWaitBits(eflag, 0x88, pdTRUE, pdFALSE, -1);

        if((flags & 0x80) == 0x80)
        {
            n++;
            if(n>9)
            {
                n =0;
            }
            
        }
        else if ((flags & 0x08) == 0x08)
        {
            n = n -1;
            if(n<0)
            {
                n=9;
            }
        }

        motif = 0x3FF >> (10 - n);

        tx_message_bar.buf[0] = (uint8_t)((motif & 0xFF00) >> 8);
        tx_message_bar.buf[1] = (uint8_t)(motif & 0x00FF);

        my_can.write(tx_message_bar);

        lcd_cls();

        lcd_locate(0, 10);
        lcd_printf("JOG: %X", valeur_jog);

        lcd_locate(0, 20);
        lcd_printf("COD: %d", valeur_cod);
        
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void setup()
{

    my_can.begin();
    my_can.setBaudRate(1000000);

    eflag = xEventGroupCreate();

    xTaskCreate(my_main, "my_main", 1024, NULL, 1, NULL);
    xTaskCreate(my_trame, "my_trame", 1024, NULL, 1, NULL);
    xTaskCreate(my_scrut, "my_scrut", 1024, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}

void loop()
{
    // Empty. Things are done in my_main
}
