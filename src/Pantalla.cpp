#include "Pantalla.h"
#include "Definiciones/debug.h"
#include "Definiciones/constantes.h"
#include "Definiciones/urls.h"
#include "Definiciones/Iconos/Dormir.h"

extern bool interrumpirAnimacion;
extern int menuPos;
extern int macroPos;
extern float tempLocal;
extern float humLocal;

Pantalla::Pantalla() : dht(DHT_PIN, DHT11)
{
    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
}

void Pantalla::Iniciar()
{
    // Inicializa la comunicación I2C
    Wire.begin(OLED_SDA, OLED_SCL);

    dht.begin(); // Inicialización del sensor DHT11

    // Inicializa la pantalla OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        debugPrintln(F("No se pudo inicializar la pantalla OLED"));
        while (true)
            ;
    }
}

void Pantalla::Escribir(int xx, int yy, String texto, int tamano)
{
    display.clearDisplay();
    display.setTextSize(tamano);
    display.setCursor(xx, yy);
    display.setTextColor(SSD1306_WHITE);
    display.print(texto);
    display.display();
    display.setCursor(0, 0);
}

void Pantalla::EscribirEnAnimacion(int xx, int yy, String texto, int tamano)
{
    display.setTextSize(tamano);
    display.setCursor(xx, yy);
    display.setTextColor(SSD1306_WHITE);
    display.print(texto);
}

void Pantalla::LimpiarPantalla()
{
    display.clearDisplay();
}

float Pantalla::ObtenerDolarBlue()
{
    float salida;

    HTTPClient http;
    http.begin(urlDolar);

    int httpCode = http.GET();
    if (httpCode > 0)
    {
        String payload = http.getString();

        // debugPrintln(payload);

        // Parsear el JSON
        StaticJsonDocument<2048> doc;
        DeserializationError error = deserializeJson(doc, payload);
        if (error)
        {
            debugPrintln("Error Json Dolas");
            salida = 000;
        }
        else
        {
            salida = doc["blue"]["value_sell"];
            // salida = "Dolar: " + String(dolarBlueVenta, 2);

            debugPrint("| Api dolar: ");
            debugPrint(salida);
            debugPrintln(" |");
        }
    }
    else
    {
        debugPrintln("Error HTTP  Dolas");
        salida = 001;
    }

    http.end();

    return salida;
}

int Pantalla::ObtenerClima()
{
    int salida;

    HTTPClient http;
    http.begin(urlClima);

    int httpCode = http.GET();

    if (httpCode > 0)
    {
        String payload = http.getString();
        // const char *payload = "{\"coord\":{\"lon\":-60.6393,\"lat\":-32.9468},\"weather\":[{\"id\":800,\"main\":\"Clear\",\"description\":\"clear sky\",\"icon\":\"01n\"}],\"base\":\"stations\",\"main\":{\"temp\":9.95,\"feels_like\":9.09,\"temp_min\":9.95,\"temp_max\":10.62,\"pressure\":1013,\"humidity\":81},\"visibility\":10000,\"wind\":{\"speed\":2.06,\"deg\":190},\"clouds\":{\"all\":0},\"dt\":1692863101,\"sys\":{\"type\":1,\"id\":8217,\"country\":\"AR\",\"sunrise\":1692873030,\"sunset\":1692913194},\"timezone\":-10800,\"id\":3838583,\"name\":\"Rosario\",\"cod\":200}";

        //debugPantallaPrintln(payload);

        // Parsear el JSON
        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, payload);
        if (error)
        {
            debugPantallaPrintln("Error Json Clima");
            salida = 00;
        }
        else
        {
            tempClima = doc["main"]["temp"];
            humedadClima = doc["main"]["humidity"];
            int idC = doc["weather"][0]["id"];

            if (idC == 800)
            {
                idClima = 8;
            }
            else if (idC > 800 && idC < 805)
            {
                idClima = 9;
            }
            else
            {
                idClima = idC / 100;
            }

            debugPantallaPrint("\033[34m| Id CLima: ");
            debugPantallaPrint(idClima);
            debugPantallaPrint(" | Temperatura: ");
            debugPantallaPrint(tempClima);
            debugPantallaPrint(" | Humedad: ");
            debugPantallaPrint(humedadClima);
            debugPantallaPrintln(" | \033[0m");
        }
    }
    else
    {
        debugPantallaPrintln("Error HTTP  Clima");
        salida = 01;
    }

    http.end();

    return salida;
}

