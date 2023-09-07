#ifndef DEBUG_H
#define DEBUG_H

#define debugGeneral 0
#define debugSleep 1

#if debugGeneral == 1
// Remplaza al tipico Serial.Print() para mostrar por consola.
#define debugPrint(x) Serial.print(x)
// Remplaza al tipico Serial.Println() para mostrar por consola.
#define debugPrintln(x) Serial.println(x)
// Definimos el Serial print.
#define debugSerialBegin(x) Serial.begin(x);
// Remplaza al tipico Serialprint dentro de un if. Remmplazar condicion por la condicion que da TRUE o FALSE y mensaje con el mensaje a mostrar.
#define debugTrue(condicion, mensaje) \
    if (condicion)                    \
    {                                 \
        Serial.println(mensaje);      \
    }
#define debugWifiManager() wifiManager.setDebugOutput(true);
#else
#define debugPrint(x)
#define debugPrintln(x)
#define debugTrue(x, y)
#define debugWifiManager() wifiManager.setDebugOutput(false);

#endif

#if debugSleep == 1
// Remplaza al tipico Serial.Print() para mostrar por consola.
#define debugSleepPrint(x) Serial.print(x)
// Remplaza al tipico Serial.Println() para mostrar por consola.
#define debugSleepPrintln(x) Serial.println(x)
// Definimos el Serial print.
#define debugSerialBegin(x) Serial.begin(x);
// Remplaza al tipico Serialprint dentro de un if. Remmplazar condicion por la condicion que da TRUE o FALSE y mensaje con el mensaje a mostrar.
#define debugSleepTrue(condicion, mensaje) \
    if (condicion)                         \
    {                                      \
        Serial.println(mensaje);           \
    }

#else
#define debugSleepPrint(x)
#define debugSleepPrintln(x)
#define debugSleepTrue(x, y)
#endif

#if debugSleep == 1 || debugGeneral == 1
#define debugSerialBegin(x) Serial.begin(x);
#else
#define debugSerialBegin(x)
#endif

#endif