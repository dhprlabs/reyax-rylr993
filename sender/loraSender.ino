#define LORA_RX_PIN    16   // RX2
#define LORA_TX_PIN    17   // TX2
#define LORA_BAUDRATE 9600

HardwareSerial LoraSerial(2);

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 5000;
const int  targetAddress   = 2;
const char* messageToSend  = "Hi there !!";

void setup() {
  Serial.begin(115200);
  delay(10);

  // --- init LoRa UART on Serial2 ---
  LoraSerial.begin(LORA_BAUDRATE, SERIAL_8N1, LORA_RX_PIN, LORA_TX_PIN);
  delay(100);

  // --- configure LoRa module & print out each response ---
  sendAT("AT+OPMODE=1",    500);
  sendAT("ATZ",           2000);
  sendAT("AT+ADDRESS=100", 500);
  sendAT("AT+BAND=865000000", 500);
  sendAT("AT+PARAMETER=9,7,1,12", 500);

  Serial.println("LoRa Transmitter Ready!");
}

void loop() {
  unsigned long now = millis();

  // 1) periodic send
  if (now - lastSendTime >= sendInterval) {
    sendData(targetAddress, messageToSend);
    lastSendTime = now;
  }

  // 2) manual send via Serial console (addr#msg)
  if (Serial.available()) {
    String in = Serial.readStringUntil('\n');
    in.trim();
    int sep = in.indexOf('#');
    if (sep > 0) {
      int addr = in.substring(0, sep).toInt();
      String msg = in.substring(sep + 1);
      sendData(addr, msg.c_str());
    } else {
      Serial.println("Use: ADDRESS#MESSAGE");
    }
  }

  // 3) print any LoRa‑module‐side unsolicited messages
  while (LoraSerial.available()) {
    String resp = LoraSerial.readStringUntil('\n');
    resp.trim();
    if (resp.length()) {
      Serial.print("[LoRa] ");
      Serial.println(resp);
    }
  }
}

// helper to send AT and immediately dump the OK/Error
void sendAT(const char* cmd, int delayMs) {
  LoraSerial.print(cmd);
  LoraSerial.print("\r\n");
  Serial.printf("[AT] %s\n", cmd);
  delay(delayMs);

  while (LoraSerial.available()) {
    String r = LoraSerial.readStringUntil('\n');
    r.trim();
    if (r.length()) {
      Serial.print("  ↳ ");
      Serial.println(r);
    }
  }
}

void sendData(int address, const char* data) {
  int len = strlen(data);
  LoraSerial.printf("AT+SEND=%d,%d,%s\r\n", address, len, data);
  Serial.printf("[SEND] → addr=%d len=%d msg=\"%s\"\n", address, len, data);
}
