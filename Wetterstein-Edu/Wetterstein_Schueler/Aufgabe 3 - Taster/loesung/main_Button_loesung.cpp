/* =============================================================
   LOESUNG zu Aufgabe 3 - Taster

   Wird NICHT mitkompiliert. Nur die fehlenden Zeilen in
   src/main_Button.cpp uebertragen.
   ============================================================= */

#include <Arduino.h>
#include <Wire.h>
#include <FastLED.h>

#define LED_PIN 2
#define NUM_LEDS 1
CRGB wetterLeds[NUM_LEDS];

#define BUTTON_PIN D3

// --- Loesung 3.1 ---
#define ANZAHL_FARBEN 6
// -------------------

int farbNummer = 0;


bool tasterWurdeGedrueckt() {
  static bool letzterMesswert   = HIGH;
  static bool tasterZustand     = HIGH;
  static unsigned long letzteAenderung = 0;
  const unsigned long entprellZeit = 50;

  bool messwert = digitalRead(BUTTON_PIN);

  if (messwert != letzterMesswert) {
    letzteAenderung = millis();
  }
  letzterMesswert = messwert;

  if (millis() - letzteAenderung > entprellZeit) {
    if (messwert != tasterZustand) {
      tasterZustand = messwert;
      if (tasterZustand == LOW) {
        return true;
      }
    }
  }
  return false;
}


void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(100);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  FastLED.addLeds<WS2812, LED_PIN>(wetterLeds, NUM_LEDS);
  FastLED.setBrightness(80);
}


void loop() {
  if (tasterWurdeGedrueckt()) {
    farbNummer = farbNummer + 1;
    if (farbNummer >= ANZAHL_FARBEN) {
      farbNummer = 0;
    }
    Serial.print("Taster gedrueckt -> Farbe Nr. ");
    Serial.println(farbNummer);
  }

  // --- Loesung 3.2 ---
  switch (farbNummer) {

    case 0:
      wetterLeds[0] = CRGB::Red;
      break;

    case 1:
      wetterLeds[0] = CRGB::Orange;
      break;

    case 2:
      wetterLeds[0] = CRGB::Yellow;
      break;

    case 3:
      wetterLeds[0] = CRGB::Green;
      break;

    case 4:
      wetterLeds[0] = CRGB::Blue;
      break;

    case 5:
      wetterLeds[0] = CRGB::Purple;
      break;
  }
  // -------------------

  FastLED.show();
  delay(20);
}


/* -------------------------------------------------------------
   LOESUNGSIDEEN ZU DEN EXTRAS

   EXTRA A - Aus-Zustand
     #define ANZAHL_FARBEN 7      // eine mehr!
     ...
     case 6:
       wetterLeds[0] = CRGB::Black;
       break;

   EXTRA B - Zaehler
     int anzahlDruecke = 0;       // ueber setup() anlegen

     if (tasterWurdeGedrueckt()) {
       anzahlDruecke++;
       Serial.print("Insgesamt gedrueckt: ");
       Serial.println(anzahlDruecke);
       ...
     }

   EXTRA C - Helligkeitsstufen
     #define ANZAHL_STUFEN 5
     int stufe = 0;

     void loop() {
       if (tasterWurdeGedrueckt()) {
         stufe++;
         if (stufe >= ANZAHL_STUFEN) stufe = 0;
       }

       switch (stufe) {
         case 0: FastLED.setBrightness(0);   break;
         case 1: FastLED.setBrightness(20);  break;
         case 2: FastLED.setBrightness(60);  break;
         case 3: FastLED.setBrightness(150); break;
         case 4: FastLED.setBrightness(255); break;
       }

       wetterLeds[0] = CRGB::Aqua;
       FastLED.show();
       delay(20);
     }

   EXTRA D - Langer Druck
     static unsigned long gedruecktSeit = 0;
     static bool langErkannt = false;

     if (digitalRead(BUTTON_PIN) == LOW) {
       if (gedruecktSeit == 0) {
         gedruecktSeit = millis();
       }
       if (!langErkannt && millis() - gedruecktSeit > 1000) {
         farbNummer = 0;          // zurueck auf Anfang
         langErkannt = true;
       }
     } else {
       gedruecktSeit = 0;
       langErkannt = false;
     }

   EXTRA E - Farbnamen auf dem Display
     #include <GyverOLED.h>
     GyverOLED<SSD1306_128x64> oled;

     // in setup():   oled.init();  oled.clear();

     // im switch zusaetzlich zur Farbe:
     case 0:
       wetterLeds[0] = CRGB::Red;
       oled.clear();
       oled.setCursor(0, 3);
       oled.print("Rot");
       oled.update();
       break;

     // Noch eleganter: eine Liste mit Namen anlegen und den
     // richtigen ueber farbNummer herausgreifen -
     // dann braucht man die Display-Zeilen nur EINMAL:
     //   const char* namen[] = {"Rot","Orange","Gelb",
     //                          "Gruen","Blau","Lila"};
     //   oled.print(namen[farbNummer]);
   ------------------------------------------------------------- */
