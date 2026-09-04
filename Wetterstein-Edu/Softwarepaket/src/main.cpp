/**
 * Wetterstation Hauptdatei
 */
#include <Arduino.h> // Grundlegende Arduino-Funktionen
#include <Wire.h>   // I2C Kommunikation
#include <Adafruit_BME280.h> // BME280 Sensor
#include <GyverOLED.h> // OLED Display
#include <FastLED.h> // LED Steuerung
//#include <ESP8266WiFi.h> // WLAN

#include "WifiManager.h"  // WLAN Management
#include "WetterAPI.h"  // Wetter API

// Globale Messwerte
float temperature = 0;
float humidity    = 0;
float pressure    = 0;
float altitude    = 0;

// Zugangsdaten des Konfigurations-Access-Points.
// Die WLAN-Daten selbst werden ueber diesen AP eingetragen und im EEPROM gespeichert.
const char* AP_SSID     = "Wetterstation-Setup";   // Name des Setup-Netzes
const char* AP_PASSWORD = "wetter1234";            // min. 8 Zeichen, sonst offenes Netz

// Sensor / Display
Adafruit_BME280 bme;  // BME280 Sensor
bool bmeOk = false;   // true, wenn der Sensor beim Start gefunden wurde
#define SEALEVELPRESSURE_HPA (1013.25) // Standard Luftdruck auf Meereshöhe
GyverOLED<SSD1306_128x64> oled; // OLED Display

// LED
#define LED_PIN 2 // Pin für die LED
#define NUM_LEDS 1 // Anzahl der LEDs
CRGB leds[NUM_LEDS]; // LED Array

// Wetter / Server
WifiManager wifi(AP_SSID, AP_PASSWORD);
// Der Ort ist nur ein Startwert - er wird im Setup-Portal eingetragen
WetterAPI wetter("Aachen,de", "285295f704919618f44a9670b4a760e3");
WiFiServer server(80);

// Timer
unsigned long lastForecastUpdate = 0;
unsigned long lastCurrentUpdate  = 0;
unsigned long lastSerialUpdate   = 0;
const unsigned long forecastInterval = 30UL * 60UL * 1000UL;
const unsigned long currentInterval  = 10UL * 60UL * 1000UL;
const unsigned long serialInterval   = 1UL * 60UL * 1000UL;

// Button
#define BUTTON_PIN D3
int buttonCase = 0;
// Debounce
const unsigned long debounceMs = 180;
static unsigned long lastChange = 0;
static bool pressedHandled = false;
static int lastButtonState = HIGH;
int lastDisplayedCase = -1;   // force first draw
// Langer Tastendruck (3 s) loescht die gespeicherten WLAN-Daten
const unsigned long resetHoldMs = 3000;
static bool resetHandled = false;

// Zeigt alle Adressen, die am I2C-Bus antworten. Hilft beim Suchen von
// Verdrahtungsfehlern oder abweichenden Sensor-Adressen.
void scanI2C() {
  Serial.println(F("I2C-Scan:"));
  int gefunden = 0;
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.printf("  Geraet gefunden auf 0x%02X\r\n", addr);
      gefunden++;
    }
    yield();
  }
  if (gefunden == 0) Serial.println(F("  Nichts gefunden - Verkabelung pruefen (SDA=D2, SCL=D1, 3V3, GND)"));
}

// LED Helper
void updateLedFromHumidity(float h) {
  if      (h < 10) leds[0] = CRGB::Red;
  else if (h < 20) leds[0] = CRGB::OrangeRed;
  else if (h < 30) leds[0] = CRGB::Orange;
  else if (h < 40) leds[0] = CRGB::Gold;
  else if (h < 50) leds[0] = CRGB::Yellow;
  else if (h < 60) leds[0] = CRGB::YellowGreen;
  else if (h < 70) leds[0] = CRGB::Green;
  else if (h < 80) leds[0] = CRGB::MediumBlue;
  else if (h < 90) leds[0] = CRGB::Blue;
  else              leds[0] = CRGB::DarkBlue;
  FastLED.show();
}

