#ifndef BOTON_H
#define BOTON_H

#include <Arduino.h>
#include "Definiciones/gpio.h"

class Boton
{
public:
    void iniciarBoton();

    void setPin(int pin);
    void setMensaje(const char *mensaje);

    int getTiempoIni();
    bool getEstado();
    int getPin();
    // Devuelve el tiempo que estuvo presionado por ultima vez el boton. Conveniente llamarlo luego de un check.
    int getUltimaDuracion();
    // Devuelve el mensaje asignado al boton.
    String getMensaje();
    // Devuelve si el boton esta siendo rpesionado.
    bool presionando();
    // Revisa si se apreta el boton, cuando se suelta se devuelve True. Mientras el boton esta apretdo, devuelve false. Requiere pasar mas de una vez para devolver true, si pasa solo ua vez el boton queda en su ultimo estado.
    bool checkEstado();

private:
    bool _presionado = false;
    int _tiempoIni = 0;
    int _ultimaDuracionDePresion = 0;
    int _pin;
    String _mensaje;
};

#endif