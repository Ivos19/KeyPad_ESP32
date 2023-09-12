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
    void PosibleClienteNuevo(IPAddress ip, uint16_t port);
    void MostrarListaSiNuevo();
    void CrearCliente(IPAddress i, uint16_t p);
    void MostrarLista();

private:
    void Pitido(int tono);
    int BuscarUltimoLugar();
    bool YaExiste(IPAddress ip, uint16_t port);

    Cliente clientesAnteriores[maxClientes];
};

#endif