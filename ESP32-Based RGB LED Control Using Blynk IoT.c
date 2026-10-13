#define BLYNK_TEMPLATE_ID "YOUR TEMP ID"
#define BLYNK_TEMPLATE_NAME "YOUR TEMP  NAME"
#define BLYNK_AUTH_TOKEN "YOUR TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

const char* ssid = "Impact's";
const char* password = "youtube124";

#define C1_PIN 25
#define C2_PIN 26
#define C3_PIN 27

BLYNK_WRITE(V0)
{
  int state = param.asInt();
  digitalWrite(C1_PIN, state);
  Serial.println(state ? "RED ON" : "RED OFF");
}

BLYNK_WRITE(V1)
{
  int state = param.asInt();
  digitalWrite(C2_PIN, state);
  Serial.println(state ? "GREEN ON" : "GREEN OFF");
}

BLYNK_WRITE(V2)
{
  int state = param.asInt();
  digitalWrite(C3_PIN, state);
  Serial.println(state ? "BLUE ON" : "BLUE OFF");
}

void setup()
{
  Serial.begin(115200);

  pinMode(C1_PIN, OUTPUT);
  pinMode(C2_PIN, OUTPUT);
  pinMode(C3_PIN, OUTPUT);

  digitalWrite(C1_PIN, LOW);
  digitalWrite(C2_PIN, LOW);
  digitalWrite(C3_PIN, LOW);

  Serial.println("Connecting to WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected!");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

void loop()
{
  Blynk.run();
}
