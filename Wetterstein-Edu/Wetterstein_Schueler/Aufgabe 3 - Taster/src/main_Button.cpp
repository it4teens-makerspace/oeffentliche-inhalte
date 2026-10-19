/* =============================================================
   IT4Teens - Wetterstein
   AUFGABE 3: Der Taster

   Ziel: Bei jedem Druck auf den Taster wechselt die LED die Farbe.

   So arbeitest du:
   Suche im Code nach "TODO". Nur dort musst du etwas schreiben.

   Zeit: ca. 30 Minuten
   ============================================================= */

#include <Arduino.h>
#include <Wire.h>
#include <FastLED.h>

// --- RGB-LED (kennst du schon aus Aufgabe 2) ---
#define LED_PIN 2
#define NUM_LEDS 1
CRGB wetterLeds[NUM_LEDS];

// --- Taster ---
#define BUTTON_PIN D3


// ===============================================================
// TODO 3.1: Wie viele Farben soll es geben?
//
// Trage hier ein, wie viele Faelle (cases) dein switch weiter
// unten haben wird. Bei 6 Farben (case 0 bis case 5) also die 6.
//
// Wichtig: Wenn du spaeter eine Farbe dazu tust, musst du diese
// Zahl mit erhoehen - sonst wird deine neue Farbe nie erreicht!

#define ANZAHL_FARBEN 6

// ===============================================================


// Merkt sich, welche Farbe gerade dran ist: 0, 1, 2, ...
int farbNummer = 0;


/* ---------------------------------------------------------------
   FERTIGE HILFSFUNKTION - hier musst du nichts aendern.

   Sie liefert true (wahr), wenn der Taster gerade frisch
   gedrueckt wurde. Sonst false (falsch).

   Warum so kompliziert? Wegen des PRELLENS:
   Im Taster sind zwei Metallteile. Beim Druecken springen die
   kurz wie ein Flummi hin und her - in wenigen Millisekunden
   also zigmal an und aus. Der Mikrocontroller ist so schnell,
   dass er jedes einzelne Zappeln als eigenen Druck zaehlen
   wuerde. Die LED wuerde wild durch alle Farben springen.

   Die Loesung heisst ENTPRELLEN: Nach der ersten Aenderung
   ignorieren wir 50 Millisekunden lang alles Weitere. Dann ist
   das Zappeln vorbei und wir haben genau einen sauberen Druck.

   (Das Wort "static" heisst: Diese Variable vergisst ihren Wert
    nicht, wenn die Funktion zu Ende ist. Sie merkt sich ihn bis
    zum naechsten Aufruf.)
   --------------------------------------------------------------- */
bool tasterWurdeGedrueckt() {
  static bool letzterMesswert   = HIGH;
  static bool tasterZustand     = HIGH;
  static unsigned long letzteAenderung = 0;
  const unsigned long entprellZeit = 50;   // Millisekunden

  bool messwert = digitalRead(BUTTON_PIN);

  // Hat sich etwas geaendert? Dann Stoppuhr neu starten.
  if (messwert != letzterMesswert) {
    letzteAenderung = millis();
  }
  letzterMesswert = messwert;

  // Ist es seit 50 ms ruhig geblieben? Dann zaehlt der Wert.
  if (millis() - letzteAenderung > entprellZeit) {
    if (messwert != tasterZustand) {
      tasterZustand = messwert;
      if (tasterZustand == LOW) {   // LOW = Taster ist gedrueckt
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

  // INPUT_PULLUP: Der Pin ist von selbst HIGH (an) und wird beim
  // Druecken auf LOW (aus) gezogen. Klingt verdreht, ist aber
  // bei Tastern der Normalfall - so braucht man keinen extra
  // Widerstand auf der Platine.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  FastLED.addLeds<WS2812, LED_PIN>(wetterLeds, NUM_LEDS);
  FastLED.setBrightness(80);
}


void loop() {
  // Taster abfragen und zur naechsten Farbe weiterschalten
  if (tasterWurdeGedrueckt()) {
    farbNummer = farbNummer + 1;

    if (farbNummer >= ANZAHL_FARBEN) {
      farbNummer = 0;              // wieder von vorne anfangen
    }

    Serial.print("Taster gedrueckt -> Farbe Nr. ");
    Serial.println(farbNummer);    // im Serial Monitor sichtbar
  }

  // =============================================================
  // TODO 3.2: Zu jeder Nummer die passende Farbe
  //
  // Ein switch ist eine Weiche: Je nachdem, welchen Wert
  // farbNummer gerade hat, wird genau ein Fall (case) ausgefuehrt.
  //
  // Die ersten beiden Faelle sind als Beispiel schon da.
  // Ergaenze die restlichen, bis du auf ANZAHL_FARBEN Faelle
  // kommst (also case 0 bis case 5).
  //
  // Das "break;" am Ende jedes Falls ist wichtig! Ohne break
  // laeuft das Programm einfach in den naechsten Fall weiter.

  switch (farbNummer) {

    case 0:
      wetterLeds[0] = CRGB::Red;
      break;

    case 1:
      wetterLeds[0] = CRGB::Orange;
      break;

    // ... hier kommen deine weiteren Faelle hin ...



  }

  // =============================================================

  FastLED.show();   // Farbe an die LED schicken
  delay(20);
}


/* =============================================================
   FERTIG? DANN WEITER MIT DEN EXTRAS

   EXTRA A - Aus-Zustand
   Baue einen Fall ein, in dem die LED aus ist:
     wetterLeds[0] = CRGB::Black;
   Denk daran, ANZAHL_FARBEN zu erhoehen.

   EXTRA B - Zaehler im Serial Monitor
   Zaehle mit, wie oft der Taster insgesamt gedrueckt wurde, und
   gib die Zahl aus. Du brauchst eine Variable ueber setup(),
   zum Beispiel  int anzahlDruecke = 0;
   Den Serial Monitor oeffnest du unten in VS Code mit dem
   Stecker-Symbol.

   EXTRA C - Helligkeit statt Farbe
   Steuere mit dem Taster nicht die Farbe, sondern die
   Helligkeit: Stufe 0 = aus, dann 20, 60, 150, 255.
   Befehl: FastLED.setBrightness(wert);

   EXTRA D - Langer Druck
   Wenn der Taster laenger als 1 Sekunde gehalten wird, soll die
   LED direkt wieder auf Farbe 0 springen.
   Tipp: merke dir mit millis(), WANN der Taster gedrueckt wurde,
   und pruefe, ob digitalRead(BUTTON_PIN) immer noch LOW ist.

   EXTRA E - Zwei Bausteine zusammen (nur mit Display)
   Schreibe zu jeder Farbe auch ihren Namen auf das OLED-Display.
   Dafuer brauchst du oben  #include <GyverOLED.h>  und die
   Zeile  GyverOLED<SSD1306_128x64> oled;  - und in setup()
   oled.init(). Den Rest kennst du aus Aufgabe 1.
   ============================================================= */
