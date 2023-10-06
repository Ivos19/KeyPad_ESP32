#ifndef MIMIR_H
#define MIMIR_H

#include "ControladorClientes.h"
#include "ControladorBotones.h"
#include "ControladorUDP.h"
#include "Pantalla.h"

#include <Preferences.h>

class Mimir
{
public:
    Mimir();
    void IniciarMimir(ControladorClientes &cCLTs, ControladorBotones &cb, ControladorUDP &cu);
    void A_Mimir(ControladorClientes &cCLTs, long tiempoDeUltimaAccion, Pantalla &pantalla);
    // bool GetRestoreData();

private:
    Preferences preferences; // Objeto para gestionar la memoria NVS
    // bool restoreData = false; // Bandera para indicar si se deben restaurar los datos

    void GuardarClientes(ControladorClientes &cCLTs);
    void CargarClientes(ControladorClientes &cCLTs);
    void Print_GPIO_wake_up(ControladorClientes &cCLTs, ControladorBotones &cb, ControladorUDP &cu);
    bool PasoElTiempo();
    void SonarAlDormir();
    void SonarAlDespertar();
    void GuardarPosiciones();
    void CargarPosiciones();
};

#endif