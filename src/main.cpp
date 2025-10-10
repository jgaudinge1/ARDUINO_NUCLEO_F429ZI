// Copyright (c) 2024 Antoine TRAN TAN

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <C12832.h>
#include "my_header.h"
#include "SPI.h"
#include "event_groups.h"


// Groupe d'événements pour la synchronisation entre tâches et interruptions
EventGroupHandle_t eflags;

// Instance de la classe SPI
SPIClass my_spi;


// Fonction appelée lors de l'interruption sur PF8
void my_interrupt(void)
{
    // Signale l'événement à la tâche principale
    xEventGroupSetBitsFromISR(eflags, 0x01, NULL);
    // Petite temporisation pour éviter les rebonds
    vTaskDelay(8/portTICK_PERIOD_MS);
}


// Tâche principale exécutée par FreeRTOS
void my_main(void *)
{
    uint32_t flags;

    // Tableaux de paquets à transmettre via SPI
    char paquet_tx[4][4] = {
        {0x02, 0x00, 0x08, 0x00},
        {0x02, 0x02, 0x7A, 0x00},
        {0x02, 0x03, 0x20, 0x00},
        {0x02, 0x0B, 0x2F, 0x2F}
    };

    // Transmission des paquets SPI avec synchronisation sur l'interruption
    for(int i = 0; i < 4; i++)
    {
        digitalWrite(PE4, LOW); // Sélection du périphérique SPI
        my_spi.transfer(paquet_tx, 4, SPI_TRANSMITONLY); // Transmission du paquet
        flags = xEventGroupWaitBits(eflags, 0x01, pdTRUE, pdFALSE, -1); // Attente de l'interruption
        digitalWrite(PE4, HIGH); // Désélection du périphérique
    }

    vTaskDelay(1000/portTICK_PERIOD_MS); // Attente avant d'initialiser l'écran

    // Initialisation de l'écran LCD
    lcd_begin();

    // Boucle d'affichage sur l'écran LCD
    while (true)
    {
        lcd_cls(); // Efface l'écran
        lcd_locate(0, 10); // Positionne le curseur
        lcd_printf("TP4 Gaudin Jonathan"); // Affiche le texte
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Rafraîchissement toutes les secondes
    }
}


// Fonction d'initialisation Arduino
void setup()
{
    // Configuration des broches
    pinMode(PE4, OUTPUT); // Chip select SPI
    digitalWrite(PE4, HIGH); // Désélection initiale
    pinMode(PD12, OUTPUT); // Broche supplémentaire
    digitalWrite(PD12, HIGH);
    pinMode(PF8, INPUT); // Broche d'interruption

    // Attache l'interruption sur PF8
    attachInterrupt(PF8, my_interrupt, RISING);

    // Configuration SPI
    SPISettings settings(100000, MSBFIRST, SPI_MODE0);
    my_spi.begin();
    my_spi.beginTransaction(settings);

    // Création du groupe d'événements pour la synchronisation
    eflags = xEventGroupCreate();

    // Création de la tâche principale
    xTaskCreate(my_main, "my_main", 1024, NULL, 1, NULL);

    // Démarrage du scheduler FreeRTOS
    vTaskStartScheduler();
}


// Boucle principale Arduino (non utilisée ici)
void loop()
{
    // Vide. Tout est géré dans la tâche my_main
}
