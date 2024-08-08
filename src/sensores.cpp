#include <Adafruit_BMP280.h>
#include "sensores.h"

Adafruit_BMP280 bmp;

void sensores_init() {
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
}

float sensores_get_temperatura() {
  return bmp.readTemperature();
}

float sensores_get_presion() {
  return bmp.readPressure();
}

float sensores_get_altitud() {
  return bmp.readAltitude(1013.25);
}


