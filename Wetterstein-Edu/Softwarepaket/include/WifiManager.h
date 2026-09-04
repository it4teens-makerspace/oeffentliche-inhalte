#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

/**
 * WLAN-Verwaltung mit Konfigurations-Portal.
 *
 * Ablauf:
 *  - begin() laedt gespeicherte Zugangsdaten aus dem EEPROM und versucht,
 *    sich damit zu verbinden.
 *  - Klappt das nicht (oder es ist noch nichts gespeichert), oeffnet der ESP
 *    einen eigenen Access Point. Dort kann man ueber eine kleine Webseite
 *    SSID und Passwort eintragen; danach startet der ESP neu.
 *  - Solange das Portal laeuft, muss loop() regelmaessig aufgerufen werden.
 */
class WifiManager {
  public:
    // ssid/password = Zugangsdaten des Konfigurations-Access-Points
    WifiManager(const char* apSsid, const char* apPassword);

    void begin();              // Zugangsdaten laden, verbinden, ggf. Portal starten
    void connect();            // Verbindung mit den gespeicherten Daten versuchen
    void loop();               // Portal bedienen (nur noetig wenn Portal aktiv)

    bool isConnected();
    bool isPortalActive();
    IPAddress getIP();         // STA-IP, im Portal-Betrieb die AP-IP
    String getSSID();          // gespeicherte SSID
    String getCity();          // gespeicherter Ort fuer die Wetter-API

    void startPortal();        // Access Point + Webseite starten
    void resetCredentials();   // gespeicherte Zugangsdaten loeschen

    // Einrichtung ueber die serielle Schnittstelle (USB), z. B. direkt nach
    // dem Flashen im Browser. Muss regelmaessig aufgerufen werden.
    // Protokoll (je eine Zeile):
    //   PING                          -> PONG
    //   SETUP:<ssid>|<passwort>|<ort> -> OK und Neustart, sonst ERR <grund>
    void checkSerialSetup();

  private:
    void loadCredentials();
    void saveCredentials(const String& ssid, const String& password, const String& city);
    String buildPage(const String& hinweis);
    void handleRoot();
    void handleSave();
    void processSerialCommand(const String& zeile);

    const char* _apSsid;
    const char* _apPassword;

    String _ssid;
    String _password;
    String _city;

    bool _portalActive = false;
    String _serialBuffer;

    ESP8266WebServer _webServer{80};
    DNSServer        _dnsServer;
};

#endif