// HTTP Seite ausgeben
void sendPage(WiFiClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html; charset=utf-8");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE html><html><head><title>Sensordaten Wetterstation</title><meta charset='utf-8'>");
  client.println("<meta http-equiv='refresh' content='5'>");
  client.println("<meta name='viewport' content='width=device-width,initial-scale=1'>");
  client.println("<style>"
                 "html{font-family:Arial;text-align:center;}"
                 "body{margin:0;}"
                 ".topnav{background:#428f9c;color:#fff;padding:6px 0;font-size:1.4rem;}"
                 ".cards{max-width:1400px;margin:0 auto;display:grid;grid-gap:2rem;grid-template-columns:repeat(auto-fit,minmax(300px,1fr));}"
                 ".cards1{max-width:700px;margin:30px auto;display:grid;grid-gap:2rem;grid-template-columns:repeat(auto-fit,minmax(500px,1fr));}"
                 ".card{background:#fff;box-shadow:2px 2px 12px 1px rgba(140,140,140,.5);padding:10px;}"
                 ".reading{font-size:2.2rem;margin:4px 0;}"
                 ".temperature{color:#38AFA9;}"
                 ".humidity{color:#17bebb;}"
                 ".pressure{color:#3bb963;}"
                 ".altitude{color:#e2b205;}"
                 "</style></head><body>");
  client.println("<div class='topnav'><h3>Wetterstation</h3></div>");

  client.println("<div class='content'>");
  client.println("<div class='cards'>");

  client.println("<div class='card temperature'><h4>Temperatur</h4>");
  client.printf("<p class='reading'>%.1f &deg;C</p></div>", temperature);

  client.println("<div class='card humidity'><h4>Feuchte</h4>");
  client.printf("<p class='reading'>%.1f %%</p></div>", humidity);

  client.println("<div class='card pressure'><h4>Druck</h4>");
  client.printf("<p class='reading'>%.1f hPa</p></div>", pressure);

  client.println("</div>"); // cards

  client.println("<div class='cards1'>");
  client.printf("<div class='card'><h2>Aktuelles Wetter</h2><p>%s</p>", wetter.getCity().c_str());
  client.printf("Zeit: %s<br>Temp: %s &deg;C<br>Wetter: %s<br>Wind: %s m/s<br><br>",
                wetter.currentWeather[0][0].c_str(),
                wetter.currentWeather[0][1].c_str(),
                wetter.currentWeather[0][2].c_str(),
                wetter.currentWeather[0][3].c_str());
  client.println("</div>");

  client.println("<div class='card'><h2>Vorhersage 24h</h2>");
  for (int i = 0; i < 8; i++) {
    client.printf("Zeit: %s<br>Temp: %s &deg;C<br>Wetter: %s<br>Wind: %s m/s<br><br>",
                  wetter.forecastData[i][0].c_str(),
                  wetter.forecastData[i][1].c_str(),
                  wetter.forecastData[i][2].c_str(),
                  wetter.forecastData[i][3].c_str());
  }
  client.println("</div>");

  client.println("</div>"); // cards1
  client.println("</div>"); // content
  client.println("</body></html>");
  client.println();
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(115200);
  Wire.begin();
  delay(50);

  oled.init();
  oled.clear();

  // Sensor auf beiden ueblichen Adressen suchen. Wird er nicht gefunden,
  // laeuft die Station trotzdem weiter - nur ohne eigene Messwerte.
  bmeOk = bme.begin(0x76) || bme.begin(0x77);
  if (!bmeOk) {
    Serial.println(F("BME280 nicht gefunden (weder 0x76 noch 0x77)."));
    scanI2C();
    oled.setCursor(0,0); oled.print("BME280 fehlt!");
    oled.setCursor(0,2); oled.print("Verkabelung");
    oled.setCursor(0,4); oled.print("pruefen.");
    oled.update();
    delay(2000);
    oled.clear();
  }

  FastLED.addLeds<WS2812, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(255);

  wifi.begin();   // gespeicherte Daten laden, verbinden oder Setup-AP oeffnen

  if (wifi.isConnected()) {
    server.begin();
    wetter.setCity(wifi.getCity());   // Ort aus dem Setup-Portal uebernehmen
    wetter.fetchCurrentWeather();
    wetter.fetchWeatherForecast();
  } else {
    // Konfigurations-Portal laeuft: Hinweis auf dem Display
    oled.clear();
    oled.setCursor(0,0); oled.print("WLAN einrichten:");
    oled.setCursor(0,2); oled.print(AP_SSID);
    oled.setCursor(0,4); oled.print("PW: "); oled.print(AP_PASSWORD);
    oled.setCursor(0,6); oled.print("IP: "); oled.print(wifi.getIP());
    oled.update();
  }
}

