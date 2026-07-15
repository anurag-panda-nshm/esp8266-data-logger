#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 0 // Pin D3 on NodeMCU corresponds directly to GPIO0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const char* ssid = "MY_SSID";
const char* password = "anuragini1234";
const char* serverUrl = "https://dataloggeresp.fastapicloud.dev/api/temperature";

// Your Root CA Certificate structure (Using the Root Trust Anchor Method)
const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n" \
"TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
"cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n" \
"WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n" \
"ZXQgU2VjdXJyaXR5IFJlc2VhcmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3Qg\n" \
"WDEwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCzh9fiQkcCR0eC7A/r\n" \
"XCHcSDJA7quDk4CDQoCz1FRIWE4tvjJeUDgRgVVlCQHXK5H4FS5dFDwmG6N0DDyH\n" \
"YDnvzVqFr76313SaZFAaGspNkq3DF9bSS5WZ2BiJaGUeugkrgh1wXIItyGlBBIeD\n" \
"RbQWGoBBnmIuYrLO3c1fieJeUsyvJCEV3cg2CqJ6wTE4A98UR+j4wU5B564u+Eka\n" \
"Dhb4WjQP7kK6AglGJCQCIB4vI1F9zM1fBH55V156U16Hj5U49c5e3g=\n" \
"-----END CERTIFICATE-----\n";

void setup() {
  Serial.begin(115200);
  sensors.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nWi-Fi Connected secure!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    sensors.requestTemperatures(); 
    float tempC = sensors.getTempCByIndex(0);
    float tempF = sensors.getTempFByIndex(0);

    // Filter out sensor read errors (-127C means disconnected wire)
    if (tempC == DEVICE_DISCONNECTED_C) {
      Serial.println("Error: DS18B20 sensor not detected on D0.");
      delay(5000);
      return;
    }

    WiFiClientSecure client;
    BearerSSL::X509List cert(root_ca);
    client.setTrustAnchors(&cert);

    HTTPClient http;
    if (http.begin(client, serverUrl)) {
      http.addHeader("Content-Type", "application/json");

      String jsonPayload = "{\"device_id\":\"ds18b20_node\",\"temp_c\":" + String(tempC) + ",\"temp_f\":" + String(tempF) + "}";
      
      int httpResponseCode = http.POST(jsonPayload);
      if (httpResponseCode > 0) {
        Serial.printf("Telemetry Push: %d. Sent C: %s, F: %s\n", httpResponseCode, String(tempC).c_str(), String(tempF).c_str());
      } else {
        Serial.printf("Error Code: %s\n", http.errorToString(httpResponseCode).c_str());
      }
      http.end();
    }
  }
  delay(10000); // Poll every 10 seconds
}
