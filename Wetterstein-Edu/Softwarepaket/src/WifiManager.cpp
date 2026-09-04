#include "WifiManager.h"
#include <EEPROM.h>

// EEPROM-Aufteilung: Kennung | SSID | Passwort | Ort
#define EEPROM_SIZE     256
#define EEPROM_MAGIC    0x57C2      // Kennung: "hier stehen gueltige Daten"
#define ADDR_MAGIC      0
#define ADDR_SSID       4           // 33 Byte (32 Zeichen + 0)
#define ADDR_PASS       40          // 65 Byte (64 Zeichen + 0)
#define ADDR_CITY       108         // 49 Byte (48 Zeichen + 0)
#define MAX_SSID_LEN    32
#define MAX_PASS_LEN    64
#define MAX_CITY_LEN    48

// Ort, der genutzt wird solange nichts eingetragen wurde
#define DEFAULT_CITY    "Aachen,de"

static const byte DNS_PORT = 53;

WifiManager::WifiManager(const char* apSsid, const char* apPassword)
  : _apSsid(apSsid), _apPassword(apPassword) {}

// ---------------------------------------------------------------- EEPROM

void WifiManager::loadCredentials() {
  EEPROM.begin(EEPROM_SIZE);

  uint16_t magic;
  EEPROM.get(ADDR_MAGIC, magic);

  _ssid = "";
  _password = "";
  _city = "";

  if (magic == EEPROM_MAGIC) {
    char ssid[MAX_SSID_LEN + 1] = {0};
    char pass[MAX_PASS_LEN + 1] = {0};
    char city[MAX_CITY_LEN + 1] = {0};
    for (int i = 0; i < MAX_SSID_LEN; i++) ssid[i] = EEPROM.read(ADDR_SSID + i);
    for (int i = 0; i < MAX_PASS_LEN; i++) pass[i] = EEPROM.read(ADDR_PASS + i);
    for (int i = 0; i < MAX_CITY_LEN; i++) city[i] = EEPROM.read(ADDR_CITY + i);
    _ssid = String(ssid);
    _password = String(pass);
    _city = String(city);
  }

  if (_city.length() == 0) _city = DEFAULT_CITY;

  EEPROM.end();
}

void WifiManager::saveCredentials(const String& ssid, const String& password, const String& city) {
  EEPROM.begin(EEPROM_SIZE);

  uint16_t magic = EEPROM_MAGIC;
  EEPROM.put(ADDR_MAGIC, magic);

  for (int i = 0; i < MAX_SSID_LEN; i++) EEPROM.write(ADDR_SSID + i, i < (int)ssid.length() ? ssid[i] : 0);
  for (int i = 0; i < MAX_PASS_LEN; i++) EEPROM.write(ADDR_PASS + i, i < (int)password.length() ? password[i] : 0);
  for (int i = 0; i < MAX_CITY_LEN; i++) EEPROM.write(ADDR_CITY + i, i < (int)city.length() ? city[i] : 0);

  EEPROM.commit();
  EEPROM.end();

  _ssid = ssid;
  _password = password;
  _city = city;
}

void WifiManager::resetCredentials() {
  EEPROM.begin(EEPROM_SIZE);
  for (int i = 0; i < EEPROM_SIZE; i++) EEPROM.write(i, 0);
  EEPROM.commit();
  EEPROM.end();

  _ssid = "";
  _password = "";
  _city = DEFAULT_CITY;
  Serial.println(F("WLAN-Zugangsdaten geloescht."));
}

// ---------------------------------------------------------------- Verbindung

void WifiManager::begin() {
  loadCredentials();

  if (_ssid.length() > 0) {
    connect();
  } else {
    Serial.println(F("Keine WLAN-Zugangsdaten gespeichert."));
  }

  if (!isConnected()) {
    startPortal();
  }
}

