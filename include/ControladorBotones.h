#ifndef CRT_BOTONES_H
#define CRT_BOTONES_H

#include "Boton.h"
#include "ControladorClientes.h"
#include "ControladorUDP.h"
#include "Definiciones/gpio.h"

class ControladorBotones
{
public:
    ControladorBotones();
    void iniciarControlador();
    bool botonPresionadoYSoltado(int i);

    void buscarPresionados(ControladorClientes &controlClientes, ControladorUDP &controladorUDP);
    void buscarGiroEncoder(ControladorClientes &controlClientes, ControladorUDP &controladorUDP);

private:
    const char *_mensajes[11] = {
        "A", // GPIO13
        "B", // GPIO12
        "C", // GPIO14
        "D", // GPIO27
        "E", // GPIO26
        "F", // GPIO25
        "G", // GPIO33
        "H", // GPIO32
        "I", // GPIO4 Encoder switch e
        "J", // GPIO2 Encoder giro horario
        "K", // GPIO15 Encoder giro antihorario
    };
    Boton botones[cantBotones];

    // Variable usada para detectar cambios de giro en encoder.
    int giroAnterior = 0;
};

#endif