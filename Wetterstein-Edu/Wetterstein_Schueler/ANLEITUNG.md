# Der Wetterstein – Programmierteil

**IT4Teens · Dauer: ca. 2,5 Stunden · Für alle ohne Vorkenntnisse**

---

## Was baust du hier?

Am Ende des Tages hältst du eine **eigene kleine Wetterstation** in der Hand: Sie misst
Temperatur, Luftfeuchtigkeit und Luftdruck, zeigt alles auf einem Display an und
verrät dir mit einer farbigen LED auf einen Blick, wie das Wetter ist.

Das Ganze steuert ein **Mikrocontroller** – ein winziger Computer für rund 5 Euro,
der keinen Bildschirm und keine Tastatur hat, dafür aber Sensoren, Lampen und Motoren
direkt ansteuern kann. Solche Chips stecken in deiner Waschmaschine, in Ampeln, im
Auto und in fast jedem Gerät, das irgendetwas „von selbst" macht.

Du gehst in **vier Schritten** vor. Jeder Schritt baut auf dem vorherigen auf:

| # | Aufgabe | Was du lernst | Zeit |
|---|---|---|---|
| 1 | **OLED-Display** | Etwas ausgeben – der Computer redet mit dir | 25 min |
| 2 | **RGB-LED** | Etwas ansteuern – der Computer bewegt die Welt | 25 min |
| 3 | **Taster** | Etwas einlesen – die Welt redet mit dem Computer | 30 min |
| 4 | **Wettersensor** | Alles zusammen – der fertige Wetterstein | 40 min |

Dazu kommen etwa 15 Minuten Einstieg und 15 Minuten Puffer.

> **Wo ist das WLAN?** Der Wetterstein kann seine Daten später auch ins Netz schicken.
> Dieser Teil ist ein eigener Baustein des Gesamtprojekts und hat sein eigenes
> Repository – er ist hier bewusst nicht dabei, sonst wird der Tag zu lang.
> → Link: https://github.com/it4teens-makerspace/oeffentliche-inhalte/tree/main/Wetterstein-Edu/Softwarepaket

---

## Wo finde ich was?

**Die Aufgabenblätter** – hier klicken, um die jeweilige Aufgabe zu öffnen:

| # | Aufgabenblatt (hier klicken) | Deine Arbeitsdatei in VS Code |
|---|---|---|
| 1 | [Aufgabe 1 – OLED-Display](Aufgabe%201%20-%20OLED/AUFGABE.md) | `Aufgabe 1 - OLED/src/main_OLED.cpp` |
| 2 | [Aufgabe 2 – RGB-LED](Aufgabe%202%20-%20RGB-LED/AUFGABE.md) | `Aufgabe 2 - RGB-LED/src/main_LED.cpp` |
| 3 | [Aufgabe 3 – Taster](Aufgabe%203%20-%20Taster/AUFGABE.md) | `Aufgabe 3 - Taster/src/main_Button.cpp` |
| 4 | [Aufgabe 4 – Wettersensor](Aufgabe%204%20-%20BME/AUFGABE.md) | `Aufgabe 4 - BME/src/main_BME.cpp` |

Auf GitHub geht es auch ohne die Links: Klicke auf den Ordner der Aufgabe, zum Beispiel
**`Aufgabe 1 - OLED`**, und darin auf die Datei **`AUFGABE.md`**.

**Die ganze Anleitung als PDF** – zum Lesen oder Ausdrucken:

> 📄 [00 Gesamtanleitung fuer Schueler.pdf](Word-Dokumente/00%20Gesamtanleitung%20fuer%20Schueler.pdf)
>
> Sie liegt im Ordner **`Word-Dokumente`**. Auf GitHub: Ordner `Word-Dokumente` anklicken
> → `00 Gesamtanleitung fuer Schueler.pdf` anklicken → oben rechts auf den
> **Download-Knopf** (Pfeil nach unten) klicken, um sie zu speichern.

---

## Was du brauchst

**Hardware**

- NodeMCU v2 (ESP8266) – das Board mit dem Mikrocontroller
- OLED-Display 128 × 64 (SSD1306)
- Wettersensor BME280
- RGB-LED (WS2812)
- Taster
- Micro-USB-Kabel

**Software auf dem Rechner**

- Visual Studio Code
- Erweiterung **PlatformIO IDE** (in VS Code links im Extensions-Menü suchen und installieren)

