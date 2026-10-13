#include <SPI.h>
#include <mcp2515.h>

#define CAN_CS 5

MCP2515 mcp2515(CAN_CS);
struct can_frame canMsg;

void setup() {
  Serial.begin(115200);
  SPI.begin(18, 19, 23, CAN_CS);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  Serial.println("CAN Transmitter Ready");
  Serial.println("Type a message and press ENTER:");
}

void loop() {
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    message.trim();

    if (message.length() == 0) return;

    canMsg.can_id = 0x100;
    canMsg.can_dlc = min((int)message.length(), 8);

    for (int i = 0; i < canMsg.can_dlc; i++) {
      canMsg.data[i] = message[i];
    }

    if (mcp2515.sendMessage(&canMsg) == MCP2515::ERROR_OK) {
      Serial.print("Sent: ");
      Serial.println(message.substring(0, 8));
    } else {
      Serial.println("CAN SEND FAILED");
    }
  }
}


reciver code:
#include <SPI.h>
#include <mcp2515.h>

#define CAN_CS 5

MCP2515 mcp2515(CAN_CS);
struct can_frame canMsg;

void setup() {
  Serial.begin(115200);
  SPI.begin(18, 19, 23, CAN_CS);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  Serial.println("CAN Receiver Ready");
  Serial.println("Waiting for messages...");
}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    if (canMsg.can_id == 0x100) {
      Serial.print("CAN ID: 0x");
      Serial.println(canMsg.can_id, HEX);

      Serial.print("DLC: ");
      Serial.println(canMsg.can_dlc);

      Serial.print("Message: ");

      for (int i = 0; i < canMsg.can_dlc; i++) {
        Serial.print((char)canMsg.data[i]);
      }

      Serial.println();
      Serial.print("HEX DATA: ");

      for (int i = 0; i < canMsg.can_dlc; i++) {
        Serial.printf("%02X ", canMsg.data[i]);
      }

      Serial.println();
    }
  }
}
