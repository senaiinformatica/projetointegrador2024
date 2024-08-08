#include <Arduino.h>
#include "saidas.h"

//Definição dos pinos dos leds
#define LedBuiltInPin 2
#define LedExterno 25
#define Buzzer 19

//Variaveis de controle dos leds
bool LedBuiltInState = LOW;
bool LedExternoState = LOW;

//Inicializa as saídas digitais
void inicializa_saidas()
{
    pinMode(LedBuiltInPin, OUTPUT);
    pinMode(LedExterno, OUTPUT);
    pinMode(Buzzer, OUTPUT);
    ledcSetup(0, 5000, 8);
    ledcAttachPin(Buzzer, 0);
}

//Atualiza as saídas digitais
void atualiza_saidas()
{
    digitalWrite(LedBuiltInPin, LedBuiltInState);
    digitalWrite(LedExterno, LedExternoState);
}