---

## Die Verkabelung

| Bauteil | Anschluss am Board |
|---|---|
| OLED-Display | SDA → **D2**, SCL → **D1**, VCC → 3V3, GND → GND |
| Wettersensor BME280 | SDA → **D2**, SCL → **D1**, VCC → 3V3, GND → GND |
| RGB-LED (Data) | **D4** |
| Taster | **D3** und GND |

Display und Sensor hängen an **denselben zwei Leitungen**. Das geht, weil sie sich den
sogenannten **I2C-Bus** teilen: Jedes Bauteil hat eine eigene Adresse – eine Art
Hausnummer –, und der Mikrocontroller spricht immer nur eines davon an. Der Sensor
hört auf `0x76`, das Display auf `0x3C`.

> ℹ️ Diese Tabelle ergibt sich aus dem Code. Wenn euer Bausatz anders verkabelt ist,
> gilt natürlich euer Aufbau – dann müssen im Code die Pin-Nummern angepasst werden.

---

## Erste Schritte

### 1. Das richtige Projekt öffnen

> ⚠️ **Das ist die wichtigste Regel des Tages.** Öffne in VS Code immer **genau einen
> Aufgabenordner**, zum Beispiel `Aufgabe 1 - OLED` – **nicht** den Ordner darüber.
> PlatformIO erkennt ein Projekt nur an der Datei `platformio.ini`, und die liegt in
> jedem Aufgabenordner einzeln.

*Datei → Ordner öffnen… → `Aufgabe 1 - OLED` → Öffnen*

Beim allerersten Mal lädt PlatformIO im Hintergrund einige Pakete herunter. Das kann
ein paar Minuten dauern – unten rechts läuft ein Fortschrittsbalken. Einfach warten.

### 2. Die drei Knöpfe, die du brauchst

Ganz unten in VS Code läuft eine blaue Leiste. Dort sind sie:

| Symbol | Name | Was es tut | Tastenkürzel |
|---|---|---|---|
| ✔ Häkchen | **Build** | Übersetzt deinen Code. Findet Tippfehler – **ohne** ihn aufs Board zu laden. | `Strg` + `Alt` + `B` |
| → Pfeil | **Upload** | Übersetzt **und** überträgt aufs Board. | `Strg` + `Alt` + `U` |
| 🔌 Stecker | **Serial Monitor** | Zeigt Texte, die das Board zurückschickt. | – |

**Faustregel:** Nach jeder kleinen Änderung einmal **Build** drücken. So findest du
einen Fehler sofort und weißt genau, welche Zeile schuld ist. Wenn du eine Stunde lang
schreibst und dann erst baust, hast du zwanzig Fehler auf einmal.

### 3. Loslegen

Öffne im Ordner `src` die Datei mit dem Programm und suche nach **`TODO`**.
Nur dort musst du etwas schreiben. Alles andere ist schon fertig.

