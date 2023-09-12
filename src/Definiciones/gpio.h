#ifndef GPIOs_H
#define GPIOs_H

// Encoder
// CLK - GPIO2
#define PIN_IN1 2
// DT - GPIO15
#define PIN_IN2 15
// SW - GPIO4
#define BotonEncoder 4

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
// Nivel de boton apretado
#define presionado HIGH // LOW
// Defino posicion del GPIO del boton del encoder entro del array para usar en logica luego.
#define posicionBotonEncoder 8
// Cantidad de botones
#define cantBotones 9
// Relacionado a mensajes[] en ControladorBotones
const int gpioBotones[cantBotones] = {
    13,           // Boton A
    12,           // Boton B
    14,           // Boton C
    27,           // Boton D
    26,           // Boton E
    25,           // Boton F
    33,           // Boton G
    32,           // Boton H
    BotonEncoder, // Boton I encoder (4) posicion 8.
};

// Bitmask para pines que pueden despertar al ESP
#define GPIO_BITMASK (GPIO_SEL_4 | GPIO_SEL_12 | GPIO_SEL_13 | GPIO_SEL_14 | GPIO_SEL_25 | GPIO_SEL_26 | GPIO_SEL_27 | GPIO_SEL_32 | GPIO_SEL_33)

#endif