#ifndef DEBUG_H
#define DEBUG_H

#define debug 1

#if debug == 1
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

#else
#define debugPrint(x)
#define debugPrintln(x)
#define debugSerialBegin(x)
#define debugTrue(x, y)

#endif

#endif