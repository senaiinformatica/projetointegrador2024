#include <Adafruit_BMP280.h>
#include <DHTesp.h>
#include <MQUnifiedsensor.h>
#include "sensores.h"

#define MQ_135_PIN 34
#define ADC_RESOLUTION_BIT 12

Adafruit_BMP280 bmp;
DHTesp dht;
MQUnifiedsensor MQ135("ESP-32", 3.3, ADC_RESOLUTION_BIT, MQ_135_PIN, "MQ-135");

void sensores_init()
{
  if (!bmp.begin(0x76))
  {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
  }
  dht.setup(5, DHTesp::DHT11);

   // Define o método de regressão: 1 -> Exponencial || 2 -> Linear
  MQ135.setRegressionMethod(1);  //_PPM =  a * ratio ^ b

  // Configura os coeficientes da equação para calcular a concentração de CO
  MQ135.setA(36974);
  MQ135.setB(-3.109);

  /*
    Coeficientes de regressão exponencial para diferentes gases:
    Gas     | a       | b
    H2      | 987.99  | -2.162
    LPG     | 574.25  | -2.222
    CO      | 36974   | -3.109
    Alcohol | 3616.1  | -2.675
    Propane | 658.71  | -2.168
  */
  
  MQ135.init();

  Serial.print("Calibrando, por favor aguarde.");
  float calcR0 = 0;
  
  for (int i = 1; i <= 10; i++) {
    MQ135.update(); // Atualiza os dados, o ESP32 lerá a tensão do pino analógico
    calcR0 += MQ135.calibrate(4.4);
    Serial.print(".");
  }

  calcR0 /= 10;
  MQ135.setR0(calcR0);
  Serial.println("  Concluído!");

  // Verificações de erro na calibração
  if (isinf(calcR0)) {
    Serial.println("Aviso: Problema de conexão, R0 é infinito (Circuito aberto detectado). Verifique o cabeamento e a alimentação.");
  } else if (calcR0 == 0) {
    Serial.println("Aviso: Problema de conexão detectado, R0 é zero (Pino analógico em curto-circuito com o terra). Verifique o cabeamento e a alimentação.");
  } else {
    Serial.print("R0 calibrado para: ");
    Serial.println(calcR0);
  }
}

float sensores_get_temperatura()
{
  return bmp.readTemperature();
}

float sensores_get_presion()
{
  return bmp.readPressure();
}

float sensores_get_altitud()
{
  return bmp.readAltitude(1020); // Ajuste o valor de pressão para o nível do mar
}

float sensores_get_humity()
{
  return dht.getHumidity();
}

float sensores_get_gas()
{
  MQ135.update();
  return MQ135.readSensor();
}