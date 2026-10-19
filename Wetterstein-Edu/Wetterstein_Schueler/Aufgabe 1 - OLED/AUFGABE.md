# Aufgabe 1 – Das OLED-Display

**Zeit:** ca. 25 Minuten · **Datei zum Bearbeiten:** `src/main_OLED.cpp`

---

## Worum geht es?

Der Wetterstein soll später Temperatur und Luftfeuchtigkeit anzeigen.
Dafür brauchen wir zuerst ein Display – und du bringst es zum Leben.

Das kleine schwarze Display ist **128 Pixel breit** und **64 Pixel hoch**.
Es hängt an nur zwei Datenleitungen (**I2C**: SDA und SCL) am Mikrocontroller.
Über diese zwei Drähte schicken wir alles, was angezeigt werden soll.

---

## Was du wissen musst

**Ein Programm für den Mikrocontroller hat immer zwei Teile:**

| Teil | Wann läuft er? | Wofür? |
|---|---|---|
| `setup()` | genau **einmal** beim Einschalten | Sachen vorbereiten, z. B. Display anschalten |
| `loop()` | **immer wieder**, endlos | die eigentliche Arbeit |

**Der Punkt-Operator:** `oled` ist unser Display. Mit `oled.clear()` sagen wir
„Display, lösche dich!". Der Punkt bedeutet also: *Dieser Befehl gilt für dieses Ding.*

**Das Display ist wie eine Schreibmaschine:**

1. `oled.clear()` – Blatt leeren
2. `oled.setCursor(spalte, zeile)` – hinstellen
   *spalte* = 0…127 (Pixel von links), *zeile* = 0…7 (Textzeile von oben)
3. `oled.print("Text")` – schreiben
4. `oled.update()` – **abschicken**

> ⚠️ Ohne `oled.update()` bleibt das Display leer! Das ist der häufigste Fehler.

---

## Deine Aufgaben

### TODO 1.1 – Display startklar machen (in `setup()`)
Schalte das Display ein und lösche seinen Inhalt.

### TODO 1.2 – Text anzeigen (in `loop()`)
Zeige einen eigenen Text an, zum Beispiel deinen Namen.
In eine Zeile passen ungefähr **21 Zeichen**.

---

## So sieht es aus, wenn es klappt

Auf dem Display steht dein Text – dauerhaft und ohne Flackern.

---

## Extras für Schnelle ⭐

Die Extras stehen unten in der Code-Datei. Reihenfolge egal, such dir aus, was dich reizt:

- **A – Große Schrift:** `oled.setScale(2);` vor dem `print`. Was macht `setScale(4)`?
- **B – Zähler:** eine Variable, die bei jedem Durchlauf um 1 hochzählt.
- **C – Bild malen:** `rect()`, `line()`, `circle()`, `dot()` – zeichne ein Smiley oder ein Haus.
- **D – Laufschrift:** lass den Text von rechts nach links durchs Display wandern.

---

## Wenn etwas nicht klappt

| Problem | Wahrscheinliche Ursache |
|---|---|
| Display bleibt komplett dunkel | `oled.init()` fehlt, oder Kabel/Stecker sitzt nicht |
| Display leuchtet, aber kein Text | `oled.update()` vergessen |
| Text ist abgeschnitten | zu lang – kürzer schreiben oder auf zwei Zeilen verteilen |
| Fehler beim Kompilieren | fehlendes Semikolon `;` oder eine Klammer zu wenig – schau auf die **erste** rote Zeile |
