#include "Encoder.h"
#include "Definiciones/debug.h"

extern bool interrumpirAnimacion;
extern int menuPos;
extern int macroPos;
extern int giro;

RotaryEncoder *encoder = nullptr;

static Encoder *encoderInstance;

Encoder::Encoder()
{
    encoderInstance = this;
}

void Encoder::encoderLoop()
{
    static int pos = 0;

    encoder->tick(); // just call tick() to check the state.

    int newPos = encoder->getPosition();
    if (pos != newPos && digitalRead(botonPin) == LOW)
    {
        int dir = (int)(encoder->getDirection());

        debugPrint("| pos:");
        debugPrint(newPos);
        debugPrint(" | dir:");
        debugPrint(dir);
        debugPrintln(" |");

        pos = newPos;

        interrumpirAnimaciones(dir);
    }
    else if (pos != newPos)
    {
        giro = newPos;
    }
}

void Encoder::interrumpirAnimaciones(int dir)
{
    if (dir == -1)
    {
        interrumpirAnimacion = true;

        if (menuPos <= 2)
        {
            menuPos++;
        }
        else
        {
            menuPos = 1;
        }

        pitido(2000);

        /* ledcWriteTone(buzzerChannel, tonoAlto);
        delay(20);
        ledcWriteTone(buzzerChannel, 0); // Detiene el tono (frecuencia 0) */
    }
    if (dir == 1)
    {
        if (macroPos <= 2)
        {
            macroPos++;
        }
        else
        {
            macroPos = 1;
        }

        pitido(5000);
        /* ledcWriteTone(buzzerChannel, tonoBajo);
        delay(20);
        ledcWriteTone(buzzerChannel, 0); // Detiene el tono (frecuencia 0) */
    }
}

void Encoder::pitido(int tono)
{
    tone(BUZZER_PIN, tono, 0.9);
    delay(25);
    noTone(BUZZER_PIN);
}

void Encoder::iniciar()
{
    encoder = new RotaryEncoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);

    // register interrupt routine
    attachInterrupt(digitalPinToInterrupt(PIN_IN1), Encoder::staticCheckPosition, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_IN2), Encoder::staticCheckPosition, CHANGE);
    // pinMode(botonPin, INPUT_PULLUP);

    // Buzzer test
    ledcSetup(buzzerChannel, buzzerFreq, buzzerResolution); // Cambia 1000 a la frecuencia inicial deseada
    ledcAttachPin(BUZZER_PIN, buzzerChannel);
}

void Encoder::staticCheckPosition()
{
    encoderInstance->checkPosition();
}

IRAM_ATTR void Encoder::checkPosition()
{
    encoder->tick(); // just call tick() to check the state.
}
