#ifndef WETTERAPI_H
#define WETTERAPI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

class WetterAPI {
  public:
    WetterAPI(const String& stadt, const String& apiSchluessel);
    void begin();
    void setCity(const String& stadt);   // Ort aus dem Setup-Portal uebernehmen
    String getCity();
    void fetchCurrentWeather();
    void fetchWeatherForecast();
    
    String currentWeather[1][4];
    String forecastData[8][4];

  private:
    String city;
    String apiKey;
};

#endif