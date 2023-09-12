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

void Encoder::EncoderLoop()
{
    static int pos = 0;

    encoder->tick(); // just call tick() to check the state.

    int newPos = encoder->getPosition();
    if (pos != newPos && digitalRead(BotonEncoder) == presionado)
    {
        int dir = (int)(encoder->getDirection());

        debugPrint("| pos:");
        debugPrint(newPos);
        debugPrint(" | dir:");
        debugPrint(dir);
        debugPrintln(" |");

        pos = newPos;

        InterrumpirAnimaciones(dir);
    }
    else if (pos != newPos)
    {
        giro = newPos;
    }
}

void Encoder::InterrumpirAnimaciones(int dir)
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

        Pitido(2000);
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

        Pitido(5000);
    }
}

void Encoder::Pitido(int tono)
{
    tone(BUZZER_PIN, tono, 0.9);
    delay(25);
    noTone(BUZZER_PIN);
}

void Encoder::Iniciar()
{
    encoder = new RotaryEncoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);

    // register interrupt routine
    attachInterrupt(digitalPinToInterrupt(PIN_IN1), Encoder::StaticCheckPosition, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_IN2), Encoder::StaticCheckPosition, CHANGE);
    // pinMode(botonPin, INPUT_PULLUP);

    // Buzzer test
    ledcSetup(buzzerChannel, buzzerFreq, buzzerResolution); // Cambia 1000 a la frecuencia inicial deseada
    ledcAttachPin(BUZZER_PIN, buzzerChannel);
}

void Encoder::StaticCheckPosition()
{
    encoderInstance->CheckPosition();
}

IRAM_ATTR void Encoder::CheckPosition()
{
    encoder->tick(); // just call tick() to check the state.
}
