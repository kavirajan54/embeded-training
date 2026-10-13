HardwareSerial UART2(2);

#define UART_RX 16
#define UART_TX 17

void setup()
{
  Serial.begin(115200);

  UART2.begin(
    9600,
    SERIAL_8N1,
    UART_RX,
    UART_TX
  );

  Serial.println("UART Loopback Test");
  Serial.println("Type something in Serial Monitor...");
}

void loop()
{
  if (Serial.available())
  {
    String message = Serial.readStringUntil('\n');
    message.trim();

    Serial.print("Sending through UART2: ");
    Serial.println(message);

    UART2.println(message);
  }

  if (UART2.available())
  {
    String received = UART2.readStringUntil('\n');
    received.trim();

    Serial.print("UART2 Received: ");
    Serial.println(received);
  }
}
