#include <Arduino.h>
#include <HTTPClient.h>
#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "secrets.h"

void setup() {
  M5.begin();
  M5.Power.begin();
  Serial.begin(115200);
}

void do_post(String payload) {
  WiFi.begin(ssid, pass);
  Serial.print("connecting to Wifi");
  unsigned long timeout = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - timeout > 5000) {
      Serial.println("\nfailed to connect to WiFi");
      return;
    }
    Serial.print(".");
    delay(1000);
  }

  Serial.printf("\nconnected to %s\n", ssid);

  Serial.println("[HTTPS] begin...");

  WiFiClientSecure *client = new WiFiClientSecure;
  if (client) {
    client->setCACert(root_ca);
    {
      // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
      HTTPClient https;
      if (https.begin(*client, F("https://slack.com/api/users.profile.set"))) {
        https.addHeader(F("Content-Type"), F("application/json"));
        String auth = "Bearer ";
        auth += slack_user_token;
        https.addHeader("Authorization", auth);

        int httpCode = https.POST(payload);

        if (httpCode > 0) {
          Serial.printf("[HTTPS] POST... code=%d\n", httpCode);
        } else {
          Serial.printf("[HTTPS] POST... failed, error=%s\n", https.errorToString(httpCode).c_str());
        }

        Serial.println("[HTTPS] end");
        https.end();
      } else {
        Serial.println("[HTTPS] unable to connect");
      }
    }
    delete client;
  }
}

void loop() {
  M5.update();
  if (M5.BtnA.wasReleased()) {
    do_post("{\"profile\":{\"status_text\":\"riding a train\",\"status_emoji\":\":train:\"}}");
    Serial.println("slack status was set");
    delay(1000);
  } else if (M5.BtnB.wasReleased()) {
    do_post("{\"profile\":{\"status_text\":\"\",\"status_emoji\":\"\"}}");
    Serial.println("slack status was cleared");
    delay(1000);
  }
}