void WifiManager::connect() {
  if (_ssid.length() == 0) return;

  Serial.print(F("Verbinde mit WLAN: "));
  Serial.println(_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid.c_str(), _password.c_str());

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 30) {  // max. 15 Sekunden warten
    delay(500);
    Serial.print('.');
    retries++;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("WLAN verbunden!"));
    Serial.print(F("IP-Adresse: "));
    Serial.println(WiFi.localIP());
  } else {
    Serial.println(F("Verbindung fehlgeschlagen!"));
  }
}

bool WifiManager::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}

bool WifiManager::isPortalActive() {
  return _portalActive;
}

IPAddress WifiManager::getIP() {
  return _portalActive ? WiFi.softAPIP() : WiFi.localIP();
}

String WifiManager::getSSID() {
  return _ssid;
}

String WifiManager::getCity() {
  return _city.length() ? _city : String(DEFAULT_CITY);
}

// ---------------------------------------------------------------- Portal

void WifiManager::startPortal() {
  Serial.println(F("Starte Konfigurations-Access-Point..."));

  WiFi.mode(WIFI_AP);
  // Passwort nur setzen wenn es lang genug ist, sonst offener AP
  if (_apPassword && strlen(_apPassword) >= 8) {
    WiFi.softAP(_apSsid, _apPassword);
  } else {
    WiFi.softAP(_apSsid);
  }
  delay(100);

  IPAddress apIP = WiFi.softAPIP();
  Serial.print(F("AP-SSID: "));  Serial.println(_apSsid);
  Serial.print(F("AP-IP:   "));  Serial.println(apIP);

  // Captive Portal: alle DNS-Anfragen auf den ESP umleiten
  _dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  _dnsServer.start(DNS_PORT, "*", apIP);

  _webServer.on("/",     [this]() { handleRoot(); });
  _webServer.on("/save", [this]() { handleSave(); });
  _webServer.onNotFound([this]() { handleRoot(); });   // Captive-Portal-Erkennung
  _webServer.begin();

  _portalActive = true;
}

void WifiManager::loop() {
  checkSerialSetup();   // Einrichtung ueber USB ist immer moeglich
  if (!_portalActive) return;
  _dnsServer.processNextRequest();
  _webServer.handleClient();
}

// ---------------------------------------------------------------- Serielle Einrichtung

void WifiManager::checkSerialSetup() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\n' || c == '\r') {
      if (_serialBuffer.length() > 0) {
        String zeile = _serialBuffer;
        _serialBuffer = "";
        processSerialCommand(zeile);
      }
    } else if (_serialBuffer.length() < 200) {   // Schutz gegen Muell auf der Leitung
      _serialBuffer += c;
    } else {
      _serialBuffer = "";
    }
  }
}

void WifiManager::processSerialCommand(const String& zeile) {
  String cmd = zeile;
  cmd.trim();

  if (cmd == "PING") {          // Erreichbarkeitstest der Webseite
    Serial.println(F("PONG"));
    return;
  }

  if (!cmd.startsWith("SETUP:")) return;   // andere Ausgaben ignorieren

  String rest = cmd.substring(6);
  int t1 = rest.indexOf('|');
  int t2 = rest.indexOf('|', t1 + 1);
  if (t1 < 0 || t2 < 0) {
    Serial.println(F("ERR Format"));
    return;
  }

  String ssid = rest.substring(0, t1);
  String pass = rest.substring(t1 + 1, t2);
  String ort  = rest.substring(t2 + 1);
  ssid.trim();
  ort.trim();

  if (ssid.length() == 0) {
    Serial.println(F("ERR SSID leer"));
    return;
  }
  if (ort.length() == 0) ort = DEFAULT_CITY;

  saveCredentials(ssid, pass, ort);
  Serial.println(F("OK"));
  Serial.flush();
  delay(300);
  ESP.restart();
}

// ---------------------------------------------------------------- Portal-Seite

