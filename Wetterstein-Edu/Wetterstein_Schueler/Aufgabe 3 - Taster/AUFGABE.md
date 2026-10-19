# Aufgabe 3 – Der Taster

**Zeit:** ca. 30 Minuten · **Datei zum Bearbeiten:** `src/main_Button.cpp`

---

## Worum geht es?

Bis jetzt hat der Wetterstein einfach sein Programm heruntergespult. Jetzt soll er
auf **dich** reagieren: Bei jedem Druck auf den Taster wechselt die LED die Farbe.

Damit lernst du zwei Dinge, die du in fast jedem Programm wiedersehen wirst:
**Eingaben lesen** und **zwischen Zuständen umschalten**.

---

## Was du wissen musst

### Warum ein Taster „prellt"

Im Taster sind zwei Metallteile. Beim Drücken springen die für ein paar
Millisekunden wie ein Flummi hin und her – also zigmal an, aus, an, aus.

Dein Auge merkt davon nichts. Der Mikrocontroller schon: Er schaut **tausende Male
pro Sekunde** nach und würde jedes Zappeln als eigenen Druck zählen. Die LED würde
bei einem Druck wild durch alle Farben rasen.

Die Lösung heißt **Entprellen** (englisch *debouncing*): Nach der ersten Änderung
ignorieren wir 50 Millisekunden lang alles Weitere. Dann ist das Zappeln vorbei –
und ein Druck ist wirklich ein Druck.

> 👍 Gute Nachricht: Das Entprellen ist schon fertig für dich programmiert, in der
> Funktion `tasterWurdeGedrueckt()`. Lies sie dir ruhig durch, aber du musst sie
> nicht anfassen.

### `INPUT_PULLUP` – warum ist gedrückt gleich `LOW`?

Der Pin ist von sich aus auf `HIGH` (Strom liegt an). Der Taster verbindet ihn beim
Drücken mit Masse, also `LOW`. Klingt verdreht, ist aber bei Tastern der Normalfall –
so spart man sich einen extra Widerstand auf der Platine.

**Gedrückt = `LOW`.**

### Der `switch` – eine Weiche

```cpp
switch (farbNummer) {
  case 0:
    wetterLeds[0] = CRGB::Red;
    break;
  case 1:
    wetterLeds[0] = CRGB::Orange;
    break;
}
```

Je nachdem, welchen Wert `farbNummer` gerade hat, wird **genau ein** Fall ausgeführt.

> ⚠️ Das `break;` am Ende jedes Falls ist Pflicht. Ohne `break` läuft das Programm
> einfach in den nächsten Fall weiter – ein Fehler, den auch Profis regelmäßig machen.

---

## Deine Aufgaben

### TODO 3.1 – Anzahl der Farben festlegen
Trage bei `ANZAHL_FARBEN` ein, wie viele Fälle dein `switch` haben wird.

> ⚠️ Wenn du später eine Farbe dazu tust, musst du diese Zahl **mit erhöhen** –
> sonst wird deine neue Farbe nie erreicht.

### TODO 3.2 – Den `switch` vervollständigen
Zwei Fälle sind als Beispiel schon da. Ergänze die restlichen.

---

## So sieht es aus, wenn es klappt

Jeder Druck schaltet **eine** Farbe weiter. Nach der letzten Farbe geht es wieder
bei der ersten los. Im Serial Monitor siehst du bei jedem Druck eine Zeile.

> 💡 **Serial Monitor öffnen:** unten in VS Code auf das Stecker-Symbol
> (*Serial Monitor*) in der blauen Leiste klicken.

---

## Extras für Schnelle ⭐

- **A – Aus-Zustand:** ein Fall, in dem die LED aus ist (`CRGB::Black`).
- **B – Zähler:** mitzählen, wie oft insgesamt gedrückt wurde, und ausgeben.
- **C – Helligkeit statt Farbe:** Stufen 0 / 20 / 60 / 150 / 255 durchschalten.
- **D – Langer Druck:** über 1 Sekunde gehalten → zurück auf Farbe 0.
- **E – Zwei Bausteine zusammen:** den Farbnamen zusätzlich aufs Display schreiben.
  *(Das ist schon fast der fertige Wetterstein.)*

---

## Wenn etwas nicht klappt

| Problem | Wahrscheinliche Ursache |
|---|---|
| LED springt bei einem Druck mehrere Farben weiter | `tasterWurdeGedrueckt()` verändert – Original wiederherstellen |
| Eine Farbe wird übersprungen / LED bleibt dunkel | `ANZAHL_FARBEN` passt nicht zur Anzahl der `case`-Fälle |
| Alle Farben laufen von allein durch | ein `break;` fehlt |
| Taster reagiert gar nicht | Kabel prüfen; ist es wirklich Pin **D3**? |
| Upload schlägt fehl | Taster beim Einstecken/Reset **nicht** gedrückt halten – D3 ist beim Start ein Sonderpin |
