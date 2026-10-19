/* =============================================================
   LOESUNG zu Aufgabe 4 - Wettersensor BME280

   Wird NICHT mitkompiliert. Nur die fehlenden Zeilen in
   src/main_BME.cpp uebertragen.
   ============================================================= */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <GyverOLED.h>
#include <FastLED.h>

Adafruit_BME280 bme;
#define SEALEVELPRESSURE_HPA (1013.25)

GyverOLED<SSD1306_128x64> oled;

#define LED_PIN 2
#define NUM_LEDS 1
CRGB wetterLeds[NUM_LEDS];


void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(100);

  oled.init();
  oled.clear();
  oled.update();

  if (!bme.begin(0x76)) {
    Serial.println("BME280 nicht gefunden! Stecker pruefen.");
    oled.clear();
    oled.setCursor(0, 3);
    oled.print("BME nicht gefunden");
    oled.update();
    while (true) {
      delay(1000);
    }
  }

  FastLED.addLeds<WS2812, LED_PIN>(wetterLeds, NUM_LEDS);
  FastLED.setBrightness(80);
}


void loop() {
  // --- Loesung 4.1 ---
  float temperatur  = bme.readTemperature();
  float luftfeuchte = bme.readHumidity();
  float luftdruck   = bme.readPressure() / 100.0;
  float hoehe       = bme.readAltitude(SEALEVELPRESSURE_HPA);
  // -------------------

  Serial.print("Temp: ");   Serial.print(temperatur);
  Serial.print(" C   Feuchte: "); Serial.print(luftfeuchte);
  Serial.print(" %   Druck: ");   Serial.print(luftdruck);
  Serial.print(" hPa   Hoehe: "); Serial.print(hoehe);
  Serial.println(" m");

  // --- Loesung 4.2 ---
  oled.clear();

  oled.setCursor(0, 0);
  oled.print("Temp:  ");
  oled.print(temperatur);
  oled.print(" C");

  oled.setCursor(0, 2);
  oled.print("Feucht:");
  oled.print(luftfeuchte);
  oled.print(" %");

  oled.setCursor(0, 4);
  oled.print("Druck: ");
  oled.print(luftdruck);
  oled.print(" hPa");

  oled.setCursor(0, 6);
  oled.print("Hoehe: ");
  oled.print(hoehe);
  oled.print(" m");

  oled.update();
  // -------------------

  // --- Loesung 4.3 ---
  if (luftfeuchte < 30) {
    wetterLeds[0] = CRGB::Red;          // sehr trocken
  }
  else if (luftfeuchte < 45) {
    wetterLeds[0] = CRGB::Gold;         // trocken
  }
  else if (luftfeuchte < 60) {
    wetterLeds[0] = CRGB::Green;        // angenehm
  }
  else if (luftfeuchte < 75) {
    wetterLeds[0] = CRGB::Aqua;         // feucht
  }
  else {
    wetterLeds[0] = CRGB::Blue;         // sehr feucht
  }
  // -------------------

  FastLED.show();

  delay(1000);
}


/* -------------------------------------------------------------
   LOESUNGSIDEEN ZU DEN EXTRAS

   EXTRA B - Stufenlose Farbe (ersetzt die if-Kette aus 4.3)

     int farbton = map(luftfeuchte, 0, 100, 0, 160);
     farbton = constrain(farbton, 0, 160);   // sicherheitshalber
     wetterLeds[0] = CHSV(farbton, 255, 255);

     // map(wert, von_min, von_max, nach_min, nach_max)
     // rechnet 0..100 % Feuchte um in 0..160 Farbton,
     // also von Rot ueber Gruen bis Blau.
     // constrain() sorgt dafuer, dass der Wert im Rahmen bleibt,
     // falls der Sensor mal etwas ausserhalb liefert.

   EXTRA C - Hoechst- und Tiefstwert

     // ueber setup():
     float maxTemp = -100;
     float minTemp =  100;

     // in loop(), nach dem Auslesen:
     if (temperatur > maxTemp) { maxTemp = temperatur; }
     if (temperatur < minTemp) { minTemp = temperatur; }

     // vor oled.update(); Zeile 7 ist frei (Zeile 6 = Hoehe)
     oled.setCursor(0, 7);
     oled.print("Min ");
     oled.print(minTemp, 1);
     oled.print("  Max ");
     oled.print(maxTemp, 1);

   EXTRA D - Zwei Display-Seiten mit dem Taster

     #define BUTTON_PIN D3
     int seite = 0;

     // tasterWurdeGedrueckt() aus Aufgabe 3 hierher kopieren
     // und in setup():  pinMode(BUTTON_PIN, INPUT_PULLUP);

     if (tasterWurdeGedrueckt()) {
       seite = seite + 1;
       if (seite >= 2) { seite = 0; }
     }

     oled.clear();
     if (seite == 0) {
       oled.setScale(2);
       oled.setCursor(0, 1);
       oled.print(temperatur); oled.print("C");
       oled.setCursor(0, 4);
       oled.print(luftfeuchte); oled.print("%");
       oled.setScale(1);
     } else {
       oled.setCursor(0, 1);
       oled.print("Druck: "); oled.print(luftdruck); oled.print(" hPa");
       oled.setCursor(0, 3);
       oled.print("Hoehe: "); oled.print(hoehe); oled.print(" m");
     }
     oled.update();

     // Achtung: Mit dem delay(1000) am Ende von loop() reagiert
     // der Taster traege. Verkleinere es auf delay(20) und miss
     // den Sensor nur jede Sekunde - mit millis():
     //
     //   static unsigned long letzteMessung = 0;
     //   if (millis() - letzteMessung > 1000) {
     //     letzteMessung = millis();
     //     ... hier messen ...
     //   }
     //
     // Das ist genau der Trick, mit dem ein Programm mehrere
     // Dinge "gleichzeitig" macht, ohne stehen zu bleiben.
   ------------------------------------------------------------- */
