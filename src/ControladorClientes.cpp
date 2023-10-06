#include "ControladorClientes.h"
#include "Definiciones/debug.h"

ControladorClientes::ControladorClientes()
{
    for (int i = 0; i < maxClientes; i++)
    {
        clientes[i] = Cliente(IPAddress(0, 0, 0, 0), 0); // Inicializar con valores predeterminados
    }
}

void ControladorClientes::PosibleClienteNuevo(IPAddress ip, uint16_t port)
{
    bool existe = false;

    if (!YaExiste(ip, port))
    {
        CrearCliente(ip, port);
    }
}

void ControladorClientes::CrearCliente(IPAddress ip, uint16_t port)
{
    int ultimaPos = BuscarUltimoLugar();
    if (ultimaPos < maxClientes)
    {
        Cliente nuevoCliente = Cliente(ip, port);
        nuevoCliente.setId(ultimaPos);
        clientes[ultimaPos] = nuevoCliente;
    }
}

bool ControladorClientes::YaExiste(IPAddress ip, uint16_t port)
{
    bool yaExiste = false;
    for (int i = 0; i < maxClientes; i++)
    {
        if (clientes[i].getIp() == ip) //&& clientes[i].getPort() == port)
        {
            yaExiste = true;
            i = maxClientes;
        }
    }
    return yaExiste;
}

int ControladorClientes::BuscarUltimoLugar()
{
    int ultimaPos;
    for (int i = 0; i < maxClientes; i++)
    {
        if (clientes[i].getIp() == IPAddress(0, 0, 0, 0))
        {
            ultimaPos = i;
            i = maxClientes;
        }
    }
    return ultimaPos;
}

void ControladorClientes::MostrarListaSiNuevo()
{
    bool mostrar = false;

    for (int i = 0; i < maxClientes; i++)
    {
        if (clientes[i].getIp() != clientesAnteriores[i].getIp() || clientes[i].getPort() != clientesAnteriores[i].getPort())
        {
            mostrar = true;
        }
    }

    if (mostrar)
    {
        MostrarLista();
    }
}

void ControladorClientes::MostrarLista()
{
    debugPrintln("------Lista Clientes------");
    for (int i = 0; i < maxClientes; i++)
    {
        if (clientes[i].getIp() != IPAddress(0, 0, 0, 0))
        {
            clientes[i].mostrarse();
            clientesAnteriores[i] = clientes[i];
        }
    }
    debugPrintln("--------------------------");

    Pitido(5000);
}

void ControladorClientes::Pitido(int tono)
{
    tone(BUZZER_PIN, tono, 0.9);
    delay(50);
    noTone(BUZZER_PIN);
}