String WifiManager::buildPage(const String& hinweis) {
  String html;
  html.reserve(3000);

  html += F("<!DOCTYPE html><html><head><meta charset='utf-8'>"
            "<meta name='viewport' content='width=device-width,initial-scale=1'>"
            "<title>Wetterstation WLAN</title><style>"
            "html{font-family:Arial;background:#f2f2f2;}"
            "body{margin:0;padding:16px;}"
            ".box{max-width:420px;margin:0 auto;background:#fff;padding:16px;"
            "box-shadow:2px 2px 12px 1px rgba(140,140,140,.5);}"
            "h3{background:#428f9c;color:#fff;margin:-16px -16px 16px;padding:10px;text-align:center;}"
            "label{display:block;margin-top:12px;font-weight:bold;}"
            "input,select{width:100%;padding:8px;margin-top:4px;box-sizing:border-box;}"
            "button{width:100%;padding:10px;margin-top:16px;background:#428f9c;color:#fff;"
            "border:0;font-size:1rem;}"
            ".hint{color:#b00;margin-top:10px;}"
            ".tipp{color:#666;font-size:.85rem;margin:6px 0 0;}"
            "</style></head><body><div class='box'><h3>Wetterstation WLAN</h3>");

  if (hinweis.length()) html += "<p class='hint'>" + hinweis + "</p>";

  html += F("<form action='/save' method='POST'>"
            "<label for='ssid'>Netzwerk</label><select id='ssid' name='ssid'>");

  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    html += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) +
            " (" + String(WiFi.RSSI(i)) + " dBm)</option>";
  }
  WiFi.scanDelete();

  html += F("</select>"
            "<label for='ssidManuell'>oder SSID manuell eingeben</label>"
            "<input type='text' id='ssidManuell' name='ssidManuell' maxlength='32' placeholder='(optional)'>"
            "<label for='pass'>Passwort</label>"
            "<input type='password' id='pass' name='pass' maxlength='64'>"
            "<label for='ort'>Ort fuer die Wettervorhersage</label>");

  html += "<input type='text' id='ort' name='ort' maxlength='48' value='" +
          getCity() + "'>";

  html += F("<p class='tipp'>Format: Ort,Laendercode &ndash; z.&nbsp;B. "
            "<code>Aachen,de</code>, <code>Wien,at</code> oder "
            "<code>New York,us</code></p>"
            "<button type='submit'>Speichern und neu starten</button>"
            "</form></div></body></html>");

  return html;
}

void WifiManager::handleRoot() {
  _webServer.send(200, "text/html; charset=utf-8", buildPage(""));
}

void WifiManager::handleSave() {
  String ssid = _webServer.arg("ssidManuell");
  if (ssid.length() == 0) ssid = _webServer.arg("ssid");
  String pass = _webServer.arg("pass");
  String ort  = _webServer.arg("ort");

  ssid.trim();
  ort.trim();
  if (ort.length() == 0) ort = DEFAULT_CITY;

  if (ssid.length() == 0) {
    _webServer.send(200, "text/html; charset=utf-8",
                    buildPage("Bitte ein Netzwerk auswaehlen oder eine SSID eintragen."));
    return;
  }

  saveCredentials(ssid, pass, ort);
  Serial.print(F("Zugangsdaten gespeichert fuer: "));
  Serial.print(ssid);
  Serial.print(F(" | Ort: "));
  Serial.println(ort);

  _webServer.send(200, "text/html; charset=utf-8",
                  F("<!DOCTYPE html><html><head><meta charset='utf-8'>"
                    "<meta name='viewport' content='width=device-width,initial-scale=1'></head>"
                    "<body style='font-family:Arial;text-align:center;padding:30px'>"
                    "<h3>Gespeichert!</h3>"
                    "<p>Die Wetterstation startet jetzt neu und verbindet sich mit dem WLAN.</p>"
                    "</body></html>"));

  delay(1500);
  ESP.restart();
}
