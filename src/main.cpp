#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "ControladorUDP.h"
#include "ControladorClientes.h"
#include "ControladorBotones.h"
#include "Pantalla.h"
#include "Encoder.h"

#include "Definiciones/ip.h"
#include "Definiciones/constantes.h"
#include "Definiciones/variablesGlobales.h"
#include "Definiciones/debug.h"
#include "Definiciones/pitidos.h"
#include <otaSetup.h>

ControladorUDP _cUDP;
ControladorClientes _cCLS;
Pantalla _pantalla;
Encoder _encoder;
ControladorBotones _cBTs;

TaskHandle_t _TaskHandleCore1;
void core1Task(void *parameter);

bool iniciado = false;

void setup()
{
  debugSerialBegin(115200);
  debugPrintln();
  debugPrintln("| Iniciado ESP en: |");

  WiFi.mode(WIFI_STA); // Iniciamos el AP. Si ya hay contraseña guardada y se pudo conectar, no inciar el "no usamos" WifiManager.

  // Inicializar WiFiManager
  WiFiManager wifiManager;

  wifiManager.setSTAStaticIPConfig(ip, gateway, subnet, dns); // Establesco el IP estatico. // Puede generar problemas con OTA, en realidad es tema del IP fijo.
  // Wifi Manager settings...
  //  wifiManager.resetSettings(); // Descomentar la línea siguiente para realizar un reinicio por defecto
  //  Agregar parámetros personalizados (opcional)
  //  WiFiManagerParameter _cUDPstom_parameter("parameter_id", "Parameter Label", "default_value", length);
  //  wifiManager.addParameter(&_cUDPstom_parameter);

  // Conectar o configurar WiFi
  wifiManager.autoConnect("KeyPad Ivo!");

  debugPrintln("| En conectar: " + String(millis()) + " |");

  debugPrintln("| Conectado a WiFi! |");

  // Configurar OTA
  setup_OTA();

  // Resto del Setup

  _cUDP.iniciar(); // Iniciamos UDP.

  _pantalla.iniciar(); // Iniciar Pantalla.  // Revisa de sacar la primer consultas a apis de aca y asi, con alguna variable extra, ejecutar una vez sonar() al mismo timepo. (Mostrar en pantalla INICIO junto con la musica)

  _cBTs.iniciarControlador(); // Iniciamos controlador de botones.

  _encoder.iniciar(); // Iniciar el Encoder.

  // Crear una tarea en el núcleo 1
  xTaskCreatePinnedToCore(
      core1Task,         // Función de la tarea
      "Core 1 Task",     // Nombre de la tarea
      10000,             // Tamaño de la pila
      NULL,              // Parámetros de la tarea
      1,                 // Prioridad de la tarea
      &_TaskHandleCore1, // Manejador de la tarea
      1                  // Núcleo destino (núcleo 1)
  );
}

void loop() // Trabajo en nueclo 0
{

  if (!iniciado)
  {
    sonar();
  }

  ArduinoOTA.handle();

  _cUDP.buscarCliente(_cCLS);

  _cCLS.mostrarListaSiNuevo();

  _encoder.encoderLoop();

  _cBTs.buscarPresionados(_cCLS, _cUDP);

  _cBTs.buscarGiroEncoder(_cCLS, _cUDP);
}

// Función que se eje_cUDPta en el núcleo 1
void core1Task(void *parameter)
{
  while (true)
  {
    if (!iniciado)
    {
      _pantalla.escribir(0, 2, "Inicio!", 3);

      iniciado = true;

      _pantalla.controladorClima();

      _pantalla.controladorDolar();
    }

    _pantalla.menu(0, 0); // En esta prueba los argumentos estan al pedo.
  }
}
