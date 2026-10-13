#include <Arduino.h>
#include <DHT.h>
#include "AdafruitIO_WiFi.h"

#define IO_USERNAME "YOUR_USERNAME"
#define IO_KEY      "YOUR_IO_KEY"
#define WIFI_SSID   "YOUR_WIFI_NAME"
#define WIFI_PASS   "YOUR_WIFI_PASSWORD"

#define DHT_PIN 15
#define BUZZER_PIN 25

DHT dht(DHT_PIN, DHT11);

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY,
                   WIFI_SSID, WIFI_PASS);

AdafruitIO_Feed *temperatureFeed = io.feed("temperature");
AdafruitIO_Feed *humidityFeed = io.feed("humidity");
AdafruitIO_Feed *buzzerFeed = io.feed("buzzer");

bool buzzerState = false;
unsigned long lastSend = 0;

void handleBuzzer(AdafruitIO_Data *data) {
  buzzerState = data->toInt() == 1;
  digitalWrite(BUZZER_PIN, buzzerState ? HIGH : LOW);

  Serial.print("Buzzer: ");
  Serial.println(buzzerState ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  dht.begin();
  buzzerFeed->onMessage(handleBuzzer);

  Serial.println("Connecting to Adafruit IO...");
  io.connect();

  while (io.status() < AIO_CONNECTED) {
    io.run();
    delay(500);
  }

  buzzerFeed->get();
  Serial.println("Adafruit IO Connected");
}

void loop() {
  io.run();

  if (millis() - lastSend >= 10000) {
    lastSend = millis();

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (!isnan(temp) && !isnan(hum)) {
      Serial.printf("Temperature: %.1f C\n", temp);
      Serial.printf("Humidity: %.1f %%\n", hum);

      temperatureFeed->save(temp);
      humidityFeed->save(hum);
    } else {
      Serial.println("DHT11 read failed");
    }
  }
}
