#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Bisu@1975";
const char* password = "Biswajit@1481";

void fetchWeather()
{
    WiFiClientSecure client;
    client.setInsecure();   // Ignore SSL certificate

    HTTPClient http;

    String url = "https://api.open-meteo.com/v1/forecast?latitude=22.5726&longitude=88.3639&current=temperature_2m,relative_humidity_2m,wind_speed_10m";

    if (http.begin(client, url))
    {
        int httpCode = http.GET();

        Serial.print("HTTP Code: ");
        Serial.println(httpCode);

        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();

            Serial.println("---------------");
            Serial.println(payload);
            Serial.println("---------------");

            JsonDocument doc;

            DeserializationError error = deserializeJson(doc, payload);

            if (!error)
            {
                float temp = doc["current"]["temperature_2m"];
                int hum = doc["current"]["relative_humidity_2m"];
                float wind = doc["current"]["wind_speed_10m"];

                Serial.println("===== WEATHER =====");
                Serial.print("Temperature : ");
                Serial.print(temp);
                Serial.println(" °C");

                Serial.print("Humidity    : ");
                Serial.print(hum);
                Serial.println(" %");

                Serial.print("Wind Speed  : ");
                Serial.print(wind);
                Serial.println(" km/h");
                Serial.println("===================");
            }
            else
            {
                Serial.println("JSON Parse Failed");
            }
        }
        else
        {
            Serial.print("HTTP Error: ");
            Serial.println(httpCode);
        }

        http.end();
    }
    else
    {
        Serial.println("Unable to connect to server.");
    }
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.print("Connecting");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    fetchWeather();
}

void loop()
{
    static unsigned long lastUpdate = 0;

    if (millis() - lastUpdate > 60000)
    {
        lastUpdate = millis();
        fetchWeather();
    }
}