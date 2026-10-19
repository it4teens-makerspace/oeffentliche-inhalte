/* =============================================================
   IT4Teens - Wetterstein
   AUFGABE 1: Das OLED-Display

   Ziel: Du bringst deinen eigenen Text auf das kleine Display.

   So arbeitest du:
   Suche im Code nach "TODO". Nur dort musst du etwas schreiben.
   Der Rest ist schon fertig und bleibt so, wie er ist.

   Zeit: ca. 25 Minuten
   ============================================================= */

#include <Arduino.h>    // Grundbefehle fuer den Mikrocontroller
#include <Wire.h>       // I2C - die zwei Datenleitungen zum Display
#include <GyverOLED.h>  // Die Bibliothek, die das Display steuert


// Hier entsteht unser Display. Wir geben ihm den Namen "oled".
// Ueber den Punkt-Operator (also oled.befehl) sagen wir ihm, was es tun soll.
GyverOLED<SSD1306_128x64> oled;


// -------------------------------------------------------------
void setup() {
  // setup() laeuft genau EINMAL - direkt nach dem Einschalten.

  Serial.begin(115200);  // Textausgabe zum Computer starten
  Wire.begin();          // I2C-Verbindung starten
  delay(100);

  // ===========================================================
  // TODO 1.1: Display startklar machen
  //
  // Schreibe unter diesen Kommentar zwei Zeilen:
  //
  //   oled.init();    <- schaltet das Display ein
  //   oled.clear();   <- loescht alles, was noch drauf steht
  //
  // Merke dir: Jede Anweisung endet mit einem Semikolon  ;




  // ===========================================================

  delay(1000);
}


// -------------------------------------------------------------
void loop() {
  // loop() wird immer und immer wieder ausgefuehrt - bis der Strom weg ist.

  // ===========================================================
  // TODO 1.2: Text anzeigen
  //
  // Dafuer brauchst du vier Befehle. Das Display arbeitet wie eine
  // Schreibmaschine: erst hinstellen, dann schreiben, dann abschicken.
  //
  //   oled.clear();               loescht das Bild
  //   oled.setCursor(spalte, zeile);   waehlt die Startposition
  //                               spalte: 0 bis 127 (Pixel von links)
  //                               zeile:  0 bis 7   (Textzeile von oben)
  //   oled.print("Text");         legt den Text bereit
  //   oled.update();              zeigt alles wirklich an  <- nicht vergessen!
  //
  // Beispiel fuer eine Zeile:
  //   oled.setCursor(0, 3);
  //   oled.print("Hallo Welt!");
  //
  // Schreibe deinen eigenen Text, zum Beispiel deinen Namen.
  // In eine Zeile passen ungefaehr 21 Zeichen.




  // ===========================================================

  delay(500);
}


/* =============================================================
   FERTIG? DANN WEITER MIT DEN EXTRAS  (Reihenfolge egal)

   EXTRA A - Grosse Schrift
   Setze VOR dem print den Befehl  oled.setScale(2);
   Erlaubt sind die Werte 1 bis 4. Was passiert bei 4?
   Tipp: Mit setScale(1) schaltest du wieder auf klein zurueck.

   EXTRA B - Ein Zaehler
   Lege ueber setup() eine Variable an:   int zaehler = 0;
   Erhoehe sie in loop() mit             zaehler = zaehler + 1;
   und gib sie aus mit                   oled.print(zaehler);
   Warum zaehlt sie schneller, wenn du das delay(500) verkleinerst?

   EXTRA C - Male ein Bild
   Diese Befehle zeichnen direkt auf das Display:
     oled.rect(x0, y0, x1, y1);      Rechteck
     oled.line(x0, y0, x1, y1);      Linie
     oled.circle(x, y, radius);      Kreis
     oled.dot(x, y);                 einzelner Punkt
   Die Flaeche ist 128 Pixel breit und 64 Pixel hoch.
   Zeichne ein Smiley, ein Haus oder das Logo deines Vereins.

   EXTRA D - Laufschrift
   Lass deinen Text von rechts nach links wandern, indem du die
   Spalte bei jedem Durchlauf um 1 verringerst. Was passiert,
   wenn die Spalte kleiner als 0 wird? Faengt der Text wieder
   rechts an?
   ============================================================= */
