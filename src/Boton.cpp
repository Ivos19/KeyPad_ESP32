#include "Boton.h"

#include "Definiciones/debug.h"

void Boton::iniciarBoton()
{
    pinMode(_pin, INPUT_PULLDOWN);
}

void Boton::setPin(int pin) { _pin = pin; }

void Boton::setMensaje(const char *mensaje) { _mensaje = mensaje; }

bool Boton::getEstado() { return _presionado; }

int Boton::getTiempoIni() { return _tiempoIni; }

int Boton::getUltimaDuracion() { return _ultimaDuracionDePresion; }

int Boton::getPin() { return _pin; }

String Boton::getMensaje() { return _mensaje; }

bool Boton::presionando()
{
    if (digitalRead(_pin) == presionado)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Boton::checkEstado()
{
    bool ahoraPresionado = presionando();
    bool salida = false;

    if (!_presionado && ahoraPresionado) // Inicio de presión
    {
        _presionado = true;
        _tiempoIni = millis();
    }
    else if (_presionado && !ahoraPresionado && (millis() - _tiempoIni) > 15) // Soltó el botón
    {
        _presionado = false;
        _ultimaDuracionDePresion = millis() - _tiempoIni;
        salida = true;
    }

    return salida;
}