void Pantalla::Menu(int a, int m)
{
    DibujarMenuClima();

    dibujarMenuEntorno();

    DibujarMenuDolar();

    DibujarSleep();
}

void Pantalla::DibujarSleep()
{

    if (menuPos == 4)
    {
        display.clearDisplay();

        display.drawBitmap(47, 0, icnoMimir[0], 32, 32, 1);
        display.display();
    }
}

void Pantalla::dibujarMenuEntorno()
{
    if (menuPos == 2)
    {
        int i = 0;
        const int xxOffSet = 0;
        int idAnimacion = 6;
        while (i < numeroDeFrames[idAnimacion] && menuPos == 2) // Entorno
        {
            MedirLocal();
            display.clearDisplay();

            display.drawBitmap(20 + xxOffSet, 0, animaciones[idAnimacion][i], 32, 32, 1);

            DibujarIndicadorPosicion_v2(true, true);
            DibujarIndicadorPosicion_v2(false, false);

            // Dibujamos Tempreatura. Icon - Valor
            display.drawBitmap(xxOffSet + 56, -1, iconosHumTemp[1], 16, 16, 1);
            EscribirEnAnimacion(xxOffSet + 78, 0, String(tempLocal, 0), 2);
            // Dibujamos Humedad. Icon - Valor
            display.drawBitmap(xxOffSet + 56, 16, iconosHumTemp[0], 16, 16, 1);
            EscribirEnAnimacion(xxOffSet + 78, 17, String(humLocal, 0), 2);

            display.display();
            i++;
            if (interrumpirAnimacion)
            {
                i = numeroDeFrames[idAnimacion];
                interrumpirAnimacion = false;
            }
        }
    }
}

void Pantalla::DibujarMenuDolar()
{
    if (menuPos == 3)
    {
        int i = 0;
        int frames = 1;
        const int xxOffSet = -10;
        while (i < frames && menuPos == 3) // Dolar
        {
            display.clearDisplay();

            DibujarIndicadorPosicion_v2(true, true);
            DibujarIndicadorPosicion_v2(false, false);

            if (dolarSubiendo == 2 || numeroDeRequest == 1)
            {
                display.drawBitmap(20 + xxOffSet, 0, animaciones[2][i], 32, 32, 1);
                frames = numeroDeFrames[2];
            }
            else if (dolarSubiendo == 0)
            {
                display.drawBitmap(20 + xxOffSet, 0, animaciones[1][i], 32, 32, 1);
                frames = numeroDeFrames[1];
            }
            else if (dolarSubiendo == 1)
            {
                display.drawBitmap(20 + xxOffSet, 0, animaciones[0][i], 32, 32, 1);
                frames = numeroDeFrames[0];
            }

            EscribirEnAnimacion(56 + xxOffSet, 9, ControladorDolar(), 2);

            display.display();

            i++;
            if (interrumpirAnimacion)
            {
                i = numeroDeFrames[2];
                interrumpirAnimacion = false;
            }
        }
    }
}

void Pantalla::DibujarMenuClima()
{
    if (menuPos == 1)
    {
        int i = 0;
        const int offsetIcons = 0;

        // Actualizamos datos desde API si son necesarios
        ControladorClima();

        // Revisamos que icono tenemos que poner en funcion del clima dado por la API.
        int id;
        if (idClima == 2 || idClima == 3 || idClima == 5 || idClima == 6) // 2.Tormenta; 3.Llovizna; 5.Lluvia; 6.Nieve
        {
            id = 4;
        }
        else if (idClima == 7 || idClima == 9) // 7.Algo en la atmosfera; 9.Nubes(este era 8 pero fue necesario diferenciarlo al pedirlo a la API)
        {
            id = 5;
        }
        else if (idClima == 8) // 8.Limpio
        {
            id = 3;
        }

        while (i < numeroDeFrames[id] && menuPos == 1) // Graficamos cuadro por cuadro.
        {
            display.clearDisplay();

            // dibujarIndicadorPosicion();
            DibujarIndicadorPosicion_v2(true, true);
            DibujarIndicadorPosicion_v2(false, false);

            // Dibujamos animacion correspondiente.
            display.drawBitmap(offsetIcons + 20, 0, animaciones[id][i], 32, 32, 1);

            String temperatura = String(tempClima);
            String humedad = String(humedadClima);

            // Dibujamos Tempreatura. Icon - Valor
            display.drawBitmap(offsetIcons + 56, -1, iconosHumTemp[1], 16, 16, 1);
            EscribirEnAnimacion(offsetIcons + 78, 0, temperatura, 2);
            // Dibujamos Humedad. Icon - Valor
            display.drawBitmap(offsetIcons + 56, 16, iconosHumTemp[0], 16, 16, 1);
            EscribirEnAnimacion(offsetIcons + 78, 17, humedad, 2);

            display.display();
            i++;

            if (interrumpirAnimacion)
            {
                i = numeroDeFrames[id];
                interrumpirAnimacion = false;
            }
        }
    }
}

