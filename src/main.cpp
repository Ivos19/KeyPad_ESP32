#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_sleep.h>
#include "driver/rtc_io.h"
#include <Preferences.h>

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

#define GPIO_BITMASK (GPIO_SEL_12 | GPIO_SEL_13 | GPIO_SEL_14 | GPIO_SEL_25 | GPIO_SEL_26 | GPIO_SEL_27 | GPIO_SEL_32 | GPIO_SEL_33)

TaskHandle_t _TaskHandleCore1;
ControladorUDP _cUDP;
ControladorClientes _cCLTs;
Pantalla _pantalla;
Encoder _encoder;
ControladorBotones _cBTs;

Preferences preferences; // Objeto para gestionar la memoria NVS ///////////////////////////////////////////

void core1Task(void *parameter);

void _print_GPIO_wake_up();
void _aMimir();
bool _pasoElTiempo();
void _cargarClientes();
void _guardarClientes();

bool iniciado = false;

bool restoreData = false; // Bandera para indicar si se deben restaurar los datos

void setup()
{
  debugSerialBegin(115200);
  debugPrintln();
  debugPrintln("| Iniciado ESP en: |");

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

  _pantalla.iniciar(); // Iniciar Pantalla.  // Revisa de sacar la primer consultas a apis de aca y asi, con alguna variable extra, ejecutar una vez sonar() al mismo timepo. (Mostrar en pantalla INICIO junto con la musica)

  _cBTs.iniciarControlador(); // Iniciamos controlador de botones.

  _encoder.iniciar(); // Iniciar el Encoder.

#pragma endregion

#pragma region DeepSleep

  // DeepSleep
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
  rtc_gpio_pulldown_en(GPIO_NUM_12);
  rtc_gpio_pulldown_en(GPIO_NUM_13);
  rtc_gpio_pulldown_en(GPIO_NUM_14);
  rtc_gpio_pulldown_en(GPIO_NUM_25);
  rtc_gpio_pulldown_en(GPIO_NUM_26);
  rtc_gpio_pulldown_en(GPIO_NUM_27);
  rtc_gpio_pulldown_en(GPIO_NUM_32);
  rtc_gpio_pulldown_en(GPIO_NUM_33);

  esp_sleep_enable_ext1_wakeup(GPIO_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);

  preferences.begin("mis_variables", false); // Inicializa la memoria NVS

  restoreData = preferences.getBool("restoreData", false);
  if (restoreData)
  {
    // Traemos los clientes que habia en NVS
    _cargarClientes();
    // Mostramos el GPIO que desperto al ESP
    _print_GPIO_wake_up();
  }

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

  if (!iniciado && !restoreData)
  {
    sonar();
  }

  ArduinoOTA.handle();

  _cUDP.buscarCliente(_cCLTs);

  _cCLTs.mostrarListaSiNuevo();

  _encoder.encoderLoop();

  _cBTs.buscarPresionados(_cCLTs, _cUDP);

  _cBTs.buscarGiroEncoder(_cCLTs, _cUDP);

  _aMimir();
}

// Función que se eje_cUDPta en el núcleo 1
void core1Task(void *parameter)
{
  while (true)
  {
    if (!iniciado)
    {
      if (restoreData)
      {
        _pantalla.escribir(0, 2, "Despertando!", 2);
      }
      else
      {
        _pantalla.escribir(0, 2, "Inicio!", 3);
      }

      iniciado = true;

      _pantalla.controladorClima();

      _pantalla.controladorDolar();
    }

    _pantalla.menu(0, 0); // En esta prueba los argumentos estan al pedo.
  }
}

// Mostrar motivo de despertar
void _print_GPIO_wake_up()
{
  uint64_t GPIO_reason = esp_sleep_get_ext1_wakeup_status();
  Serial.print("GPIO that triggered the wake up: GPIO ");
  Serial.println((log(GPIO_reason)) / log(2), 0);
}

// Todo sobre dormir el ESP
void _aMimir()
{
  if (_pasoElTiempo())
  {
    tone(BUZZER_PIN, 2000, 0.9);
    delay(50);
    noTone(BUZZER_PIN);

    _guardarClientes();

    esp_deep_sleep_start();
  }
}

bool _pasoElTiempo()
{
  if (millis() > 10000)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void _cargarClientes()
{
  debugSleepPrintln(" <-----> ARRIBA! ARRIBA! <----->");
  debugSleepPrintln("-----------------------------------------------------------------------------");
  // Leer los datos de clientes desde la NVS
  String serializedData = preferences.getString("clientes", "");

  if (serializedData != "")
  {
    // Deserializar los datos
    StaticJsonDocument<1000> doc;
    DeserializationError error = deserializeJson(doc, serializedData);

    debugSleepPrint(" <--- Cliente en memoria NVS: ");
    debugSleepPrint(serializedData);
    debugSleepPrintln(" ---");

    if (!error)
    {
      // Recuperar los clientes deserializados
      for (int i = 0; i < maxClientes; i++)
      {
        IPAddress ipCLT;
        int portCLT;

        ipCLT.fromString(doc[i]["ip"].as<String>());
        portCLT = doc[i]["port"].as<int>();

        /* debugSleepPrint(" -| ");
        debugSleepPrint(ipCLT);
        debugSleepPrint(" | ");
        debugSleepPrint(portCLT);
        debugSleepPrintln(" |- "); */

        _cCLTs.CrearCliente(ipCLT, portCLT);
      }
    }
  }
  preferences.putBool("restoreData", false);
  debugSleepPrintln("");
}

void _guardarClientes()
{
  debugSleepPrintln(" <-----> A MIMIR! <----->");
  debugSleepPrintln("-----------------------------------------------------------------------------");
  // Serializar los datos de clientes para meterlos en la NVS
  StaticJsonDocument<1000> doc;
  for (int i = 0; i < maxClientes; i++)
  {
    JsonObject cliente = doc.createNestedObject();
    cliente["ip"] = _cCLTs.clientes[i].getIp();
    cliente["port"] = _cCLTs.clientes[i].getPort();
  }

  // Guardar los datos serializados en la NVS
  String serializedData;
  serializeJson(doc, serializedData);
  debugSleepPrint(" ---> Cliente en RAM: ");
  debugSleepPrint(serializedData);
  debugSleepPrintln(" ---");

  preferences.putString("clientes", serializedData);

  preferences.putBool("restoreData", true);
  debugSleepPrintln("");
}
