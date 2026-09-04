# Wetterstation (ESP8266)

Eine WLAN-Wetterstation auf Basis eines NodeMCU (ESP8266). Sie misst lokal
Temperatur, Luftfeuchte und Luftdruck mit einem BME280 und holt sich zusätzlich
das aktuelle Wetter sowie eine 24-Stunden-Vorhersage von
[OpenWeatherMap](https://openweathermap.org/). Die Werte erscheinen auf einem
OLED-Display, auf einer eingebauten Webseite und als Farbe einer RGB-LED.

WLAN-Zugangsdaten und der Ort werden **nicht im Code hinterlegt**, sondern beim
ersten Start über ein eigenes Setup-WLAN eingetragen und im EEPROM gespeichert.

## Funktionen

- **Sensorwerte**: Temperatur, Luftfeuchte, Luftdruck, berechnete Höhe (BME280, I²C)
- **Wetterdaten**: aktuelles Wetter und Vorhersage in 3-Stunden-Schritten, ortsbezogen
- **OLED-Display**: sechs Seiten, per Taster durchschaltbar
- **Webseite**: alle Werte im Browser unter der IP der Station, aktualisiert sich alle 5 s
- **RGB-LED**: zeigt die Luftfeuchte als Farbverlauf von Rot (trocken) bis Dunkelblau (feucht)
- **Setup-Portal**: Access Point mit Captive Portal zum Eintragen von WLAN und Ort

## Hardware

| Bauteil | Anschluss am NodeMCU |
|---|---|
| BME280 (I²C, Adresse `0x76`) | `D1` = SCL, `D2` = SDA, `3V3`, `GND` |
| OLED SSD1306 128×64 (I²C) | dieselben I²C-Leitungen `D1` / `D2` |
| WS2812 / NeoPixel (1 LED) | `D4` (GPIO2) |
| Taster | `D3` (GPIO0) gegen `GND`, interner Pull-up |

## Installation

### Variante A – im Browser flashen (empfohlen)

ESP per USB anschließen und auf der Flash-Seite auf *Verbinden* klicken:

> https://it4teens-makerspace.github.io/oeffentliche-inhalte/wetterstation/

Funktioniert mit Chrome, Edge oder Opera auf einem Desktop-Rechner (Web Serial).
Firefox, Safari und Mobilgeräte werden nicht unterstützt. Unter Windows ist ggf.
der USB-Treiber CH340 oder CP210x nötig.

### Variante B – mit PlatformIO

```bash
git clone https://github.com/it4teens-makerspace/oeffentliche-inhalte.git
cd oeffentliche-inhalte/Wetterstein-Edu/Softwarepaket
pio run -t upload
```

## Erste Inbetriebnahme

WLAN-Zugangsdaten und Ort lassen sich auf zwei Wegen eintragen – beide führen
zum selben Ergebnis, gespeichert wird im EEPROM.

### Über die Flash-Seite (per USB)

Direkt nach dem Flashen im Schritt 2 der Webseite SSID, Passwort und Ort
eingeben und senden. Die Daten gehen über dasselbe USB-Kabel an die Station,
die sie speichert und neu startet. Es gibt keinen Server, der etwas
entgegennimmt – die Eingaben verlassen den Browser nicht.

### Über das Setup-WLAN (ohne USB, auch vom Handy)

1. Die Station öffnet das WLAN **`Wetterstation-Setup`** (Passwort `wetter1234`).
2. Mit diesem WLAN verbinden. Die Setup-Seite öffnet sich meist automatisch,
   sonst im Browser `192.168.4.1` aufrufen.
3. Eigenes WLAN aus der Liste wählen (oder SSID manuell eintragen), Passwort
   eingeben und den **Ort** im Format `Ort,Ländercode` angeben – z. B.
   `Aachen,de`, `Wien,at`, `New York,us`.
4. Speichern. Die Station startet neu und verbindet sich. Die vergebene
   IP-Adresse steht auf der ersten Display-Seite und im seriellen Monitor
   (115200 Baud).

### Serielles Protokoll

Wer die Station aus eigenen Werkzeugen einrichten will, sendet über USB bei
115200 Baud eine Zeile:

```
SETUP:<ssid>|<passwort>|<ort>
```

Die Station antwortet mit `OK` und startet neu, bei fehlerhafter Eingabe mit
`ERR <grund>`. `PING` wird mit `PONG` beantwortet. Das funktioniert im laufenden
Betrieb ebenso wie im Setup-Modus.

## Bedienung

**Kurzer Tastendruck** schaltet die Display-Seite weiter:

| Seite | Inhalt |
|---|---|
| 0 | Name, WLAN, IP-Adresse, eingestellter Ort |
| 1 | Sensorwerte: Temperatur, Feuchte, Druck, Höhe (oder Sensor-Hinweis) |
| 2 | Aktuelles Wetter vom API |
| 3–5 | Vorhersage für die nächsten drei Zeitpunkte (+3 h, +6 h, +9 h) |

**Taster 3 Sekunden halten** löscht die gespeicherten Zugangsdaten und startet
die Station neu – sie geht dann wieder ins Setup-Portal. Nützlich beim
WLAN-Wechsel oder wenn die Station weitergegeben wird.

Die **Webseite** erreichst du unter `http://<IP-der-Station>` im selben Netz.

## Konfiguration im Code

Fast alles wird zur Laufzeit eingestellt. Im Code anpassbar sind in
[`src/main.cpp`](src/main.cpp):

- `AP_SSID` / `AP_PASSWORD` – Name und Passwort des Setup-Netzes
  (mindestens 8 Zeichen, sonst wird ein offenes Netz aufgemacht)
- der **OpenWeather-API-Key** im Konstruktor von `WetterAPI`
- die Update-Intervalle: Vorhersage alle 30 min, aktuelles Wetter alle 10 min

> **Hinweis zum API-Key:** Der Schlüssel wird mit in die Firmware kompiliert und
> lässt sich aus einer veröffentlichten `firmware.bin` auslesen. Für ein
> öffentliches Repository einen eigenen, kostenlosen Key bei OpenWeatherMap
> anlegen und ihn nicht als geheim betrachten.

## Fehlersuche

**„BME280 fehlt!" auf dem Display.** Die Station läuft weiter, zeigt aber keine
eigenen Messwerte – Wetterdaten aus dem Internet und die Webseite funktionieren
normal. Im seriellen Monitor (115200 Baud) gibt sie beim Start einen I²C-Scan
aus, der alle gefundenen Adressen auflistet. Typische Ursachen:

- **Nichts gefunden:** Verkabelung prüfen. SDA gehört an `D2`, SCL an `D1`,
  dazu `3V3` und `GND`. Ein vertauschtes SDA/SCL ist der häufigste Fehler.
- **Ein Gerät gefunden, aber der Sensor wird trotzdem abgelehnt:** Auf dem
  Modul sitzt vermutlich ein **BMP280** statt eines BME280. Beide nutzen
  dieselben Adressen, der BMP280 misst aber keine Luftfeuchte und wird von der
  Bibliothek an der Chip-Kennung erkannt und abgelehnt.
- Die Adressen `0x76` und `0x77` werden beide automatisch probiert, ein
  Jumper auf dem Modul spielt also keine Rolle.

## Projektstruktur

```
src/main.cpp            Setup, Hauptschleife, Display, eingebauter Webserver
src/WifiManager.cpp     WLAN-Verbindung, Setup-Portal, EEPROM-Speicherung
src/WetterAPI.cpp       Abfrage und Auswertung der OpenWeather-Daten
web/                    Quellen der Flash-Seite (GitHub Pages)
```

Das Projekt liegt im Repo unter `Wetterstein-Edu/Softwarepaket/`. Der Workflow
`.github/workflows/wetterstation-flasher.yml` im Repo-Wurzelverzeichnis baut bei
jedem Push auf `main` die Firmware, packt den Quellcode als `Softwarepaket.zip`
und aktualisiert die Flash-Seite automatisch.
