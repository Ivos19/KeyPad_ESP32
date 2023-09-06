#include "ControladorBotones.h"
#include "Definiciones/debug.h"
// #include "Definiciones/variablesGlobales.h"

extern int macroPos;
extern int giro;

ControladorBotones::ControladorBotones()
{
    for (size_t i = 0; i < cantBotones; i++)
    {
        botones[i].setPin(gpioBotones[i]);
    }
}

void ControladorBotones::iniciarControlador()
{
    for (size_t i = 0; i < cantBotones; i++)
    {
        botones[i].iniciarBoton();
        botones[i].setMensaje(_mensajes[i]);
    }
}

bool ControladorBotones::botonPresionadoYSoltado(int i)
{
    bool ret = botones[i].checkEstado();
    return ret;
}

void ControladorBotones::buscarPresionados(ControladorClientes &cc, ControladorUDP &cu)
{
    for (size_t i = 0; i < cantBotones; i++)
    {
        bool estado = botones[i].checkEstado();
        debugTrue(estado, ("Se apreto el boton: " + String(botones[i].getPin()) + " por " + String(botones[i].getUltimaDuracion()) + " ms"));
        if (i == posicionBotonEncoder && estado && botones[i].getUltimaDuracion() < 200) // Evitamos enviar mensaje cuando mantenemos el encoder presionado por mas de 200ms. Evitando enviar mensaje cuando en realidad navegamos por el menu.
        {
            cu.responder_a_Clientes(cc, ("G" + String(macroPos) + botones[i].getMensaje()).c_str());
        }
        else if (estado && i != posicionBotonEncoder)
        {
            cu.responder_a_Clientes(cc, ("G" + String(macroPos) + botones[i].getMensaje()).c_str());
        }
    }
}

void ControladorBotones::buscarGiroEncoder(ControladorClientes &cc, ControladorUDP &cu)
{

    if (!botones[8].presionando())
    {
        if (giro < giroAnterior) // Girar a derecha/antiHorario achica giro. [10]
        {
            debugPrintln("Giro anti horario");
            cu.responder_a_Clientes(cc, ("G" + String(macroPos) + _mensajes[10]).c_str());
            giroAnterior = giro;
        }
        if (giro > giroAnterior) // Girar a izquierda/horario agranda giro. [10]
        {
            debugPrintln("Giro horario");
            cu.responder_a_Clientes(cc, ("G" + String(macroPos) + _mensajes[9]).c_str());
            giroAnterior = giro;
        }
    }
}