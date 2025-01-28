// Copyright (c) 2025 Antoine TRAN TAN

#ifndef IHM_H
#define IHM_H

#include <STM32_CAN.h>

#define BAR_SET 0x7B0
#define JOG_REQ 0x790
#define JOG_DATA 0x791
#define COD_REQ 0x7A0
#define COD_DATA 0x7A1

#define FLAG_JOG 0x01
#define FLAG_COD 0x02

class IHM
{
public:
    IHM();
    ~IHM();

    void bargraph(uint16_t leds);
    uint8_t getJog(void);

private:
    static STM32_CAN my_can;
    static CAN_message_t RxMessage;
};

#endif
