#include "WetterAPI.h"

/**
 * @param stadt (eg.: Aachen,de)
 * @param apiSchluessel 
 */
WetterAPI::WetterAPI(const String& stadt, const String& apiSchluessel)
  : city(stadt), apiKey(apiSchluessel) {}

void WetterAPI::begin() {}

void WetterAPI::setCity(const String& stadt) {
  if (stadt.length() > 0) city = stadt;
}

String WetterAPI::getCity() {
  return city;
}

/**
 * Ortsnamen fuer die URL kodieren (Leerzeichen, Komma, Umlaute).
 */
static String urlEncode(const String& text) {
  String encoded;
  encoded.reserve(text.length() * 3);
  for (size_t i = 0; i < text.length(); i++) {
    char c = text[i];
    if (isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~') {
      encoded += c;
    } else {
      char buf[4];
      snprintf(buf, sizeof(buf), "%%%02X", (unsigned char)c);
      encoded += buf;
    }
  }
  return encoded;
}

void WetterAPI::fetchCurrentWeather() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;

    String url = "http://api.openweathermap.org/data/2.5/weather?q=" + urlEncode(city) + "&appid=" + apiKey + "&units=metric&lang=de";

    if (http.begin(client, url)) {
      int httpCode = http.GET();
      yield();

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        StaticJsonDocument<1024> doc;
        StaticJsonDocument<256> filter;

        filter["dt"] = true;
        filter["timezone"] = true;                // add timezone offset (seconds)
        filter["main"]["temp"] = true;
        filter["weather"][0]["description"] = true;
        filter["wind"]["speed"] = true;

        if (deserializeJson(doc, payload, DeserializationOption::Filter(filter))) return;

        long tzOffset = doc["timezone"] | 0;      // seconds from UTC (handles MEZ/MESZ)
        time_t utc = doc["dt"].as<time_t>();
        time_t local = utc + tzOffset;            // convert to local time

        struct tm* timeinfo = gmtime(&local);     // gmtime on already-offset timestamp
        char timeStr[25];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);

        float temp = doc["main"]["temp"];
        const char* desc = doc["weather"][0]["description"];
        float wind = doc["wind"]["speed"];

        currentWeather[0][0] = String(timeStr);
        currentWeather[0][1] = String(temp, 1);
        currentWeather[0][2] = String(desc);
        currentWeather[0][3] = String(wind, 1);
      }

      http.end();
    }
  }
}

void WetterAPI::fetchWeatherForecast() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;

    String url = "http://api.openweathermap.org/data/2.5/forecast?q=" + urlEncode(city) + "&appid=" + apiKey + "&units=metric&lang=de&cnt=8";

    if (http.begin(client, url)) {
      int httpCode = http.GET();

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();

        StaticJsonDocument<32 * 1024> doc;
        StaticJsonDocument<2000> filter;

        // Request timezone and use dt instead of dt_txt
        filter["city"]["timezone"] = true;
        for (int i = 0; i < 8; i++) {
          filter["list"][i]["dt"] = true;                   // <-- use Unix time
          filter["list"][i]["main"]["temp"] = true;
          filter["list"][i]["weather"][0]["description"] = true;
          filter["list"][i]["wind"]["speed"] = true;
        }

        if (deserializeJson(doc, payload, DeserializationOption::Filter(filter))) return;

        long tzOffset = doc["city"]["timezone"] | 0;        // seconds from UTC (handles MEZ/MESZ)
        JsonArray list = doc["list"];
        for (size_t i = 0; i < 8 && i < list.size(); i++) {
          JsonObject forecast = list[i];

          // Convert to local time using timezone offset
          time_t raw = forecast["dt"].as<time_t>();
          time_t local = raw + tzOffset;
          struct tm* t = gmtime(&local);                    // gmtime of already-offset timestamp
          char timeStr[25];
          strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", t);

          forecastData[i][0] = String(timeStr);
          forecastData[i][1] = String(forecast["main"]["temp"].as<float>(), 1);
          forecastData[i][2] = String((const char*)forecast["weather"][0]["description"]);
          forecastData[i][3] = String(forecast["wind"]["speed"].as<float>(), 1);
        }
      }

      http.end();
    }
  }
}
