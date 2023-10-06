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

void ControladorBotones::IniciarControlador()
{
    for (size_t i = 0; i < cantBotones; i++)
    {
        botones[i].iniciarBoton();
        botones[i].setMensaje(_mensajes[i]);
    }
}

long ControladorBotones::TiempoUltimaAccion()
{
    return _tiempoUltimaAccion;
}

bool ControladorBotones::BotonPresionadoYSoltado(int i)
{
    bool ret = botones[i].checkEstado();
    return ret;
}

String ControladorBotones::GetMensajeBotonBoton(int gpio)
{
    String mensaje;
    for (size_t i = 0; i < cantBotones; i++)
    {
        if (botones[i].getPin() == gpio)
        {
            mensaje = botones[i].getMensaje();
            i = cantBotones;
        }
    }
    return mensaje;
}

/* void ControladorBotones::BuscarPresionados(ControladorClientes &cc, ControladorUDP &cu)
{
    for (size_t i = 0; i < cantBotones; i++)
    {
        bool estado = botones[i].checkEstado();
        debugBotonTrue(estado, ("Se apreto el boton: " + String(botones[i].getPin()) + " por " + String(botones[i].getUltimaDuracion()) + " ms"));
        if (i == posicionBotonEncoder && estado && botones[i].getUltimaDuracion() < 200) // Evitamos enviar mensaje cuando mantenemos el encoder presionado por mas de 200ms. Evitando enviar mensaje cuando en realidad navegamos por el menu.
        {
            cu.responder_a_Clientes(cc, ("G" + String(macroPos) + botones[i].getMensaje()).c_str());
            SetTiempoUltimaAccion();
        }
        else if (estado && i != posicionBotonEncoder)
        {
            cu.responder_a_Clientes(cc, ("G" + String(macroPos) + botones[i].getMensaje()).c_str());
            SetTiempoUltimaAccion();
        }
    }
} */

void ControladorBotones::BuscarPresionados_V2(ControladorClientes &cc, ControladorUDP &cu)
{
    for (Boton boton : botones) // Iteramos sobre la lista de botones.
    {
        bool estaSiendoApretado = boton.presionando();                                  // Revisamos si el boton actual esta siendo presionado.
        String mensajeDelBotonActual = boton.getMensaje();                              // Obtenemos el mensaje del boton actual para no buscarlo de nuevo.
        bool estaEnLaLista = EstaEnLaLista(_botonesPresionados, mensajeDelBotonActual); // Revisamos si ya esta en la lista de botones presionados. Esta lista se actualiza cada vez que se pasa por aca.

        if (estaSiendoApretado && !estaEnLaLista) // Si el Boton esta siendo presionado y no esta en la lista...
        {
            _botonesPresionados.push_back(mensajeDelBotonActual); // Lo agregamos al final de la lista.

            if (_botonesPresionados.size() == 1) // Revisamos si solo se apreto un boton.
            {
                esCombinacion = false;
            }
            else if (_botonesPresionados.size() > 1) // Si se estan presioanndo mas de uno, establecemos que se estab presioando una combinacion de botones.
            {
                esCombinacion = true;
            }
        }
        else if (!estaSiendoApretado && estaEnLaLista) // Si el boton esta en la lista y ahora no se esta presioando mas...
        {

            // Creamos el mensaje a enviar con la combinacion de mensajes de los botones que estaban siendo presionados
            String mensaje_a_enviar;
            for (String mensaje : _botonesPresionados)
            {
                mensaje_a_enviar = mensaje_a_enviar + mensaje;
            }

            if (!EsElPrimerBotonDeLaCombinacion(_botonesPresionados, mensajeDelBotonActual)) // Si no es el primer boton de la lista mandamos el mensaje.
            {
                cu.responder_a_Clientes(cc, ("G" + String(macroPos) + mensaje_a_enviar).c_str());
            }

            if (EsElPrimerBotonDeLaCombinacion(_botonesPresionados, mensajeDelBotonActual) && !esCombinacion) // Si es el primer Boton de la lista pero no es una combinacion de teclas mandamos el mensaje.
            {
                cu.responder_a_Clientes(cc, ("G" + String(macroPos) + mensaje_a_enviar).c_str());
                esCombinacion = false;
            }

            // Actualizamos el momento de la ultima activacion de un boton
            SetTiempoUltimaAccion();
            // Sacamos el boton que se dejo de apretar de la lista
            _botonesPresionados.remove(mensajeDelBotonActual);
        }
    }
}

bool ControladorBotones::EsElPrimerBotonDeLaCombinacion(std::list<String> lista, String elemento)
{
    if (lista.front() == elemento)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ControladorBotones::EstaEnLaLista(std::list<String> lista, String elemento)
{
    auto iterador = std::find(lista.begin(), lista.end(), elemento);
    if (iterador != lista.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ControladorBotones::SetTiempoUltimaAccion()
{
    _tiempoUltimaAccion = millis();
}

void ControladorBotones::BuscarGiroEncoder(ControladorClientes &cc, ControladorUDP &cu)
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