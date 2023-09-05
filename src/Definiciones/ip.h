#include <WiFi.h>

#ifndef CREDENCIALES_H
#define CREDENCIALES_H

IPAddress ip(192, 168, 100, 90); // Aca esta le problema de OTA. Parece que con IP fija no quiere funcionar.
IPAddress gateway(192, 168, 100, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

#endif