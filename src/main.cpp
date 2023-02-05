#include <Arduino.h>

#include "ESP8266WiFi.h"
#include <ESP8266HTTPClient.h>
#include "ArduinoJson.h"

#include "wifi_params.hpp"

struct WeatherData
{
    float temperature = 0;
    int weathercode = 0;

    void update(JsonObject new_data)
    {
        temperature = new_data["temperature"].as<float>();
        weathercode = new_data["weathercode"].as<int>();
    }
};

// Weather API URL
const char *weather_api = "https://api.open-meteo.com/v1/forecast?latitude=49.83&longitude=18.28&current_weather=true&timezone=Europe%2FBerlin";
const char *time_api = "https://www.timeapi.io/api/Time/current/coordinate?latitude=49.84&longitude=18.279999";

// Allocate the memory for the JSON document
StaticJsonDocument<600> weather_doc;
StaticJsonDocument<600> time_doc;

// Helper variables
unsigned long long int sleepTime = 3600000000; // 3600000000 microseconds = 1 hour
WiFiClientSecure client;
HTTPClient http;

WeatherData data;

void setup()
{
  Serial.begin(9600);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  // Allow insecure https requests
  client.setInsecure();
}

void loop()
{
  // Check WiFi connection status
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi Disconnected");
  }
  else // Normal state
  {
    // Get the current time
    http.begin(client, time_api);
    if (http.GET() == HTTP_CODE_OK)
    {
      deserializeJson(time_doc, http.getString());
      Serial.printf("Time: %02d:%02d\n", time_doc["hour"].as<int>(), time_doc["minute"].as<int>());
      sleepTime = static_cast<unsigned long long int>(65 - time_doc["minute"].as<int>()) * 60000000llu;
    }

    // Fetch data from the weather API
    http.begin(client, weather_api);

    if (http.GET() == HTTP_CODE_OK)
    {
      deserializeJson(weather_doc, http.getString());
      data.update(weather_doc["current_weather"].as<JsonObject>());
      Serial.printf("Temperature: %.01f\nWeather code: %02d\n", data.temperature, data.weathercode);
    }
  }
  Serial.printf("Going to sleep for: %llu microseconds (%d minutes)\n", sleepTime, static_cast<int>(sleepTime / 60000000llu));
  ESP.deepSleep(sleepTime);
}
