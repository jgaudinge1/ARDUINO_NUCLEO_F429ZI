#ifndef RGB_H
#define RGB_H
#include <Arduino.h>
#include "couleur.h"

class RGB
{

    public:
        RGB()
        {
            pinMode(PE11, OUTPUT); // Rouge
            pinMode(PD15, OUTPUT); // Vert
            pinMode(PF12, OUTPUT); // Bleu
        }

        void LEDon(T_couleur Value)
        {
            if(Value == rouge)
            {
                digitalWrite(PE11, 0);
                digitalWrite(PD15, 1);
                digitalWrite(PF12, 1);
            }
            else if(Value == vert)
            {
                digitalWrite(PE11, 1);
                digitalWrite(PD15, 0);
                digitalWrite(PF12, 1);
            }
            else if(Value == bleu)
            {
                digitalWrite(PE11, 1);
                digitalWrite(PD15, 1);
                digitalWrite(PF12, 0);
            }
            else
            {
                digitalWrite(PE11, 1);
                digitalWrite(PD15, 1);
                digitalWrite(PF12, 1);
            }
        }

        void LEDoff(void)
        {
                digitalWrite(PE11, 1);
                digitalWrite(PD15, 1);
                digitalWrite(PF12, 1);
        }


    private:

};

#endif