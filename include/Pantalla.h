#ifndef PANTALLA_H
#define PANTALLA_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

#include "Definiciones/Iconos/TempHum.h"
#include "Definiciones/AnimationFrames/sol.h"
#include "Definiciones/AnimationFrames/dolarUp.h"
#include "Definiciones/AnimationFrames/dolarDown.h"
#include "Definiciones/AnimationFrames/humedad.h"
#include "Definiciones/AnimationFrames/dolarEstable.h"
#include "Definiciones/AnimationFrames/nube.h"
#include "Definiciones/AnimationFrames/lluvia.h"
#include "Definiciones/AnimationFrames/casa.h"
#include "Definiciones/gpio.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Espera en ms entre request de Dolar a api.
const int intervaloRequestDolar = 1800000; // 30min
// Espera en ms entre request de Clima a api.
const int intervaloRequestClima = 600000; // 1800000; // 30min
// Intervalo para actualizar las mediciones del DTH11.
const unsigned long intervalCheckEntorno = 50000;
// Cantidad de animaciones.
const int cantAnimaciones = 7;
/*
// Cantidad de animaciones.
const int cantAnimaciones = 7;
// Coordenada X posicion Menus.
const int xxMenu = 0;
// Coordenada Y posicion Menus.
const int yyMenu = 0;
// Ancho indicador menu.
const int anchoIconMenu = 16;
// Ancho indicador menu.
const int altoIconMenu = 16;
// Ancho barras menus y macros
const int anchoBarra = 4; */

class Pantalla
{
public:
    // Constructor...
    Pantalla();
    // Usado para iniciar la clase.
    void iniciar();
    // Limpia la pantalla.
    void limpiarPantalla();
    // Usada para mostrar el texto dado como argumento, junto a coordenadas y tamano.
    void escribir(int xx, int yy, String texto, int tamano);
    // Usada para mostrar el menu.
    void menu(int animacion, int macro);

    // Controlador HTTP Clima, no jodas, no voy a armar otra clase.
    void controladorClima();
    // Ya se, tendria que ser una clase controlar, pero esto de mostrar cosas en el menu me enloquecio y lo deje aca por aragan.
    String controladorDolar();

private:
    Adafruit_SSD1306 display;
    DHT dht;

    // Dibuja pantalla Clima.
    void dibujarMenuClima();

    // Dibujar pantalla Dolar.
    void dibujarMenuDolar();

    // Dibuja pantalla Entorno.
    void dibujarMenuEntorno();

    // Dibuja indicadores de opcion de menu seleccionada. NO LIMPIA NI MUESTRA. VIEJO!
    // void dibujarIndicadorPosicion();

    // Dibujar barras indicadoras de seleccion para Menu y Macro. NO LIMPIA NI MUESTRA.
    void dibujarIndicadorPosicion_v2(bool izquierda, bool menu);

    // Usada para escribir en las animaciones ya que no borra al inicar.
    void escribirEnAnimacion(int xx, int yy, String texto, int tamano);

    // Numero maximo de framas para las animaciones.
    static const int maxFrames = 60;