void loop() {
  // Solange das Konfigurations-Portal laeuft, nur dieses bedienen
  if (wifi.isPortalActive()) {
    wifi.loop();
    return;
  }

  wifi.checkSerialSetup();   // Einrichtung per USB jederzeit moeglich

  // --- Button poll & debounce ---
  int s = digitalRead(BUTTON_PIN);
  if (s != lastButtonState) {
    lastChange = millis();
    lastButtonState = s;
  }
  if (s == LOW && !pressedHandled && (millis() - lastChange) > debounceMs) {
    buttonCase = (buttonCase + 1) % 6;
    pressedHandled = true;
  }
  if (s == LOW && pressedHandled && !resetHandled && (millis() - lastChange) > resetHoldMs) {
    // Taste laenger als 3 s gehalten: WLAN-Daten loeschen und neu starten
    resetHandled = true;
    oled.clear();
    oled.setCursor(0,0); oled.print("WLAN-Daten");
    oled.setCursor(0,2); oled.print("geloescht.");
    oled.setCursor(0,4); oled.print("Neustart...");
    oled.update();
    wifi.resetCredentials();
    delay(1000);
    ESP.restart();
  }
  if (s == HIGH) { pressedHandled = false; resetHandled = false; }

  // Sensorwerte einlesen (nur wenn der Sensor gefunden wurde)
  if (bmeOk) {
    temperature = bme.readTemperature();
    humidity    = bme.readHumidity();
    pressure    = bme.readPressure() / 100.0F;
    altitude    = bme.readAltitude(SEALEVELPRESSURE_HPA);
  }

  // Nur neu zeichnen wenn sich buttonCase änderte
  if (buttonCase != lastDisplayedCase) {
    oled.clear();
    switch (buttonCase) {
      case 0: 
        oled.setCursor(0,0); oled.print("Wetterstation");
        oled.setCursor(0,2); oled.print("WLAN: "); oled.print(wifi.getSSID());
        oled.setCursor(0,4); oled.print("IP: "); oled.print(wifi.getIP());
        oled.setCursor(0,6); oled.print("Ort: "); oled.print(wetter.getCity());
        break;
      case 1:
        if (!bmeOk) { oled.setCursor(0,0); oled.print("BME280 fehlt!"); break; }
        oled.setCursor(0,0); oled.print("Temp: "); oled.print(temperature,1); oled.print(" C");
        oled.setCursor(0,2); oled.print("Feuchte: "); oled.print(humidity,1); oled.print(" %");
        oled.setCursor(0,4); oled.print("Druck: "); oled.print(pressure,1); oled.print(" hPa");
        oled.setCursor(0,6); oled.print("Hoehe: "); oled.print(altitude,1); oled.print(" m");
        break;
      case 2:
        oled.setCursor(0,0); oled.print("Aktuelle Wetter:");
        oled.setCursor(0,2); oled.printf("Temp: %s C", wetter.currentWeather[0][1].c_str());
        oled.setCursor(0,4); oled.printf("Wetter: %s", wetter.currentWeather[0][2].c_str());
        oled.setCursor(0,6); oled.printf("Wind: %s",   wetter.currentWeather[0][3].c_str());
        break;
      case 3:
      case 4:
      case 5: {
        int idx = buttonCase - 3;
        oled.setCursor(0,0); oled.printf("%s", wetter.forecastData[idx][0].c_str());
        oled.setCursor(0,2); oled.printf("Temp: %s C", wetter.forecastData[idx][1].c_str());
        oled.setCursor(0,4); oled.printf("Wetter: %s", wetter.forecastData[idx][2].c_str());
        oled.setCursor(0,6); oled.printf("Wind: %s",   wetter.forecastData[idx][3].c_str());
        break;
      }
    }
    oled.update();
    lastDisplayedCase = buttonCase;
  }

  // Periodische Updates
  unsigned long now = millis();
  if (now - lastForecastUpdate >= forecastInterval) {
    wetter.fetchWeatherForecast();
    lastForecastUpdate = now;
  }
  if (now - lastCurrentUpdate >= currentInterval) {
    wetter.fetchCurrentWeather();
    lastCurrentUpdate = now;
  }
  if (now - lastSerialUpdate >= serialInterval) {
    lastSerialUpdate = now;
    Serial.printf("Temp: %.1f C | Feuchte: %.1f %% | Druck: %.1f hPa | Höhe: %.1f m\r\n",
                  temperature, humidity, pressure, altitude);
  }

  // LED Farbe
  if (bmeOk) updateLedFromHumidity(humidity);

  // OLED Anzeige
  oled.clear();
  switch (buttonCase) {
    case 0: 
        oled.setCursor(0,0); oled.print("Wetterstation");
        oled.setCursor(0,2); oled.print("WLAN: "); oled.print(wifi.getSSID());
        oled.setCursor(0,4); oled.print("IP: "); oled.print(wifi.getIP());
        oled.setCursor(0,6); oled.print("Ort: "); oled.print(wetter.getCity());
        break;
    case 1:
      if (!bmeOk) { oled.setCursor(0, 0); oled.print("BME280 fehlt!"); break; }
      oled.setCursor(0, 0);
      oled.print("Temp: ");   oled.print(temperature, 1); oled.print(" C");
      oled.setCursor(0, 2);
      oled.print("Feuchte: "); oled.print(humidity, 1);   oled.print(" %");
      oled.setCursor(0, 4);
      oled.print("Druck: ");  oled.print(pressure, 1);    oled.print(" hPa");
      oled.setCursor(0, 6);
      oled.print("Hoehe: ");  oled.print(altitude, 1);    oled.print(" m");
      break;
    case 2:
      oled.setCursor(0, 0); oled.print("Aktuelle Wetter:");
      oled.setCursor(0, 2); oled.printf("Temp: %s C", wetter.currentWeather[0][1].c_str());
      oled.setCursor(0, 4); oled.printf("Wetter: %s", wetter.currentWeather[0][2].c_str());
      oled.setCursor(0, 6); oled.printf("Wind: %s m/s", wetter.currentWeather[0][3].c_str());
      break;
    case 3:
      oled.setCursor(0, 0); oled.printf("%s", wetter.forecastData[0][0].c_str());
      oled.setCursor(0, 2); oled.printf("Temp: %s C", wetter.forecastData[0][1].c_str());
      oled.setCursor(0, 4); oled.printf("Wetter: %s", wetter.forecastData[0][2].c_str());
      oled.setCursor(0, 6); oled.printf("Wind: %s m/s", wetter.forecastData[0][3].c_str());
      break;
    case 4:
      oled.setCursor(0, 0); oled.printf("%s", wetter.forecastData[1][0].c_str());
      oled.setCursor(0, 2); oled.printf("Temp: %s C", wetter.forecastData[1][1].c_str());
      oled.setCursor(0, 4); oled.printf("Wetter: %s",  wetter.forecastData[1][2].c_str());
      oled.setCursor(0, 6); oled.printf("Wind: %s m/s", wetter.forecastData[1][3].c_str());
      break;
    case 5:
      oled.setCursor(0, 0); oled.printf("%s", wetter.forecastData[2][0].c_str());
      oled.setCursor(0, 2); oled.printf("Temp: %s C", wetter.forecastData[2][1].c_str());
      oled.setCursor(0, 4); oled.printf("Wetter: %s", wetter.forecastData[2][2].c_str());
      oled.setCursor(0, 6); oled.printf("Wind: %s m/s", wetter.forecastData[2][3].c_str());
      break;
  }
  oled.update();

  // HTTP (einfacher synchroner Server)
  WiFiClient client = server.accept();
  if (client) {
    String currentLine;
    unsigned long timeout = millis();
    while (client.connected() && (millis() - timeout < 5000)) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            sendPage(client);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
        timeout = millis();
      }
      yield();
    }
    client.stop();
  }
  delay(50);
}
