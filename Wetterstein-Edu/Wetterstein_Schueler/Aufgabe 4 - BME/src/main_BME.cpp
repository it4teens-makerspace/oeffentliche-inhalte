/* =============================================================
   IT4Teens - Wetterstein
   AUFGABE 4: Der Wettersensor - alles zusammen

   Ziel: Der Sensor misst das Wetter, das Display zeigt die Werte
         und die LED verraet auf einen Blick, wie feucht es ist.

   Das ist die Koenigsaufgabe: Hier kommt alles zusammen, was du
   in den Aufgaben 1 bis 3 gelernt hast.

   Zeit: ca. 40 Minuten
   ============================================================= */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>   // der Wettersensor
#include <GyverOLED.h>         // das Display
#include <FastLED.h>           // die RGB-LED


// --- Wettersensor ---
Adafruit_BME280 bme;

// Luftdruck auf Meereshoehe. Den braucht der Sensor, um aus dem
// gemessenen Druck die Hoehe auszurechnen. 1013.25 hPa ist der
// weltweite Durchschnittswert.
#define SEALEVELPRESSURE_HPA (1013.25)

// --- Display ---
GyverOLED<SSD1306_128x64> oled;

// --- RGB-LED ---
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

  // Sensor starten. 0x76 ist seine Adresse auf der I2C-Leitung -
  // so eine Art Hausnummer. Meldet er sich nicht, hat das
  // Weitermachen keinen Sinn.
  if (!bme.begin(0x76)) {
    Serial.println("BME280 nicht gefunden! Stecker pruefen.");
    oled.clear();
    oled.setCursor(0, 3);
    oled.print("BME nicht gefunden");
    oled.update();
    while (true) {
      delay(1000);   // hier bleiben wir stehen
    }
  }

  FastLED.addLeds<WS2812, LED_PIN>(wetterLeds, NUM_LEDS);
  FastLED.setBrightness(80);
}


void loop() {
  // =============================================================
  // TODO 4.1: Den Sensor auslesen
  //
  // Unten stehen vier Variablen vom Typ float. float heisst
  // "Kommazahl" - die Temperatur ist ja selten genau 23 Grad,
  // sondern eher 23.4.
  //
  // Im Moment steht ueberall eine 0 drin. Ersetze jede 0 durch
  // den passenden Befehl:
  //
  //   temperatur    ->  bme.readTemperature()
  //   luftfeuchte   ->  bme.readHumidity()
  //   luftdruck     ->  bme.readPressure() / 100.0
  //   hoehe         ->  bme.readAltitude(SEALEVELPRESSURE_HPA)
  //
  // Warum "/ 100.0" beim Luftdruck?
  // Der Sensor liefert Pascal (etwa 101300). Im Wetterbericht
  // steht aber Hektopascal (etwa 1013). 1 hPa = 100 Pa - also
  // teilen wir durch 100.
  //
  // Tipp: Lade das Programm ruhig schon jetzt hoch und schau in
  // den Serial Monitor. Solange dort ueberall 0.00 steht, weisst
  // du: Diese Aufgabe ist noch offen.

  float temperatur  = 0;
  float luftfeuchte = 0;
  float luftdruck   = 0;
  float hoehe       = 0;

  // =============================================================


  // Zur Kontrolle alles in den Serial Monitor schreiben.
  // Praktisch zum Suchen von Fehlern - das ist schon fertig.
  Serial.print("Temp: ");   Serial.print(temperatur);
  Serial.print(" C   Feuchte: "); Serial.print(luftfeuchte);
  Serial.print(" %   Druck: ");   Serial.print(luftdruck);
  Serial.print(" hPa   Hoehe: "); Serial.print(hoehe);
  Serial.println(" m");


  // =============================================================
  // TODO 4.2: Die Werte auf das Display bringen
  //
  // Denk an die Reihenfolge aus Aufgabe 1:
  //   clear -> setCursor -> print -> ... -> update
  //
  // Nimm fuer jeden Wert eine eigene Zeile (0, 2, 4 und 6) und
  // schreibe dazu, was er bedeutet und welche Einheit er hat.
  //
  // Beispiel fuer die erste Zeile:
  //   oled.clear();
  //   oled.setCursor(0, 0);
  //   oled.print("Temp: ");
  //   oled.print(temperatur);
  //   oled.print(" C");
  //
  // Am Ende nicht das oled.update() vergessen!
  //
  // Einheiten: Temperatur C, Feuchte %, Druck hPa, Hoehe m




  // =============================================================


  // =============================================================
  // TODO 4.3: Die LED zeigt die Luftfeuchtigkeit
  //
  // Jetzt wird der Wetterstein wirklich nuetzlich: Die LED soll
  // die Farbe wechseln, je nachdem wie feucht die Luft ist.
  //
  // Dafuer brauchst du eine if / else if / else - Kette:
  //
  //   if (luftfeuchte < 30) {
  //     wetterLeds[0] = CRGB::Red;          // sehr trocken
  //   }
  //   else if (luftfeuchte < 50) {
  //     wetterLeds[0] = CRGB::Gold;         // trocken
  //   }
  //   else if ...
  //   else {
  //     wetterLeds[0] = CRGB::Blue;         // sehr feucht
  //   }
  //
  // Wichtig: Der Computer prueft von oben nach unten und nimmt
  // den ERSTEN Fall, der zutrifft. Deshalb reicht immer nur die
  // obere Grenze - "und groesser als 30" musst du nicht
  // dazuschreiben, das ergibt sich von selbst.
  //
  // Am Schluss brauchst du ein else ohne Bedingung. Das faengt
  // alles ab, was uebrig bleibt - so bleibt die LED nie dunkel,
  // egal was der Sensor misst.
  //
  // Nimm mindestens 4 Stufen.




  // =============================================================

  FastLED.show();

  delay(1000);   // einmal pro Sekunde messen reicht voellig
}


