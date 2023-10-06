#ifndef CRT_BOTONES_H
#define CRT_BOTONES_H

#include "Boton.h"
#include "ControladorClientes.h"
#include "ControladorUDP.h"
#include "Definiciones/gpio.h"

#include <list>

class ControladorBotones
{
public:
    ControladorBotones();
    void IniciarControlador();
    bool BotonPresionadoYSoltado(int i);
    String GetMensajeBotonBoton(int i);
    long TiempoUltimaAccion();

    void BuscarPresionadosV2();

    /* void BuscarPresionados(ControladorClientes &controlClientes, ControladorUDP &controladorUDP); */
    void BuscarGiroEncoder(ControladorClientes &controlClientes, ControladorUDP &controladorUDP);

    // Busca botones presionados y permite combinacion de ellos.
    void BuscarPresionados_V2(ControladorClientes &controlClientes, ControladorUDP &controladorUDP);

private:
    long _tiempoUltimaAccion = 0;
    void SetTiempoUltimaAccion();
    void aMimir();

    const char *_mensajes[11] = {
        "A", // GPIO13
        "B", // GPIO12
        "C", // GPIO14
        "D", // GPIO27
        "E", // GPIO26
        "F", // GPIO25
        "G", // GPIO33
        "H", // GPIO32
        "I", // GPIO4 Encoder switch
        "J", // GPIO2 Encoder giro horario
        "K", // GPIO15 Encoder giro antihorario
    };
    Boton botones[cantBotones];

    std::list<String> _botonesPresionados;
    bool EstaEnLaLista(std::list<String> lista, String elemento);
    bool EsElPrimerBotonDeLaCombinacion(std::list<String> lista, String elemento);
    bool esCombinacion = false;

    // Variable usada para detectar cambios de giro en encoder.
    int giroAnterior = 0;
};

#endif