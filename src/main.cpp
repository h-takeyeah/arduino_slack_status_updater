#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "secrets.h"

RTC_DATA_ATTR int bootCount = 0;

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

void do_post(String payload) {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
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

  Serial.printf("\nconnected to %s\n", WIFI_SSID);

  Serial.println("[HTTPS] begin...");

  WiFiClientSecure *client = new WiFiClientSecure;
  if (client) {
    client->setCACert(root_ca);
    {
      // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
      HTTPClient https;
      if (https.begin(*client, "https://slack.com/api/users.profile.set")) {
        https.addHeader("Content-Type", "application/json");
        String auth = "Bearer ";
        auth += SLACK_USER_TOKEN;
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

void setup() {
  Serial.begin(115200);
  delay(1000);

  bootCount = (bootCount + 1) % 2;
  Serial.println("Boot number:" + String(bootCount));

  print_wakeup_reason();

  esp_deep_sleep_enable_gpio_wakeup(BIT(D1), ESP_GPIO_WAKEUP_GPIO_LOW);

  if (bootCount) {
    do_post("{\"profile\":{\"status_text\":\"riding a train\",\"status_emoji\":\":train:\"}}");
    Serial.println("slack status was set");
  } else {
    do_post("{\"profile\":{\"status_text\":\"\",\"status_emoji\":\"\"}}");
    Serial.println("slack status was cleared");
  }

  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
}

void loop() {
}
