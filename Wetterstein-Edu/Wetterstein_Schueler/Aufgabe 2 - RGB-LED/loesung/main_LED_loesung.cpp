/* =============================================================
   LOESUNG zu Aufgabe 2 - RGB-LED

   Wird NICHT mitkompiliert. Nur die fehlenden Zeilen in
   src/main_LED.cpp uebertragen, nicht die ganze Datei kopieren.
   ============================================================= */

#include <Arduino.h>
#include <Wire.h>
#include <FastLED.h>

// --- Loesung 2.1 ---
#define LED_PIN 2
#define NUM_LEDS 1
CRGB wetterLeds[NUM_LEDS];
// -------------------


void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(100);

  FastLED.addLeds<WS2812, LED_PIN>(wetterLeds, NUM_LEDS);
  FastLED.setBrightness(80);
}


void loop() {
  // --- Loesung 2.2: Regenbogen ---
  wetterLeds[0] = CRGB::Red;
  FastLED.show();
  delay(500);

  wetterLeds[0] = CRGB::Orange;
  FastLED.show();
  delay(500);

  wetterLeds[0] = CRGB::Yellow;
  FastLED.show();
  delay(500);

  wetterLeds[0] = CRGB::Green;
  FastLED.show();
  delay(500);

  wetterLeds[0] = CRGB::Blue;
  FastLED.show();
  delay(500);

  wetterLeds[0] = CRGB::Purple;
  FastLED.show();
  delay(500);
  // -------------------------------
}


/* -------------------------------------------------------------
   LOESUNGSIDEEN ZU DEN EXTRAS

   EXTRA A - mit Liste und for-Schleife (ersetzt den ganzen loop)

     CRGB meineFarben[] = { CRGB::Red, CRGB::Orange, CRGB::Yellow,
                            CRGB::Green, CRGB::Blue, CRGB::Purple };
     int anzahlFarben = 6;

     void loop() {
       for (int i = 0; i < anzahlFarben; i++) {
         wetterLeds[0] = meineFarben[i];
         FastLED.show();
         delay(500);
       }
     }

   EXTRA B - weicher Regenbogen

     int farbton = 0;           // ueber setup() anlegen

     void loop() {
       wetterLeds[0] = CHSV(farbton, 255, 255);
       FastLED.show();
       farbton++;               // laeuft bei 255 automatisch auf 0 zurueck,
                                // wenn farbton ein uint8_t ist.
       if (farbton > 255) farbton = 0;
       delay(20);
     }

   EXTRA C - Atmen

     void loop() {
       wetterLeds[0] = CRGB::Aqua;

       for (int helligkeit = 0; helligkeit < 150; helligkeit++) {
         FastLED.setBrightness(helligkeit);
         FastLED.show();
         delay(10);
       }
       for (int helligkeit = 150; helligkeit > 0; helligkeit--) {
         FastLED.setBrightness(helligkeit);
         FastLED.show();
         delay(10);
       }
     }

   EXTRA D - Zufallsfarben

     void loop() {
       wetterLeds[0] = CRGB(random(0, 256), random(0, 256), random(0, 256));
       FastLED.show();
       delay(300);
     }
   ------------------------------------------------------------- */
