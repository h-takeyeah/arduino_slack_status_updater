#include <Arduino.h>
#include <HTTPClient.h>
#include <M5Stack.h>
#include <WiFi.h>

#include "secrets.h"

HTTPClient client;

void setup() {
  M5.begin();
  M5.Power.begin();
}

void do_post(String payload) {
  WiFi.begin(ssid, pass);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("connecting to Wifi");
  unsigned long timeout = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - timeout > 5000) {
      M5.Lcd.println("\nfailed to connect to WiFi");
      return;
    }
    M5.Lcd.print(".");
    delay(1000);
  }

  M5.Lcd.printf("\nconnected to %s\n", ssid);

  M5.Lcd.println("[HTTPS] begin...");
  client.begin(F("https://slack.com/api/users.profile.set"), root_ca);

  client.addHeader(F("Content-Type"), F("application/json"));
  String auth = "Bearer ";
  auth += slack_user_token;
  client.addHeader("Authorization", auth);

  int httpCode = client.POST(payload);

  if (httpCode > 0) {
    M5.Lcd.printf("[HTTPS] GET... code=%d\n", httpCode);
  } else {
    M5.Lcd.printf("[HTTPS] GET... failed, error=%s\n", client.errorToString(httpCode).c_str());
  }

  M5.Lcd.println("[HTTPS] end");
  Wifi.disconnect();
}

void loop() {
  M5.update();
  if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200)) {
    do_post(F("{\"profile\":{\"status_text\":\"riding a train\",\"status_emoji\":\":train:\"}}"));
    M5.Lcd("slack status was set");
  } else if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200)) {
    do_post();
    M5.Lcd("slack status was cleared");
  }
}
