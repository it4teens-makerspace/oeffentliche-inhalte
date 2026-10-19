/* =============================================================
   IT4Teens - Wetterstein
   AUFGABE 2: Die RGB-LED

   Ziel: Deine LED leuchtet nacheinander in verschiedenen Farben.

   So arbeitest du:
   Suche im Code nach "TODO". Nur dort musst du etwas schreiben.

   Zeit: ca. 25 Minuten

   HINWEIS: Dieses Programm laesst sich erst uebersetzen, wenn du
   TODO 2.1 erledigt hast. Klickst du vorher auf Build, kommt:

       'wetterLeds' was not declared in this scope

   Das ist KEIN Defekt, sondern genau richtig. Uebersetzt heisst
   es: "Du benutzt weiter unten etwas namens wetterLeds, aber du
   hast mir nie gesagt, was das sein soll." Genau das holst du in
   TODO 2.1 nach.

   Merk dir das Vorgehen: Bei einer Fehlermeldung immer die
   ERSTE rote Zeile lesen. Dort steht die Datei, die Zeilennummer
   und das eigentliche Problem. Alles darunter sind oft nur
   Folgefehler.
   ============================================================= */

#include <Arduino.h>
#include <Wire.h>
#include <FastLED.h>   // Bibliothek fuer die RGB-LED


// ===============================================================
// TODO 2.1: Die LED anmelden
//
// Der Mikrocontroller muss zwei Dinge wissen:
//   - An welchem Pin haengt die LED?  -> Pin 2
//   - Wie viele LEDs sind es?         -> 1
//
// Schreibe genau diese drei Zeilen unter den Kommentar:
//
//   #define LED_PIN 2
//   #define NUM_LEDS 1
//   CRGB wetterLeds[NUM_LEDS];
//
// Was bedeutet das?
//   #define gibt einer Zahl einen Namen. Ueberall, wo spaeter
//   LED_PIN steht, setzt der Computer die 2 ein. Das ist
//   praktisch: steckst du die LED um, aenderst du nur EINE Zeile.
//
//   CRGB wetterLeds[NUM_LEDS]; legt einen Platz fuer die Farbe
//   jeder LED an. Die eckigen Klammern [ ] bedeuten "Liste".
//   Die erste LED der Liste heisst wetterLeds[0]  -  ja, Null!
//   Programmierer fangen beim Zaehlen fast immer bei 0 an.




// ===============================================================


void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(100);

  // LED anmelden - das ist schon fertig
  FastLED.addLeds<WS2812, LED_PIN>(wetterLeds, NUM_LEDS);
  FastLED.setBrightness(80);   // 0 = aus, 255 = maximal.
                               // 80 reicht dicke und blendet nicht.
}


void loop() {
  // =============================================================
  // TODO 2.2: Farben nacheinander anzeigen
  //
  // Eine Farbe zu setzen geht immer in drei Schritten:
  //
  //   wetterLeds[0] = CRGB::Red;   // Farbe aussuchen
  //   FastLED.show();              // an die LED schicken
  //   delay(500);                  // 500 ms warten (= halbe Sekunde)
  //
  // Das :: heisst Scope-Operator. Er bedeutet:
  // "nimm die Farbe Red aus der Sammlung CRGB".
  //
  // Baue eine Abfolge aus mindestens 5 Farben - zum Beispiel
  // einen Regenbogen. Achte darauf, dass FastLED.show() nach
  // JEDER Farbe kommt. Ohne show() passiert naemlich nichts.
  //
  // Farben zum Aussuchen:
  //   Red        Orange      Gold        Yellow
  //   Green      SpringGreen Aqua        Cyan
  //   Blue       RoyalBlue   Purple      Magenta
  //   Pink       White       Amethyst    Black (= aus)
  //
  // (Es gibt noch ueber 100 weitere. Tippe CRGB:: und warte kurz -
  //  VS Code schlaegt dir alle vor.)




  // =============================================================
}


/* =============================================================
   FERTIG? DANN WEITER MIT DEN EXTRAS

   EXTRA A - Kuerzer statt laenger
   Dein Code hat jetzt viele fast gleiche Zeilen. Das geht kuerzer!
   Lege dir eine Liste von Farben an und laufe mit einer
   for-Schleife durch:

     CRGB meineFarben[] = { CRGB::Red, CRGB::Gold, CRGB::Green,
                            CRGB::Blue, CRGB::Purple };

     for (int i = 0; i < 5; i++) {
       wetterLeds[0] = meineFarben[i];
       FastLED.show();
       delay(500);
     }

   Baue das ein. Weniger Code, gleiches Ergebnis - das nennt man
   guten Code.

   EXTRA B - Weicher Regenbogen ohne Ruckeln
   Statt fester Farben kannst du den Farbton stufenlos drehen.
   CHSV steht fuer Farbton (Hue), Saettigung, Helligkeit:

     // ueber setup():  int farbton = 0;
     wetterLeds[0] = CHSV(farbton, 255, 255);
     FastLED.show();
     farbton = farbton + 1;     // 0 bis 255, dann faengt es von vorn an
     delay(20);

   EXTRA C - Atmen
   Lass die LED langsam heller und wieder dunkler werden.
   Tipp: FastLED.setBrightness(wert); in einer for-Schleife,
   wert laeuft von 0 bis 150 und wieder zurueck.
   Nach jedem setBrightness brauchst du wieder FastLED.show().

   EXTRA D - Zufall
   random(0, 256) liefert eine Zufallszahl von 0 bis 255.
   Damit kannst du eine zufaellige Farbe bauen:
     wetterLeds[0] = CRGB(random(0,256), random(0,256), random(0,256));
   Die drei Zahlen sind Rot, Gruen und Blau. Aus diesen drei
   Grundfarben mischt die LED jede andere Farbe zusammen.
   ============================================================= */
