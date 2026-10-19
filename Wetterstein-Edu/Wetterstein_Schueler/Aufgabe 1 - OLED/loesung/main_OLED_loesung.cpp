/* =============================================================
   LOESUNG zu Aufgabe 1 - OLED-Display

   Diese Datei wird NICHT mitkompiliert (sie liegt nicht in src/).
   Wer nicht weiterkommt: den Inhalt lesen und die fehlenden
   Zeilen in src/main_OLED.cpp uebertragen - nicht die ganze
   Datei kopieren, sonst gibt es setup() und loop() doppelt.
   ============================================================= */

#include <Arduino.h>
#include <Wire.h>
#include <GyverOLED.h>

GyverOLED<SSD1306_128x64> oled;


void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(100);

  // --- Loesung 1.1 ---
  oled.init();
  oled.clear();
  // -------------------

  delay(1000);
}


void loop() {
  // --- Loesung 1.2 ---
  oled.clear();               // altes Bild loeschen

  oled.setCursor(0, 2);
  oled.print("Hallo Welt!");

  oled.setCursor(0, 4);
  oled.print("Ich bin der Stein");

  oled.update();              // erst jetzt wird es sichtbar
  // -------------------

  delay(500);
}


/* -------------------------------------------------------------
   LOESUNGSIDEEN ZU DEN EXTRAS

   EXTRA A - Grosse Schrift
     oled.clear();
     oled.setScale(2);
     oled.setCursor(0, 2);
     oled.print("GROSS");
     oled.setScale(1);        // wieder klein fuer den Rest
     oled.update();

   EXTRA B - Zaehler
     // ueber setup():  int zaehler = 0;
     zaehler = zaehler + 1;
     oled.clear();
     oled.setCursor(0, 3);
     oled.print("Zaehler: ");
     oled.print(zaehler);
     oled.update();

   EXTRA C - Bild malen (Smiley)
     // Achtung: rect() und circle() zeichnen standardmaessig
     // GEFUELLT. Fuer nur den Umriss OLED_STROKE angeben, sonst
     // sind Augen und Mund weiss auf weiss unsichtbar.
     oled.clear();
     oled.circle(64, 32, 25, OLED_STROKE);  // Kopf (nur Umriss)
     oled.circle(55, 24, 3);                // linkes Auge
     oled.circle(73, 24, 3);                // rechtes Auge
     oled.line(52, 38, 58, 44);             // Mund
     oled.line(58, 44, 70, 44);
     oled.line(70, 44, 76, 38);
     oled.update();

   EXTRA D - Laufschrift
     // ueber setup():  int spalte = 127;
     oled.clear();
     oled.setCursor(spalte, 3);
     oled.print("Wetterstein");
     oled.update();

     spalte = spalte - 2;
     if (spalte < -80) {        // Text ist links rausgelaufen
       spalte = 127;            // wieder rechts anfangen
     }
     delay(50);
   ------------------------------------------------------------- */
