#include <WiFi.h>

#ifndef Cliente_H
#define Cliente_H

class Cliente
{
public:
    Cliente();
    Cliente(IPAddress ipSended, uint16_t portSended);

    void mostrarse();

    // Funciones miembro
    void CompletarCliente(IPAddress ipSended, uint16_t portSended);

    // Setters
    void setId(int i);
    void setIp(IPAddress ipSended);
    void setPort(uint16_t portSended);
    // Getters
    int getId();
    IPAddress getIp();
    uint16_t getPort();

private:
    int id;
    IPAddress ip;
    int port;
};

#endif