Zu jeder Aufgabe gibt es außerdem ein Aufgabenblatt: **`AUFGABE.md`** im jeweiligen
Ordner. Dort steht ausführlich, worum es geht. Die Links zu allen vier Blättern findest
du oben unter [Wo finde ich was?](#wo-finde-ich-was).

---

## Das musst du über Code wissen

### Jedes Programm hat zwei Teile

```cpp
void setup() {
  // Läuft GENAU EINMAL, direkt nach dem Einschalten.
  // Hier wird alles vorbereitet.
}

void loop() {
  // Läuft danach IMMER WIEDER, endlos schnell,
  // bis der Strom weg ist. Hier passiert die Arbeit.
}
```

Ein Mikrocontroller ist nie „fertig". Er wiederholt `loop()` viele tausend Mal pro
Sekunde – und genau daraus entsteht alles, was das Gerät tut.

### Die vier Regeln, an denen alle zuerst scheitern

1. **Jede Anweisung endet mit einem Semikolon `;`**
   Vergisst du es, meckert der Compiler meist in der **nächsten** Zeile. Schau also
   auch immer eine Zeile darüber.
2. **Jede geöffnete Klammer `{` braucht eine schließende `}`**
   VS Code hilft: Klick auf eine Klammer, dann leuchtet die Partnerklammer mit auf.
3. **Groß- und Kleinschreibung zählt.**
   `FastLED` ist etwas anderes als `fastled`. Der Computer ist da gnadenlos.
4. **Text in Anführungszeichen, Zahlen ohne.**
   `oled.print("Hallo");` aber `oled.print(23);`

### Kommentare

```cpp
// Alles hinter zwei Schrägstrichen ist eine Notiz für Menschen.
/* Das geht auch
   über mehrere Zeilen. */
```

Der Computer überliest das komplett. Kommentare sind für dich und für die, die deinen
Code später lesen – auch für dich selbst in drei Wochen.

---

## Wenn etwas nicht klappt

**Fehler beim Übersetzen (rote Meldungen)**

Lies die **erste** rote Zeile. Dort stehen Dateiname, Zeilennummer und Problem.
Alles darunter sind meistens nur Folgefehler. Häufig:

| Meldung | Bedeutet |
|---|---|
| `expected ';' before ...` | Semikolon fehlt – meist eine Zeile **darüber** |
| `'xyz' was not declared in this scope` | Der Name ist unbekannt: vertippt, oder die Variable wurde noch nicht angelegt |
| `expected '}' at end of input` | Eine schließende Klammer fehlt |

**Upload schlägt fehl**

- Steckt das USB-Kabel richtig? Manche Kabel können nur laden, nicht Daten übertragen –
  probiere ein anderes.
- Ist der Serial Monitor noch offen? Der belegt den Anschluss. Erst schließen, dann hochladen.
- Board kurz ab- und wieder anstecken, dann erneut probieren.

**Das Programm lädt, aber es passiert nichts**

Das ist selten ein Defekt. Fast immer fehlt einer der beiden „Abschicken"-Befehle:

- Display: `oled.update()`
- LED: `FastLED.show()`

Ohne die bleibt alles nur im Speicher – und damit unsichtbar.

---

## Spickzettel

```cpp
// --- Display ---
oled.clear();                 // Bild löschen
oled.setCursor(spalte, zeile);// Position: spalte 0-127, zeile 0-7
oled.print("Text");           // schreiben
oled.print(zahl);             // Zahl schreiben
oled.setScale(2);             // Schriftgröße 1 bis 4
oled.update();                // ANZEIGEN – nicht vergessen!

// --- RGB-LED ---
wetterLeds[0] = CRGB::Red;    // Farbe wählen
wetterLeds[0] = CRGB(r, g, b);// eigene Mischung, je 0-255
FastLED.setBrightness(80);    // Helligkeit 0-255
FastLED.show();               // ANZEIGEN – nicht vergessen!

// --- Taster ---
digitalRead(BUTTON_PIN)       // LOW = gedrückt

// --- Sensor ---
bme.readTemperature();        // Grad Celsius
bme.readHumidity();           // Prozent
bme.readPressure() / 100.0;   // Hektopascal

// --- Allgemein ---
delay(500);                   // 500 ms warten
millis();                     // Millisekunden seit dem Start
Serial.println("Hallo");      // Text an den Computer
```

---

## Und wenn ich früher fertig bin?

Kein Problem – **jede Aufgabe hat Extras**. Sie stehen unten in der Code-Datei und im
Aufgabenblatt und sind mit ⭐ markiert. Such dir aus, was dich interessiert; die
Reihenfolge ist egal, und niemand muss alle machen.

Die Extras sind keine Beschäftigungstherapie: In Aufgabe 2 lernst du, wie man
zwanzig fast gleiche Zeilen zu vier zusammenschrumpft. In Aufgabe 4 baust du einen
stufenlosen Farbverlauf. Das ist echtes Handwerk.

---

## Und wenn ich hängenbleibe?

1. Lies die Fehlermeldung – ernsthaft, die erste Zeile.
2. Schau ins Aufgabenblatt `AUFGABE.md`, ganz unten steht eine Fehlertabelle.
3. Frag die Person neben dir. Fremden Code zu lesen ist die beste Übung.
4. Frag die Betreuerin oder den Betreuer.
5. Im Ordner `loesung` liegt die Musterlösung. Sie ist kein Betrug – schau hinein,
   **verstehe** die Zeile, die dir fehlte, und tippe sie selbst ab.

> ⚠️ Kopiere nicht die ganze Lösungsdatei in den `src`-Ordner. Dann gibt es `setup()`
> und `loop()` doppelt und nichts geht mehr. Übertrage nur die Zeilen, die dir fehlen.
