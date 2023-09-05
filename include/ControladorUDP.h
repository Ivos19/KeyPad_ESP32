#ifndef CONTROLADORUDP_H
#define CONTROLADORUDP_H

#include <WiFi.h>
#include <WiFiUdp.h>

#include "Definiciones/constantes.h"
#include "ControladorUDP.h"
#include "ControladorClientes.h"
#include "Cliente.h"

class ControladorUDP
{
public:
    WiFiUDP udp;
    Cliente posibleCliente;
    Cliente Escuchar();
    void iniciar();
    void buscarCliente(ControladorClientes &cc);
    void responder_a_Clientes(ControladorClientes &cc, const char *mensaje);

private:
    void enviarMensaje(Cliente c, char const *mensaje);
};

#endif