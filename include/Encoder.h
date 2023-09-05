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
    void iniciar();
    void encoderLoop();
    IRAM_ATTR void checkPosition();

private:
    void pitido(int tono);
    static void staticCheckPosition(); // Función estática para la interrupción
    void interrumpirAnimaciones(int dir);
};

#endif