# Aufgabe 2 – Die RGB-LED

**Zeit:** ca. 25 Minuten · **Datei zum Bearbeiten:** `src/main_LED.cpp`

---

## Worum geht es?

Der Wetterstein soll schon von weitem zeigen, wie das Wetter ist – ohne dass man
das Display lesen muss. Dafür sorgt eine **RGB-LED**.

In dieser einen LED stecken in Wirklichkeit **drei winzige Lämpchen**: ein rotes,
ein grünes und ein blaues. Jedes davon kann in 256 Helligkeitsstufen leuchten
(0 = aus, 255 = volle Kraft). Zusammen ergibt das über
**16 Millionen Farben** – genau wie bei deinem Handybildschirm.

| Rot | Grün | Blau | Ergebnis |
|---|---|---|---|
| 255 | 0 | 0 | Rot |
| 255 | 255 | 0 | Gelb |
| 0 | 255 | 255 | Türkis |
| 255 | 255 | 255 | Weiß |
| 0 | 0 | 0 | aus |

---

## Was du wissen musst

**`#define` gibt einer Zahl einen Namen:**

```cpp
#define LED_PIN 2
```

Überall, wo später `LED_PIN` steht, setzt der Computer beim Übersetzen die `2` ein.
Vorteil: Steckst du die LED an einen anderen Pin, änderst du nur **eine einzige Zeile**.

**Die eckigen Klammern `[ ]` bedeuten „Liste":**

```cpp
CRGB wetterLeds[NUM_LEDS];
```

Das ist ein Platz für die Farbe jeder LED. Die **erste** LED heißt `wetterLeds[0]` –
ja, mit Null. Programmierer fangen beim Zählen fast immer bei 0 an. Daran gewöhnst
du dich schneller, als du denkst.

**Farbe setzen geht immer in drei Schritten:**

```cpp
wetterLeds[0] = CRGB::Red;   // 1. Farbe aussuchen
FastLED.show();              // 2. an die LED schicken
delay(500);                  // 3. halbe Sekunde warten
```

> ⚠️ Ohne `FastLED.show()` passiert **nichts**. Das ist hier der Klassiker unter den Fehlern.

Das `::` heißt **Scope-Operator** und bedeutet: „nimm `Red` aus der Farbsammlung `CRGB`".

---

## Deine Aufgaben

### TODO 2.1 – Die LED anmelden
Schreibe die drei Zeilen, die Pin, Anzahl und den Farbspeicher festlegen.

> ⚠️ **Wichtig:** Solange TODO 2.1 offen ist, lässt sich das Programm **nicht**
> übersetzen. Du bekommst dann diese Meldung:
>
> ```
> 'wetterLeds' was not declared in this scope
> ```
>
> Das ist kein Defekt, sondern genau richtig. Übersetzt heißt es: *„Du benutzt weiter
> unten etwas namens `wetterLeds`, aber du hast mir nie gesagt, was das sein soll."*
>
> **Merk dir das Vorgehen:** Bei einer Fehlermeldung immer die **erste** rote Zeile
> lesen. Dort stehen Datei, Zeilennummer und das eigentliche Problem. Alles darunter
> sind meist nur Folgefehler.

### TODO 2.2 – Farben nacheinander anzeigen
Baue eine Abfolge aus **mindestens 5 Farben** – zum Beispiel einen Regenbogen.

Farben zur Auswahl:
`Red` `Orange` `Gold` `Yellow` `Green` `SpringGreen` `Aqua` `Cyan`
`Blue` `RoyalBlue` `Purple` `Magenta` `Pink` `White` `Amethyst` `Black`

> 💡 Tipp: Tippe `CRGB::` und warte kurz – VS Code schlägt dir alle über 100 Farben vor.

---

## So sieht es aus, wenn es klappt

Die LED wechselt im halben Sekundentakt durch deine Farben und fängt dann wieder von vorne an.

---

## Extras für Schnelle ⭐

- **A – Kürzer statt länger:** Ersetze die vielen fast gleichen Zeilen durch eine
  **Farbliste + `for`-Schleife**. Gleiches Ergebnis, ein Drittel Code. *(Das ist das
  wichtigste Extra – hier lernst du am meisten.)*
- **B – Weicher Regenbogen:** `CHSV(farbton, 255, 255)` dreht den Farbton stufenlos
  weiter, statt zwischen festen Farben zu springen.
- **C – Atmen:** LED langsam heller und wieder dunkler werden lassen.
- **D – Zufall:** `random(0, 256)` für jede Grundfarbe – jedes Mal eine neue Farbe.

---

## Wenn etwas nicht klappt

| Problem | Wahrscheinliche Ursache |
|---|---|
| LED bleibt dunkel | `FastLED.show()` vergessen, oder `LED_PIN` stimmt nicht |
| LED zeigt nur eine Farbe | `delay()` fehlt – die Farben wechseln zu schnell zum Sehen |
| LED blendet extrem | `FastLED.setBrightness()` runtersetzen, z. B. auf 40 |
| Farbe stimmt nicht ganz | manche LEDs vertauschen Rot und Grün – nicht schlimm, probier eine andere Farbe |
| `'wetterLeds' was not declared` | TODO 2.1 fehlt oder steht **unter** `setup()` statt darüber |