    // Conjunto de animaciones disponibles - Respetar mismo orden que en numeroDeFrames[]. | DolarUp | Dolar Down | Dolar Estable | Sol | LLuvia | Nube | Casa |
    const unsigned char *animaciones[cantAnimaciones][maxFrames] = {
        {dolarup0, dolarup1, dolarup2, dolarup3, dolarup4, dolarup5, dolarup6, dolarup7, dolarup8, dolarup9, dolarup10, dolarup11, dolarup12, dolarup13, dolarup14, dolarup15, dolarup16, dolarup17, dolarup18, dolarup19, dolarup20, dolarup21, dolarup22, dolarup23, dolarup24, dolarup25, dolarup26, dolarup27, dolarup28, dolarup29, dolarup30, dolarup31, dolarup32},
        {dolardown0, dolardown1, dolardown2, dolardown3, dolardown4, dolardown5, dolardown6, dolardown7, dolardown8, dolardown9, dolardown10, dolardown11, dolardown12, dolardown13, dolardown14, dolardown15, dolardown16, dolardown17, dolardown18, dolardown19, dolardown20, dolardown21, dolardown22, dolardown23, dolardown24, dolardown25, dolardown26, dolardown27, dolardown28, dolardown29, dolardown30, dolardown31, dolardown32},
        {dolarEstable_001, dolarEstable_002, dolarEstable_003, dolarEstable_004, dolarEstable_005, dolarEstable_006, dolarEstable_007, dolarEstable_008, dolarEstable_009, dolarEstable_010, dolarEstable_011, dolarEstable_012, dolarEstable_013, dolarEstable_014, dolarEstable_015, dolarEstable_016, dolarEstable_017, dolarEstable_018, dolarEstable_019, dolarEstable_020, dolarEstable_021, dolarEstable_022, dolarEstable_023, dolarEstable_024, dolarEstable_025, dolarEstable_026, dolarEstable_027, dolarEstable_028, dolarEstable_029, dolarEstable_030, dolarEstable_031, dolarEstable_032, dolarEstable_033, dolarEstable_034, dolarEstable_035, dolarEstable_036, dolarEstable_037, dolarEstable_038, dolarEstable_039, dolarEstable_040, dolarEstable_041, dolarEstable_042, dolarEstable_043, dolarEstable_044, dolarEstable_045, dolarEstable_046, dolarEstable_047, dolarEstable_048, dolarEstable_049, dolarEstable_050, dolarEstable_051, dolarEstable_052, dolarEstable_053, dolarEstable_054, dolarEstable_055, dolarEstable_056, dolarEstable_057},
        {sol_001, sol_002, sol_003, sol_004, sol_005, sol_006, sol_007, sol_008, sol_009, sol_010, sol_011, sol_012, sol_013, sol_014, sol_015, sol_016, sol_017, sol_018, sol_019, sol_020, sol_021, sol_022, sol_023, sol_024, sol_025, sol_026, sol_027, sol_028},
        {lluvia_001, lluvia_002, lluvia_003, lluvia_004, lluvia_005, lluvia_006, lluvia_007, lluvia_008, lluvia_009, lluvia_010, lluvia_011, lluvia_012, lluvia_013, lluvia_014, lluvia_015, lluvia_016, lluvia_017, lluvia_018, lluvia_019, lluvia_020, lluvia_021, lluvia_022, lluvia_023, lluvia_024, lluvia_025, lluvia_026, lluvia_027, lluvia_028, lluvia_029, lluvia_030, lluvia_031, lluvia_032, lluvia_033, lluvia_034, lluvia_035, lluvia_036, lluvia_037, lluvia_038, lluvia_039, lluvia_040, lluvia_041, lluvia_042, lluvia_043, lluvia_044, lluvia_045, lluvia_046, lluvia_047, lluvia_048, lluvia_049, lluvia_050},
        {nublado_001, nublado_002, nublado_003, nublado_004, nublado_005, nublado_006, nublado_007, nublado_008, nublado_009, nublado_010, nublado_011, nublado_012, nublado_013, nublado_014, nublado_015, nublado_016, nublado_017, nublado_018, nublado_019, nublado_020, nublado_021, nublado_022, nublado_023, nublado_024, nublado_025, nublado_026, nublado_027, nublado_028, nublado_029, nublado_030, nublado_031, nublado_032, nublado_033, nublado_034, nublado_035, nublado_036, nublado_037, nublado_038, nublado_039, nublado_040, nublado_041, nublado_042, nublado_043, nublado_044, nublado_045},
        {casa_001, casa_002, casa_003, casa_004, casa_005, casa_006, casa_007, casa_008, casa_009, casa_010, casa_011, casa_012, casa_013, casa_014, casa_015, casa_016, casa_017, casa_018, casa_019, casa_020, casa_021, casa_022, casa_023, casa_024, casa_025, casa_026, casa_027, casa_028, casa_029, casa_030, casa_031, casa_032, casa_033, casa_034, casa_035, casa_036, casa_037, casa_038, casa_039, casa_040}};

    // Conjunto de cantidad de frames de animaciones en animaciones[] - Respetar el mismo orden que en animaciones[]. | DolarUp | Dolar Down | Dolar Estable | Sol | Lluvia | Nube | Casa |
    const unsigned int numeroDeFrames[cantAnimaciones] = {cantFramesDolarUp, cantFramesDolarDown, cantFramesDolarEstable, cantFramesSol, cantFramesLluvia, canFramesNublado, cantFramesCasa};

    // Iconos Temperatura y humedad. | Humedad 1 | Termometro 2 |
    const unsigned char *iconosHumTemp[cantIconosHumTemp] = {Humedad_1, Termometro_2};

    // Conjunto de numeros para los indicadores de Menu y Macro.
    // const unsigned char *nums[cantMenus] = {num1, num2, num3};

    // Variable usada en el controlador de dolar para manejar timepo.
    int dolarPedidoEn = 0;
    // Ultimo mensajes, mensaje para mostrar si no se solicita nuevamente.
    String ultimoDolar;
    // Ultimo valor obtenido del dolar.
    float ultimoValor = 0;
    // Utilizada para saber si el Dolar esta subiendo (1), bajando(0) o quedo igual(2) en funcioncion del valor anterior y actual.
    int dolarSubiendo = 2;
    // Usado para saber el nuemero de request realiazadas.
    int numeroDeRequest = 0;
    // Busca en https://bluelytics.com.ar el valor del dolar y devuelve un Float con el valor | 000 => Error Json | 001 => Error HTTP |
    float obtenerDolarBlue();

    // Id del clima actual obtenido.
    int idClima;
    // Temperatura actual obtenido.
    int tempClima;
    // Humedad actual obtenido.
    int humedadClima;
    // Registro de timepo para la consulta HTTP.
    int climaPedidoEn = 0;
    // Busca en urlClima el clima y setea las variables de clase relacionadas | 000 => Error Json | 001 => Error HTTP |
    int obtenerClima();

    // Almacena el tiempo del último ciclo.
    unsigned long previousMillis = 0;
    // Medimos periodicamente.
    void medirLocal();
};

#endif