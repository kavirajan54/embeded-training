#include <SPI.h>
#include <LoRa.h>

#define SS_PIN   5
#define RST_PIN  14
#define DIO0_PIN 26

void setup() {
  Serial.begin(115200);

  LoRa.setPins(SS_PIN, RST_PIN, DIO0_PIN);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa initialization failed!");
    while (true);
  }

  Serial.println("LoRa Sender Ready");
}

void loop() {
  String message = "Hello Receiver";

  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();

  Serial.println("Sent: " + message);

  delay(2000);
}



























receiver code:

#include <SPI.h>
#include <LoRa.h>

#define SS_PIN   5
#define RST_PIN  14
#define DIO0_PIN 26

void setup() {
  Serial.begin(115200);

  LoRa.setPins(SS_PIN, RST_PIN, DIO0_PIN);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa initialization failed!");
    while (true);
  }

  Serial.println("LoRa Receiver Ready");
}

void loop() {
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    String message = "";

    while (LoRa.available()) {
      message += (char)LoRa.read();
    }

    Serial.println("Received: " + message);
  }
}