/* =============================================================
   FERTIG? DANN WEITER MIT DEN EXTRAS

   EXTRA A - Hauch mal drauf
   Atme vorsichtig aus etwas Abstand Richtung Sensor und schau
   dem Display zu. Wie schnell reagiert die Feuchte? Wie lange
   dauert es, bis sie wieder auf dem alten Wert ist?
   Fasse den Sensor dabei nicht direkt an.

   EXTRA B - Stufenlose Farbe (die Koenigsdisziplin)
   Statt fester Stufen soll die Farbe fliessend mit der Feuchte
   wandern - von Rot (trocken) ueber Gruen bis Blau (feucht).

   Dafuer gibt es CHSV: Farbton, Saettigung, Helligkeit.
   Der Farbton ist eine Zahl von 0 bis 255 auf einem Farbkreis:
     0 = Rot, 64 = Gelbgruen, 96 = Gruen, 160 = Blau

   map() rechnet einen Bereich in einen anderen um:
     int farbton = map(luftfeuchte, 0, 100, 0, 160);
     wetterLeds[0] = CHSV(farbton, 255, 255);

   Ersetze damit deine if-Kette aus 4.3.
   Zum Ausprobieren: Was passiert bei map(..., 0, 100, 160, 0)?

   EXTRA C - Hoechst- und Tiefstwert
   Merke dir die hoechste und niedrigste Temperatur, seit das
   Geraet laeuft, und zeige beide unten auf dem Display an.
   Tipp: zwei Variablen ueber setup(), und bei jeder Messung
     if (temperatur > maxTemp) { maxTemp = temperatur; }

   EXTRA D - Zwei Seiten mit dem Taster
   Kombiniere Aufgabe 3 und 4: Mit dem Taster (Pin D3) schaltest
   du zwischen zwei Display-Seiten um -
   Seite 0: Temperatur und Feuchte gross (oled.setScale(2))
   Seite 1: Druck und Hoehe
   Die fertige Funktion tasterWurdeGedrueckt() kannst du dir
   aus Aufgabe 3 herueberkopieren.

   EXTRA E - Denk mal nach
   Der Sensor zeigt eine Hoehe an. Die stimmt aber selten.
   Warum? (Antwort: Er misst gar keine Hoehe, sondern rechnet sie
   aus dem Luftdruck aus - und der aendert sich auch mit dem
   Wetter. Bei einem Sturmtief "waechst" dein Zimmer um ueber
   100 Meter, ohne dass sich etwas bewegt hat.)
   ============================================================= */
