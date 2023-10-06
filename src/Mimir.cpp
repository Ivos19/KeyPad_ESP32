#include <Arduino.h>
#include <esp_sleep.h>

#include <ArduinoJson.h>
#include <Preferences.h>

#include "Mimir.h"
#include "Definiciones/debug.h"
#include "Definiciones/gpio.h"
#include "Definiciones/constantes.h"
#include "driver/rtc_io.h"
#include "Pantalla.h"

extern int macroPos;
extern int menuPos;
extern bool interrumpirAnimacion;

Mimir::Mimir()
{
    Preferences preferences;
}

/* bool Mimir::GetRestoreData()
{
    return restoreData;
} */

void Mimir::IniciarMimir(ControladorClientes &cc, ControladorBotones &cb, ControladorUDP &cu)
{
    preferences.begin("mis_variables", false); // Inicializa la memoria NVS

    // restoreData = preferences.getBool("restoreData", false);
    if (esp_sleep_get_wakeup_cause() != 0) // restoreData && esp_sleep_get_wakeup_cause() != 0)
    {
        // Traemos los clientes que habia en NVS
        CargarClientes(cc);
        // Cargar Posiciones
        CargarPosiciones();
        // Mostramos el GPIO que desperto al ESP
        Print_GPIO_wake_up(cc, cb, cu);

        interrumpirAnimacion = false;
    }

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
    rtc_gpio_pulldown_en(GPIO_NUM_4);

    esp_sleep_enable_ext1_wakeup(GPIO_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
}

// Mostrar motivo de despertar
void Mimir::Print_GPIO_wake_up(ControladorClientes &cc, ControladorBotones &cb, ControladorUDP &cu)
{
    esp_sleep_wakeup_cause_t causa = esp_sleep_get_wakeup_cause();
    uint64_t GPIO_reason = esp_sleep_get_ext1_wakeup_status();

    debugSleepPrint("| Razon: ");
    debugSleepPrint(causa);
    debugSleepPrint(" | Se desperto por GPIO ");
    int gpio = (log(GPIO_reason)) / log(2);
    debugSleepPrint(gpio);
    debugSleepPrintln(" |");

    cu.responder_a_Clientes(cc, ("G" + String(macroPos) + cb.GetMensajeBotonBoton(gpio)).c_str());

    SonarAlDespertar();
}

void Mimir::CargarPosiciones()
{
    macroPos = preferences.getInt("macroPos", 1);
    menuPos = preferences.getInt("menuPos", 1);
    debugSleepPrintln("| MenuPos: " + String(menuPos) + " | MacroPos: " + String(macroPos) + " |");
}

void Mimir::SonarAlDespertar()
{
    for (int i = 1; i <= 3; i++)
    {
        tone(BUZZER_PIN, 2000 + 1000 * i); // Aumenta la frecuencia con cada iteración
        delay(70);
    }
    noTone(BUZZER_PIN);
}

// Todo sobre dormir el ESP
void Mimir::A_Mimir(ControladorClientes &cCLTs, long tiempoDeUltimaAccion, Pantalla &p)
{
    if ((millis() - tiempoDeUltimaAccion) > tiempoEspera)
    {
        SonarAlDormir();

        GuardarClientes(cCLTs);

        GuardarPosiciones();

        interrumpirAnimacion = true;
        menuPos = 4;
        delay(100);

        esp_deep_sleep_start();
    }
}

void Mimir::GuardarPosiciones()
{
    int macro = preferences.getInt("macroPos", 1);
    int menu = preferences.getInt("menuPos", 1);

    if (macroPos != macro)
    {
        preferences.putInt("macroPos", macroPos);
        debugSleepPrint("| Actualizada memoria MacroPos: " + String(macroPos) + " |");
    }
    if (menuPos != menu)
    {
        preferences.putInt("menuPos", menuPos);
        debugSleepPrint("| Actualizada memoria MenuPos: " + String(menuPos) + " |");
    }
    debugSleepPrintln("");
}

void Mimir::SonarAlDormir()
{
    for (size_t i = 1; i < 4; i++)
    {
        tone(BUZZER_PIN, 8000 / i);
        delay(70);
    }
    noTone(BUZZER_PIN);
}

bool Mimir::PasoElTiempo()
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

void Mimir::CargarClientes(ControladorClientes &cCLTs)
{
    debugSleepPrintln(" <-----> ARRIBA! ARRIBA! <----->");
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

                cCLTs.CrearCliente(ipCLT, portCLT);
            }
        }
    }
    // preferences.putBool("restoreData", false);
    debugSleepPrintln("");
}

void Mimir::GuardarClientes(ControladorClientes &cCLTs)
{
    debugSleepPrintln(" <-----> A MIMIR! <----->");
    // Serializar los datos de clientes para meterlos en la NVS
    StaticJsonDocument<1000> doc;
    for (int i = 0; i < maxClientes; i++)
    {
        JsonObject cliente = doc.createNestedObject();
        cliente["ip"] = cCLTs.clientes[i].getIp();
        cliente["port"] = cCLTs.clientes[i].getPort();
    }

    // Guardar los datos serializados en la NVS
    String serializedData;
    serializeJson(doc, serializedData);
    debugSleepPrint(" ---> Cliente en RAM: ");
    debugSleepPrint(serializedData);
    debugSleepPrintln(" ---");

    // Revisamos que tenemos en memoria para no guardar si es lo mismo
    String dataClientesEnMemoria = preferences.getString("clientes", "");

    if (dataClientesEnMemoria != serializedData)
    {
        preferences.putString("clientes", serializedData);
    }

    // preferences.putBool("restoreData", true);
    debugSleepPrintln("");
}
