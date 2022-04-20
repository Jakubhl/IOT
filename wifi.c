#include <Arduino.h>
#include <BUT_ESP32_lib.h>
#include <BUT_Devkit_lib.h>

#define BUFFER_SIZE 512

HardwareSerial &debugSerial = Serial;
HardwareSerial &espSerial = Serial1;
char espBuffer[BUFFER_SIZE] = {0};
uint8_t started = 0;

char ssid[] = "Home40";
char password[] = "FritzBox2";

but_esp esp(espSerial, debugSerial, espBuffer, BUFFER_SIZE);
but_devkit devkit(debugSerial);

void setup() {
  debugSerial.begin(9600);
  espSerial.begin(115200);
  
  devkit.initModules();

  for (uint8_t i = 0; i < 6; i++){
		devkit.setLedState(LED_1, devkit.getLedState(LED_1)?OFF:ON);
		devkit.setLedState(LED_2, devkit.getLedState(LED_2)?OFF:ON);
		delay(250);
	}

  esp.initESP();
  delay(500);
  esp.setWiFiMode(WiFiModes::STATION);
  delay(500);
  esp.connectToAP(ssid, password);
  delay(5000);

  char ip[64];
  esp.getStationIP(ip);
  debugSerial.print("Station IP: ");
  debugSerial.println(ip);
  delay(100);

  char clientID[] = "mqttx_4fc2d264";
  char userName[] = "student";
  char password[] = "student";
  esp.mqttConfigureUser(clientID, userName, password);

  char remoteIP[] = "192.168.4.155";
  uint16_t port = 1883;
  esp.mqttConnect(remoteIP, port);

  char topic[] = "led";
  esp.mqttSubscribe(topic);
}

void loop() {
  uint8_t closed = 0;

  while (!closed) {
    
    if(devkit.getButtonState(BUTTON_1) == true){
      char topic[] = "button";
      char message[] = "button1";
      esp.mqttPublish(topic, message);
      delay(1000);
		}
		
    if(devkit.getButtonState(BUTTON_2) == true){
      char topic[] = "button";
      char message[] = "button2";
      esp.mqttPublish(topic, message);
      delay(1000);
    }

    uint32_t timeout = 10000;
    char recv_msg[128];
    uint16_t len = 0;
    char topic[] = "led";
    esp.mqttWaitForMessage(timeout, topic, recv_msg, &len);
    if (len == 0) {
      esp.mqttClose();
      closed = 1;
    }
  }
}

void testTCP_UDP(void)
{
  char remoteIP[] = "192.168.4.155";
  uint16_t remotePort = 8080;
  uint16_t localPort = 1234;
  uint16_t keepAlive = 10;
  //esp.openTCPConnection(remoteIP, remotePort, keepAlive);
  esp.openUDPConnection(remoteIP, remotePort, localPort);
  delay(10);

  char data[513];
  //2, 64, 128, 256 a 512 B
  int16_t msg_size = 256;
  for (int16_t k = 0; k < msg_size; k++) {
    data[k] = random('a', 'z');
  }
  data[msg_size] = '\0';

  uint16_t data_len = strlen(data);
  char recv_msg[256];
  uint32_t timeout = 10000;
  
  esp.sendData(data, data_len);
  esp.waitForMessage(timeout, recv_msg);
  debugSerial.print("Received message: ");
  debugSerial.println(recv_msg);

  delay(1000);
  esp.sendData(data, data_len);
  esp.waitForMessage(timeout, recv_msg);
  debugSerial.print("Received message: ");
  debugSerial.println(recv_msg);
  
  esp.closeConnection();
}