void Pantalla::DibujarIndicadorPosicion_v2(bool izquierda, bool menu)
{
    int xR, xL, opt, pos1, pos2, pos3;
    if (izquierda)
    {
        xL = anchoBarra - 1;
        xR = 0;
        pos1 = 0;
        pos2 = 10;
        pos3 = 21;
    }
    else
    {
        xL = 127 - anchoBarra;
        xR = 127 - anchoBarra;
        pos1 = 21;
        pos2 = 10;
        pos3 = 0;
    }
    if (menu)
    {
        opt = menuPos;
    }
    else
    {
        opt = macroPos;
    }

    display.drawLine(xL, 0, xL, 32, 1);
    if (opt == 1)
    {
        display.fillRect(xR, pos1, anchoBarra, 11, 1);
    }
    else if (opt == 2)
    {
        display.fillRect(xR, pos2, anchoBarra, 11, 1);
    }
    else if (opt == 3)
    {
        display.fillRect(xR, pos3, anchoBarra, 11, 1);
    }
}

String Pantalla::ControladorDolar()
{
    String frase;
    float valor;
    bool mostrar = false;
    if (dolarPedidoEn != 0 && (millis() - dolarPedidoEn) > intervaloRequestDolar || dolarPedidoEn == 0)
    {
        dolarPedidoEn = millis();
        mostrar = true;

        valor = ObtenerDolarBlue();
        frase = "D:" + String(valor, 0);

        numeroDeRequest++;
    }
    if (mostrar)
    {
        if (ultimoValor < valor)
        {
            dolarSubiendo = 1;
        }
        else if (ultimoValor > valor)
        {
            dolarSubiendo = 0;
        }
        else if (ultimoValor == valor)
        {
            dolarSubiendo = 2;
        }

        debugPrint("| Ultimo valor: ");
        debugPrint(ultimoValor);
        debugPrint(" | Actual valor: ");
        debugPrint(valor);
        debugPrint(" | Dolar Subiendo: ");
        debugPrint(dolarSubiendo);
        debugPrintln(" |");

        ultimoDolar = frase;
        ultimoValor = valor;

        return frase;
    }
    else
    {
        return ultimoDolar;
    }
}

void Pantalla::ControladorClima()
{
    if (climaPedidoEn != 0 && (millis() - climaPedidoEn) > intervaloRequestClima || climaPedidoEn == 0)
    {
        climaPedidoEn = millis();
        ObtenerClima();
    }
}

void Pantalla::MedirLocal()
{
    unsigned long currentMillis = millis(); // Obtiene el tiempo actual

    // Comprueba si ha pasado el intervalo deseado
    if ((currentMillis - previousMillis >= intervalCheckEntorno) || previousMillis == 0)
    {
        // Guarda el tiempo actual para la próxima comparación
        previousMillis = currentMillis;

        // Llama a la función que deseas ejecutar cada 5 segundos
        tempLocal = dht.readTemperature();
        humLocal = dht.readHumidity();

        if (isnan(tempLocal) || isnan(humLocal))
        {
            debugPrintln("Error al leer el sensor DHT11_1");
            tempLocal = 0;
            humLocal = 0;
            return;
        }

        debugPrintln(" -----------------------");
        debugPrintln("| T: " + String(tempLocal) + "c | H: " + String(humLocal) + "% |");
        debugPrintln(" -----------------------");
    }
}
