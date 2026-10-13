#include <Arduino.h>
#include <DHT.h>
#define DHT_PIN 15
#define BUZZER_PIN 25

DHT dht(DHT_PIN, DHT11);
QueueHandle_t displayQ, alertQ;
struct Data { float temp, hum; };

void sensor(void *p) {
  Data d;
  while (1) {
    d = {dht.readTemperature(), dht.readHumidity()};
    if (!isnan(d.temp) && !isnan(d.hum)) {
      xQueueOverwrite(displayQ, &d);
      xQueueOverwrite(alertQ, &d);
    }
    vTaskDelay(pdMS_TO_TICKS(2500));
  }
}

void display(void *p) {
  Data d;
  while (1)
    if (xQueueReceive(displayQ, &d, portMAX_DELAY))
      Serial.printf("Temp: %.1f C | Hum: %.1f%%\n", d.temp, d.hum);
}

void alert(void *p) {
  Data d;
  bool alarm = false;
  while (1)
    if (xQueueReceive(alertQ, &d, portMAX_DELAY)) {
      if (d.temp > 30) alarm = true;
      if (d.temp < 29) alarm = false;
      digitalWrite(BUZZER_PIN, alarm);
    }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  dht.begin();
  displayQ = xQueueCreate(1, sizeof(Data));
  alertQ = xQueueCreate(1, sizeof(Data));
  xTaskCreate(sensor, "Sensor", 2048, NULL, 2, NULL);
  xTaskCreate(display, "Display", 2048, NULL, 1, NULL);
  xTaskCreate(alert, "Alert", 2048, NULL, 3, NULL);
}

void loop() { vTaskDelay(pdMS_TO_TICKS(1000)); }
