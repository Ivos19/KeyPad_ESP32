#include "ControladorUDP.h"
#include "Definiciones/debug.h"

void ControladorUDP::iniciar()
{
    udp.begin(localUdpPort);
    Serial.printf("| Now listening at IP %s, UDP port %d |\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

Cliente ControladorUDP::Escuchar()
{
    int packetSize = udp.parsePacket();
    Cliente cl;
    if (packetSize)
    {
        Serial.printf("| Received %d bytes from %s, port %d | ", packetSize, udp.remoteIP().toString().c_str(), udp.remotePort());

        char packetBuffer[255]; // Buffer para almacenar los datos recibidos
        int len = udp.read(packetBuffer, sizeof(packetBuffer));
        if (len > 0)
        {
            packetBuffer[len] = '\0'; // Agregar terminador nulo al final de los datos
            Serial.printf("| UDP packet contents: %s | \n", packetBuffer);
        }
        cl = Cliente(udp.remoteIP(), udp.remotePort());
    }
    return cl;
}

void ControladorUDP::buscarCliente(ControladorClientes &cc)
{
    Cliente nuevoCliente = Escuchar();

    if (nuevoCliente.getIp() != IPAddress(0, 0, 0, 0))
    {
        cc.PosibleClienteNuevo(nuevoCliente.getIp(), nuevoCliente.getPort());
    }
}

void ControladorUDP::responder_a_Clientes(ControladorClientes &cc, char const *mensaje)
{
    for (size_t i = 0; i < maxClientes; i++)
    {
        if (cc.clientes[i].getIp() != IPAddress(0, 0, 0, 0))
        {
            enviarMensaje(cc.clientes[i], mensaje);
        }
    }
}

void ControladorUDP::enviarMensaje(Cliente c, char const *mensaje)
{
    udp.beginPacket(c.getIp(), puertodeRespuesta);
    udp.write((const uint8_t *)mensaje, strlen(mensaje));
    udp.endPacket();

    debugPrint("Mensaje enviado: " + String(mensaje));
    debugSleepPrint("Mensaje enviado: " + String(mensaje));
    Serial.println("");
}
