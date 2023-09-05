#ifndef GPIOs_H
#define GPIOs_H

// Encoder
// CLK - GPIO2
#define PIN_IN1 2
// DT - GPIO15
#define PIN_IN2 15
// SW - GPIO4
const int botonPin = 4;

// Oled - Define las pines para la comunicación I2C con la pantalla OLED
// SDA - GPIO23
#define OLED_SDA 23
// SCL - GPIO 22
#define OLED_SCL 22

// DHT11
// Pin donde está conectado el sensor DHT11 (Laber RX2)
#define DHT_PIN 16

// Buzzer
#define BUZZER_PIN 17

// Botones
const int cantBotones = 9;
// Relacionado a mensajes[] en ControladorBotones
const int gpioBotones[cantBotones] = {
    13,       // Boton A
    12,       // Boton B
    14,       // Boton C
    27,       // Boton D
    26,       // Boton E
    25,       // Boton F
    33,       // Boton G
    32,       // Boton H
    botonPin, // Boton I encoder (4)
};

#endif