#ifndef ENCODER_H
#define ENCODER_H

#include <RotaryEncoder.h>
#include "Definiciones/gpio.h"
#include "Definiciones/constantes.h"
#include <driver/ledc.h>

class Encoder
{
public:
    Encoder();
    void Iniciar();
    void EncoderLoop();
    IRAM_ATTR void CheckPosition();

private:
    void Pitido(int tono);
    static void StaticCheckPosition(); // Función estática para la interrupción
    void InterrumpirAnimaciones(int dir);
};

#endif