#ifndef CONTROLADORCLIENTES_H
#define CONTROLADORCLIENTES_H

#include "Cliente.h"
#include "Definiciones/constantes.h"
#include "Definiciones/gpio.h"

class ControladorClientes
{
public:
    Cliente clientes[maxClientes];

    ControladorClientes();
    void posibleClienteNuevo(IPAddress ip, uint16_t port);
    void mostrarListaSiNuevo();
    void CrearCliente(IPAddress i, uint16_t p);
    void mostrarLista();

private:
    void pitido(int tono);
    int BuscarUltimoLugar();
    bool yaExiste(IPAddress ip, uint16_t port);

    Cliente clientesAnteriores[maxClientes];
};

#endif