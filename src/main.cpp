/*************************
 * Projeto IOT com ESP32
 * Escola SENAI de Informática
 * Curso de Apredizagem industrial
 * Integrador de soluções em nuvem
 * Data: 01/08/2024
 *
 * Autor: Thiago Augusto de Oliveira
 *
 * Descrição: Projeto de automação utilizando ESP32
 * com conexão WiFi e MQTT.
 *
 * versão: 0.9
 *************************/

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include "iot.h"
#include "saidas.h"
#include "entradas.h"
#include "tempo.h"
#include "sensores.h"

// Definição dos tópicos de publicação
#define mqtt_pub_topic1 "projeto_integrado/SENAI134/Cienciadedados/GrupoX"

JsonDocument doc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variáveis globais
unsigned long lastTime = 0;
const unsigned long timerDelay = 1000;

void sensores();
void envia_dados();
void atualiza_display();
void inicializa_display();

void setup()
{
  Serial.begin(115200);
  inicializa_display();
  setup_wifi();
  setup_time();
  inicializa_entradas();
  inicializa_saidas();
  inicializa_mqtt();
  sensores_init();

  int melody[] = {
      440, 440, 440, 349, 523, 440, 349, 523, 440,
      659, 659, 659, 698, 523, 784, 740, 698, 659};
  int tempo[] =
      {
          500, 500, 500, 350, 150, 500, 350, 150, 1000,
          500, 500, 500, 350, 150, 500, 350, 150, 1000};
  for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++)
  {
    ledcWrite(0, melody[i]);
    delay(tempo[i] * 1.30);
    ledcWrite(0, 0);
  }
}

void loop()
{
  atualiza_time();
  atualiza_saidas();
  atualiza_botoes();
  atualiza_mqtt();
  sensores();
  if (millis() - lastTime > timerDelay)
  {
    lastTime = millis();
    envia_dados();
    atualiza_display();
  }
}

void sensores()
{
  float temperatureRead = sensores_get_temperatura();
  float pressureRead = sensores_get_presion();
  float altitudeRead = sensores_get_altitud();
  float humityRead = sensores_get_humity();

  doc["temperature"] = round(temperatureRead * 100.0) / 100.0;
  doc["pressure"] = round(pressureRead * 100.0) / 100.0;
  doc["altitude"] = round(altitudeRead * 100.0) / 100.0;
  doc["humidity"] = round(humityRead * 1.0);
  doc["CO2"] = round(sensores_get_gas() * 100.0) / 100.0;
  doc["timestamp"] = timeStamp();
}

void envia_dados()
{
  String json;
  serializeJson(doc, json);
  publica_mqtt(mqtt_pub_topic1, json);
}

void inicializa_display()
{
  byte sinalExcelente[8] = {
      0b00001,
      0b00001,
      0b00011,
      0b00011,
      0b00111,
      0b00111,
      0b01111,
      0b11111};
  byte sinalBom[8] = {
      0b00000,
      0b00000,
      0b00010,
      0b00010,
      0b00110,
      0b00110,
      0b01110,
      0b11110};
  byte sinalMedio[8] = {
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00100,
      0b00100,
      0b01100,
      0b11100};

  byte sinalFraco[8] = {
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b01000,
      0b11000};
  byte semSinal[8] = {
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b10000};
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, sinalExcelente);
  lcd.createChar(1, sinalBom);
  lcd.createChar(2, sinalMedio);
  lcd.createChar(3, sinalFraco);
  lcd.createChar(4, semSinal);

  lcd.setCursor(5, 0);
  lcd.print("C");
  lcd.setCursor(13, 0);
  lcd.print("kPa");
}

void atualiza_display()
{
  float temperatureRead = sensores_get_temperatura();
  float pressureRead = sensores_get_presion();
  float altitudeRead = sensores_get_altitud();

  lcd.setCursor(0, 0);
  lcd.print("    ");
  lcd.setCursor(0, 0);
  lcd.print(temperatureRead, 1);

  lcd.setCursor(8, 0);
  lcd.print("     ");
  lcd.setCursor(8, 0);
  lcd.print(pressureRead / 1000, 1);

  switch (sinal())
  {
  case 0:
    lcd.setCursor(15, 1);
    lcd.write(byte(0));
    break;
  case 1:
    lcd.setCursor(15, 1);
    lcd.write(byte(1));
    break;
  case 2:
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
    break;
  case 3:
    lcd.setCursor(15, 1);
    lcd.write(byte(3));
    break;
  case 4:
    lcd.setCursor(15, 1);
    lcd.write(byte(4));
  }
}