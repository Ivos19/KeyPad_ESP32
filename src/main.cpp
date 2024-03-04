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
#include "Mimir.h"

#include "Definiciones/ip.h"
#include "Definiciones/constantes.h"
#include "Definiciones/variablesGlobales.h"
#include "Definiciones/debug.h"
#include "Definiciones/pitidos.h"
#include <otaSetup.h>

TaskHandle_t _TaskHandleCore1;
ControladorUDP _cUDP;
ControladorClientes _cCLTs;
Pantalla _pantalla;
Encoder _encoder;
ControladorBotones _cBTs;
Mimir _cMimir;

void core1Task(void *parameter);

void _print_GPIO_wake_up();
void _aMimir();
bool _pasoElTiempo();
void _cargarClientes();
void _guardarClientes();

bool iniciadoTask = false;
bool iniciadoMain = false;
bool despertado = esp_sleep_get_wakeup_cause();

void setup()
{
  debugSerialBegin(115200);
  debugPrintln();
  debugPrintln("| Iniciado ESP en: |");

  despertado = esp_sleep_get_wakeup_cause();
  debugSleepTrue(!despertado, "\033[36m | Razon: " + String(despertado) + " | Inicio desde 0 |\033[0m");

#pragma region Setear Wifi y OTA

  WiFi.mode(WIFI_STA); // Iniciamos el AP. Si ya hay contraseña guardada y se pudo conectar, no inciar el "no usamos" WifiManager.

  // Inicializar WiFiManager
  WiFiManager wifiManager;

  wifiManager.setSTAStaticIPConfig(ip, gateway, subnet, dns); // Establesco el IP estatico. // Puede generar problemas con OTA, en realidad es tema del IP fijo.
  // Wifi Manager settings...
  //  wifiManager.resetSettings(); // Descomentar la línea siguiente para realizar un reinicio por defecto
  //  Agregar parámetros personalizados (opcional)
  //  WiFiManagerParameter _cUDPstom_parameter("parameter_id", "Parameter Label", "default_value", length);
  //  wifiManager.addParameter(&_cUDPstom_parameter);
  debugWifiManager();

  // Conectar o configurar WiFi
  wifiManager.autoConnect("KeyPad Ivo!");

  // Configurar OTA
  setup_OTA();

#pragma endregion

#pragma region Inicializar clases
  // Resto del Setup

  _cUDP.iniciar(); // Iniciamos UDP.

  _pantalla.Iniciar(); // Iniciar Pantalla.  // Revisa de sacar la primer consultas a apis de aca y asi, con alguna variable extra, ejecutar una vez sonar() al mismo timepo. (Mostrar en pantalla INICIO junto con la musica).

  _cBTs.IniciarControlador(); // Iniciamos controlador de botones.

  _encoder.Iniciar(); // Iniciar el Encoder.

  _cMimir.IniciarMimir(_cCLTs, _cBTs, _cUDP); // Iniciamos el controlador de DeepSleep.

#pragma endregion

#pragma region Setear el segundo nucleo
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
#pragma endregion
}

void loop() // Trabajo en nueclo 0
{

  if (!iniciadoMain && despertado == 0) //!_cMimir.GetRestoreData() && despertado == 0)
  {
    sonar();
    iniciadoMain = true;
  }

  ArduinoOTA.handle();

  _cUDP.buscarCliente(_cCLTs);

  _cCLTs.MostrarListaSiNuevo();

  _encoder.EncoderLoop();

  _cBTs.BuscarPresionados_V2(_cCLTs, _cUDP);
  //_cBTs.BuscarPresionadosV2();

  _cBTs.BuscarGiroEncoder(_cCLTs, _cUDP);

  _cMimir.A_Mimir(_cCLTs, _cBTs.TiempoUltimaAccion(), _pantalla);
}

// Función que se eje_cUDPta en el núcleo 1
void core1Task(void *parameter)
{
  while (true)
  {
    if (!iniciadoTask && despertado != 0)
    {
      if (despertado != 0)
      {
        _pantalla.LimpiarPantalla();
        _pantalla.Escribir(0, 5, "Arriba!", 2);
      }
      else
      {
        _pantalla.Escribir(0, 2, "Inicio!", 3);
      }

      iniciadoTask = true;

      _pantalla.ControladorClima();

      _pantalla.ControladorDolar();
    }

    _pantalla.Menu(0, 0); // En esta prueba los argumentos estan al pedo.
  }
}
