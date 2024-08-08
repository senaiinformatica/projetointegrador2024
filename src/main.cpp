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
#include "iot.h"
#include "saidas.h"
#include "entradas.h"
#include "tempo.h"
#include "sensores.h"
#include <ArduinoJson.h>

//Definição dos tópicos de publicação
#define mqtt_pub_topic1 "projeto_integrado/SENAI134/Cienciadedados/GrupoX"

JsonDocument doc;

//Variáveis globais


void setup()
{
  Serial.begin(115200);
  setup_wifi();
  setup_time();
  inicializa_entradas();
  inicializa_saidas();
  inicializa_mqtt();
  sensores_init();
}

void loop()
{
  atualiza_time();
  atualiza_saidas();
  atualiza_botoes();
  atualiza_mqtt();

  float temperatureRead = sensores_get_temperatura();
  float pressureRead = sensores_get_presion();
  float altitudeRead = sensores_get_altitud();

  doc["temperature"] = temperatureRead;
  doc["pressure"] = pressureRead;
  doc["altitude"] = altitudeRead;

  String json;
  serializeJson(doc, json);

  publica_mqtt(mqtt_pub_topic1, json);
  delay(1000);
}


