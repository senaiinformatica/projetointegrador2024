#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "iot.h"
#include "senhas.h"
#include "saidas.h"
#include "tempo.h"


// Definição dos tópicos de inscrição
#define mqtt_topic1 "projeto_integrado/SENAI134/Cienciadedados/controle"

// Definição do ID do cliente MQTT randomico
const String cliente_id = "ESP32Clientsenai" + String(random(0xffff), HEX);

// Definição dos dados de conexão
WiFiClient espClient;
PubSubClient client(espClient);

// Protótipos das funções
void tratar_msg(char *topic, String msg);
void callback(char *topic, byte *payload, unsigned int length);
void reconecta_mqtt();
void inscricao_topicos();

// Inicia a conexão WiFi
void setup_wifi()
{
  Serial.println();
  Serial.print("Conectando-se a Rede WiFi ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado ao WiFi com sucesso com IP: ");
  Serial.println(WiFi.localIP());
}

// Inicia a conexão MQTT
void inicializa_mqtt()
{
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// Atualiza a conexão MQTT
void atualiza_mqtt()
{
  client.loop();
  if (!client.connected())
  {
    reconecta_mqtt();
  }
}

// Função de callback chamada quando uma mensagem é recebida
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.printf("Mensagem recebida [ %s ] \n\r", topic);
  String msg = "";
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    msg += (char)payload[i];
  }
  Serial.println();
  tratar_msg(topic, msg);
}

// Função de reconexão ao Broker MQTT
void reconecta_mqtt()
{
  while (!client.connected())
  {
    Serial.print("Tentando se conectar ao Broker MQTT: ");
    Serial.println(mqtt_server);
    if (client.connect(cliente_id.c_str()))
    {
      Serial.println("Conectado ao Broker MQTT");
      inscricao_topicos();
    }
    else
    {
      Serial.println("Falha ao conectar ao Broker.");
      Serial.println("Havera nova tentativa de conexao em 2 segundos");
      delay(2000);
    }
  }
}

// Publica uma mensagem no tópico MQTT
void publica_mqtt(String topico, String msg)
{
  client.publish(topico.c_str(), msg.c_str());
}
byte sinal()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    int rssi = WiFi.RSSI();
    if (rssi > -50)
    {
      return 4;
    }
    else if (rssi > -60)
    {
      return 3;
    }
    else if (rssi > -70)
    {
      return 2;
    }
    else if (rssi > -80)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
}
//!----------------------------------------
// TODO Alterar a programação apartir daqui
//!----------------------------------------

// Inscreve nos tópicos MQTT
void inscricao_topicos()
{
  client.subscribe(mqtt_topic1);
}

// Trata as mensagens recebidas
void tratar_msg(char *topic, String msg)
{
  // Tratamento do tópico 1
  if (strcmp(topic, mqtt_topic1) == 0)
  {
    JsonDocument doc;
    deserializeJson(doc, msg);
    if (doc.containsKey("freqEnvSec")) //tem o campo token?
    {

      timerDelay = int(doc["freqEnvSec"]) * 1000;
      salvarValor();
    }

  }
}