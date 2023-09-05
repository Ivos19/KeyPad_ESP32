#include <WiFi.h>
#include "Cliente.h"
#include "Definiciones/debug.h"
//  Definición de la clase

// Constructor
Cliente::Cliente()
{
    setIp(IPAddress(0, 0, 0, 0));
}

Cliente::Cliente(IPAddress i = IPAddress(0, 0, 0, 0), uint16_t p = 0)
{
    setIp(i);
    setPort(p);
}

// Funciones miembro
void Cliente::CompletarCliente(IPAddress ipSended, uint16_t portSended)
{
    setIp(ipSended);
    setPort(portSended);
}

// Se muestra sus datos por Serie.
void Cliente::mostrarse()
{
    debugPrint("IP Cliente: ");
    debugPrint(getIp());
    debugPrint(" | Puerto: ");
    debugPrint(getPort());
    debugPrint(" | Id:  ");
    Serial.println(getId());
}

// Setters
void Cliente::setId(int i)
{
    id = i;
}
void Cliente::setIp(IPAddress ipSended)
{
    ip = ipSended;
}
void Cliente::setPort(uint16_t portSended)
{
    port = portSended;
}
// Getters
int Cliente::getId()
{
    return id;
}
IPAddress Cliente::getIp()
{
    return ip;
}
uint16_t Cliente::getPort()
{
    return port;
}
