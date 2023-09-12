#ifndef CONSTANTES_H
#define CONSTANTES_H

// Relacionados a UDP
const unsigned int localUdpPort = 4210;           // local port to listen on
const unsigned int puertodeRespuesta = 4211;      // puerto al que enviar desde ESP a CLientes
const char replyPacket[] = "Respuesta desde ESP"; // Mensaje de respuesta
const int maxClientes = 5;                        // Numero maximo de clientes

// Relacionados a Buzzer en Encoder
const int buzzerChannel = 0;     // Canal LEDC para el buzzer
const int buzzerFreq = 1000;     // Frecuencia en Hz
const int buzzerResolution = 15; // Era Resolución (8 bits)

const int tonoBajo = 500;     // Frecuencia tono bajo
const int tonoAlto = 1000;    // Frecuencia tono alto
const int tonoConecto = 2000; // Frecuencia tono alto

// Relacionadas a Pantalla
// Coordenada X posicion Menus.
#define xxMenu 0
// Coordenada Y posicion Menus.
#define yyMenu 0
// Ancho indicador menu.
#define anchoIconMenu 16
// Ancho indicador menu.
#define altoIconMenu 16
// Ancho barras menus y macros
#define anchoBarra 3

// Tiempo que debe pasar sin presionar ningun boton para dormir el dispositivo
#define tiempoEspera 10000

#endif
