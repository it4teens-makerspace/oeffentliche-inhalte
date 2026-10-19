# Aufgabe 4 – Der Wettersensor

**Zeit:** ca. 40 Minuten · **Datei zum Bearbeiten:** `src/main_BME.cpp`

---

## Worum geht es?

Das ist die Königsaufgabe: Hier kommt **alles zusammen**, was du bisher gelernt hast.
Der Sensor misst das Wetter, das Display zeigt die Werte, und die LED verrät auf
einen Blick, wie feucht die Luft ist. Danach ist dein Wetterstein fertig.

---

## Was du wissen musst

### Der Sensor: BME280

Ein Chip, kleiner als dein Fingernagel, der drei Dinge gleichzeitig misst:

| Messgröße | Einheit | typischer Wert im Zimmer |
|---|---|---|
| Temperatur | °C | 20 – 25 |
| Luftfeuchtigkeit | % | 35 – 60 |
| Luftdruck | hPa | 980 – 1030 |

Die **Höhe** misst er nicht wirklich – er *rechnet* sie aus dem Luftdruck aus.
Warum das nur ungefähr stimmt, steht in Extra E.

### `float` – Zahlen mit Komma

```cpp
float temperatur = 23.4;
```

`int` kann nur ganze Zahlen (23). `float` kann auch 23.4 – und genau das braucht man
bei Messwerten. Im Code schreibt man den Dezimal**punkt**, nicht das Komma.

### Pascal und Hektopascal

Der Sensor liefert den Druck in **Pascal** (etwa 101300). Im Wetterbericht steht aber
**Hektopascal** (etwa 1013). `1 hPa = 100 Pa` – deshalb `/ 100.0`.

### Die `if`-Kette

```cpp
if (luftfeuchte < 30) {
  wetterLeds[0] = CRGB::Red;
}
else if (luftfeuchte < 50) {
  wetterLeds[0] = CRGB::Gold;
}
else {
  wetterLeds[0] = CRGB::Blue;
}
```

Der Computer prüft **von oben nach unten** und nimmt den **ersten** Fall, der zutrifft.
Deshalb genügt immer nur die obere Grenze – „und größer als 30" musst du nicht
dazuschreiben, das ergibt sich von selbst.

> 💡 Das letzte `else` **ohne** Bedingung fängt alles ab, was übrig bleibt. So bleibt
> die LED nie dunkel, egal was der Sensor misst. In der alten Version dieses Programms
> fehlte genau das – bei 47 % Luftfeuchtigkeit ging die LED einfach aus, und niemand
> wusste warum.

---

## Deine Aufgaben

### TODO 4.1 – Den Sensor auslesen
Vier Variablen stehen schon da, überall mit einer `0`. Ersetze jede `0` durch den
passenden Sensor-Befehl.

> 💡 Lade das Programm ruhig **jetzt schon** hoch und öffne den Serial Monitor.
> Solange dort überall `0.00` steht, weißt du: Diese Aufgabe ist noch offen.
> Sobald echte Zahlen erscheinen, hast du sie geschafft.

### TODO 4.2 – Die Werte auf das Display bringen
Vier Zeilen (0, 2, 4 und 6), jeweils mit Bezeichnung und Einheit.

### TODO 4.3 – Die LED zeigt die Luftfeuchtigkeit
Eine `if`/`else if`/`else`-Kette mit **mindestens 4 Stufen**.

---

## So sieht es aus, wenn es klappt

Auf dem Display stehen vier aktuelle Messwerte, die sich jede Sekunde aktualisieren.
Die LED leuchtet passend zur Luftfeuchtigkeit. **Der Wetterstein lebt.**

---

## Extras für Schnelle ⭐

- **A – Hauch mal drauf:** Atme aus etwas Abstand Richtung Sensor. Wie schnell reagiert
  die Feuchte, wie lange dauert der Weg zurück? *(Sensor dabei nicht anfassen.)*
- **B – Stufenlose Farbe:** `map()` + `CHSV()` statt fester Stufen – die Farbe wandert
  fließend von Rot über Grün nach Blau. *(Das eindrucksvollste Extra.)*
- **C – Höchst- und Tiefstwert:** merken und anzeigen.
- **D – Zwei Display-Seiten mit dem Taster:** Aufgabe 3 und 4 kombiniert.
- **E – Denk mal nach:** Warum stimmt die angezeigte Höhe nie? Antwort steht in der
  Code-Datei ganz unten.

---

## Wenn etwas nicht klappt

| Problem | Wahrscheinliche Ursache |
|---|---|
| Display zeigt „BME nicht gefunden" | Sensor-Stecker sitzt nicht, oder er hat die Adresse `0x77` statt `0x76` – im Code ändern und testen |
| Alle Werte sind `0.00` | TODO 4.1 ist noch offen |
| Druck ist etwa `101300` statt `1013` | `/ 100.0` fehlt |
| Zahlen überlagern sich / Reste alter Zahlen | `oled.clear()` am Anfang von TODO 4.2 fehlt |
| Display bleibt leer | `oled.update()` am Ende vergessen |
| Temperatur ist 2–3 °C zu hoch | normal! Der Chip sitzt neben der warmen Platine und heizt sich selbst mit |
| LED geht bei manchen Werten aus | das abschließende `else` fehlt |
