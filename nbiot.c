#include "Arduino.h"
#include "BUT_NB_IoT_lib.h"

#define BUFFER_SIZE 513
#define APN_NAME (char *) "lpwa.vodafone.iot"
#define REMOTE_IP (char *) "147.229.146.40"
#define REMOTE_PORT 65007

#define OPERATOR_NUMBER (char *) "23003"

HardwareSerial &debugSerial = Serial;
HardwareSerial &saraSerial = Serial2;

char nbiotBuffer[BUFFER_SIZE] = {0};
uint8_t started = 0;

but_nb_iot nbiot(saraSerial, debugSerial, nbiotBuffer, BUFFER_SIZE);
 
void testCv5(uint8_t sock);
void testCv6(uint8_t sock);
void testCv7();
void testCv8(uint8_t sock);
void testCv9(uint8_t sock);

void setup() {
  debugSerial.begin(9600);
  saraSerial.begin(9600);
  delay(1000);

  /*1. Nastavte vývojovou desku, aby byl aktivní komunikační modul pro technologii NB-IoT.*/
  /*2. S využitím knihovny dostupné v online kurzu k předmětu IOT vytvořte program, který bude
  umožňovat automatické připojení modulu do mobilní sítě.*/
  /*3. Z dokumentace dostupné na stránkách výrobce modulu či v online kurzu předmětu nastudujte
  možnosti přenosu dat pomocí technologie NB-IoT. Zaměřte se zejména na možnosti přenosu
  z různými příznaky (flags).*/

  nbiot.rebootModule();
  delay(3000);

  nbiot.setRadioActive(true);
  nbiot.setOperator(OPERATOR_NUMBER);
  delay(2000);

  while (!nbiot.isOperatorSet(OPERATOR_NUMBER)) {
    delay(100);
  }

  nbiot.setAPNName(APN_NAME);

  while(!nbiot.isAttached()) {
    delay(100);
  }

  uint8_t sock = 0;
  if (!nbiot.openUDPSocket(REMOTE_PORT, &sock)) {
    debugSerial.println("Failed to create socket!");
  }

  /* 4. Otestujte funkčnost odesílání zpráv ve směru uplink. V průběhu měření vyzkoušejte 
   * přenos zpráv s odlišnou velikostí i příznaky (0x000, 0x200).
   * 0x000: no flags are set
   * 0x100: exception message. Send message with high priority
   * 0x200: release indicator. Indicate release after next message
   * 0x400: release indicator. Indicate release after next message has been replied to
   * */

  /*5. Odeslání opakujte desetkrát pro velikosti zprávy 12, 64, 128, 256 a 450 bajtů. Ze získaných dat
  spočítejte úspěšnost přenosu ve směru uplink. Pro každou odeslanou zprávu také zjistěte
  použitý vysílací výkon, úroveň přijatého signálu, RSSI (Received Signal Strength Indicator) a
  SNR (Signal-to-Noise Ratio).*/
  //
  //testCv5(sock);
  //
  
  /*6. Stejnou sadu měření zopakujte také pro obousměrný přenos zpráv (uplink + downlink). Pro
  obousměrný přenos také spočítejte zpoždění přenosu zprávy RTT (Round-Trip Time).*/
  //
  //testCv6(sock);
  //

  /*7. Pro měření zpoždění vyzkoušejte také nástroj ping (Packet InterNet Groper). Použijte rozsah
  velikostí zpráv z bodu 5. Výsledky porovnejte s časy získanými v předchozím bodě.*/
  //testCv7();

  /*8. Prozkoumejte, jak se liší doba přechodu z aktivního režimu do režimu eDRX při použití různých
  příznaků při přenosu zprávy.*/
  //testCv8(sock);

  /*9. Na komunikačním modulu povolte přechod do PSM (Power Saving Mode) režimu a otestujte
  funkčnost automatického přechodu do režimu spánku. Přechod do režimu spánku otestujte
  pro hodnotu aktivního časovače (T3324 – GPRS Timer 2) 6 s, 14 s a 30 s. Zaměřte se zejména
  na rozdíly přechodu do režimu spánku po odeslání zprávy při použití různých příznaků.*/
  testCv9(sock);

  /*10. Proveďte odregistrování zařízení ze sítě, odpojte měřící přípravek od napájení a ukliďte
  pracoviště.*/

}

void testCv9(uint8_t sock)
{
  char active_timer[] = "00000001";
  char periodic_timer[] = "00000001";
  
  char cmd1[] = "AT+NPSMR=1";
  nbiot.sendRawCommand(cmd1);
  delay(1000);
  int msg_size = 32;

  char msg[50];
  for (int k = 0; k < msg_size; k++) {
    msg[k] = random('a', 'z');
  }
  msg[msg_size] = '\0';

  char flag[] = "0x000";
  nbiot.sendMessage(msg, REMOTE_IP, REMOTE_PORT, flag, sock);
  nbiot.enablePSM(active_timer, periodic_timer);

  uint32_t to = 10000;
  char res[450];
  unsigned long t1 = millis();
  do  {
    nbiot.readResponse(res, to);
  } while (strstr(res, "+CSCON: 0") == 0);

  do  {
    nbiot.readResponse(res, to);
  } while (strstr(res, "+NPSMR: 1") == 0);
  unsigned long t2 = millis();

  debugSerial.print("t: ");
  debugSerial.println(t2-t1);
}

/*
// 0x200
+CSCON: 0

RESPONSE:
+CSCON: 0

t: 582

// 0x000
+CSCON: 0

RESPONSE: 
+CSCON: 0

t: 21536
*/
void testCv8(uint8_t sock)
{
  char cmd1[] = "AT+CSCON=1";
  nbiot.sendRawCommand(cmd1);
  delay(1000);

  int msg_size = 32;
  uint32_t timeout = 25000;

  char msg[50];
  for (int k = 0; k < msg_size; k++) {
    msg[k] = random('a', 'z');
  }
  msg[msg_size] = '\0';

  char flag[] = "0x000";
  unsigned long t1 = millis();
  nbiot.sendMessage(msg, REMOTE_IP, REMOTE_PORT, flag, sock);
  char res[450];
  
  do  {
    nbiot.readResponse(res, timeout);
  } while (strstr(res, "+CSCON: 0") == 0);

  unsigned long t2 = millis();
  debugSerial.print("RESPONSE: ");
  debugSerial.println(res);
  debugSerial.print("t: ");
  debugSerial.println(t2-t1);
}

/*
  Iteration: 0
  RTT:512
  Iteration: 1
  RTT:1163
  Iteration: 2
  RTT:1709
  Iteration: 3
  RTT:2120
  Iteration: 4
  RTT:3234
*/
void testCv6(uint8_t sock)
{
  int msg_size[] = {12, 64, 128, 256, 450};
  unsigned long time[5];
  unsigned long repetitions = 1;

  for (int j = 0; j < 5; j++) {
    char msg[450];
    char msg_recv[460];

    for (int k = 0; k < msg_size[j]; k++) {
      msg[k] = random('a', 'z');
    }
    msg[msg_size[j]] = '\0';

    char flag[] = "0x000";
    for (unsigned int i = 0; i < repetitions; i++) {
      uint16_t msg_recv_size = 0;
      uint16_t msg_recv_rem_size = 0;

      uint16_t msg_size_d = 0;
      uint16_t remote_port = 0;
      uint32_t timeout = 10000;
      delay(5000);
      unsigned long t1 = millis();
      nbiot.sendMessage(msg, REMOTE_IP, REMOTE_PORT, flag, sock);
      nbiot.waitForMessage(&sock, &msg_size_d, timeout);
      unsigned long t2 = millis();
      nbiot.readMessage(msg_recv, sock, msg_size_d, REMOTE_IP, &remote_port, &msg_recv_size, &msg_recv_rem_size);
      time[j] = (t2-t1);

      debugSerial.print("Received size: ");
      debugSerial.print(msg_recv_size);
      debugSerial.print(" received remaining size: ");
      debugSerial.print(msg_recv_rem_size);
      debugSerial.print(" message: ");

      msg_recv[msg_recv_size+1] = '\0';
      debugSerial.println(msg_recv);
    }
  }

  for (int i = 0; i < 5; i++) {
    debugSerial.print("Iteration: ");
    debugSerial.println(i);
    debugSerial.print("RTT:");
    debugSerial.println(time[i] / repetitions);
  }
}

void testCv5(uint8_t sock) {
  int msg_size[] = {12, 64, 128, 256, 450};
  int rssi_arr[5];
  int snr_arr[5];
  int txpow_arr[5];

  int repetitions = 2;

  for (int j = 0; j < 5; j++) {
    char msg[450];
    rssi_arr[j] = 0;
    snr_arr[j] = 0;
    txpow_arr[j] = 0;

    for (int k = 0; k < msg_size[j]; k++) {
      msg[k] = random('a', 'z');
    }
    msg[msg_size[j]] = '\0';

    char flag[] = "0x000";

    for (int i = 0; i < repetitions; i++) {  
      nbiot.sendMessage(msg, REMOTE_IP, REMOTE_PORT, flag, sock);

      cellStats cell_stat_i;
      radioStats radio_stat_i;
      nbiot.getCellStats(&cell_stat_i);
      nbiot.getRadioStats(&radio_stat_i);

      rssi_arr[j] += cell_stat_i.RSSI;
      snr_arr[j] += radio_stat_i.SNR;
      txpow_arr[j] += radio_stat_i.txPower;
    }
  }

  for (int i = 0; i < 5; i++) {
    debugSerial.print("Iteratior ");
    debugSerial.print(i);
    debugSerial.println(" -----------------------------------------");

    debugSerial.print("Cell stat: RSSI: ");
    debugSerial.println(rssi_arr[i] / (10.f * repetitions));

    debugSerial.print("Radio stat: SNR: ");
    debugSerial.println(snr_arr[i] / (10.f * repetitions));

    debugSerial.print("Radio stat: TX Power: ");
    debugSerial.println(txpow_arr[i] / (10.f * repetitions));
  }
}

/*
TTL: 39
RTT: 552
IP: 147.229.146.40

TTL: 39
RTT: 890
IP: 147.229.146.40

TTL: 39
RTT: 847
IP: 147.229.146.40

TTL: 39
RTT: 1137
IP: 147.229.146.40

TTL: 39
RTT: 1054
IP: 147.229.146.40

*/
void testCv7()
{
  uint16_t msg_size[] = {12, 64, 128, 256, 450};
  uint16_t timeout = 10000;

  pingStats ps;
  for (int i = 0; i < 5; i++) {
    uint16_t pkt_size = msg_size[i];

    nbiot.pingToRemoteHost(REMOTE_IP, pkt_size, timeout, &ps);
    debugSerial.print("TTL: ");
    debugSerial.println(ps.ttl);
    debugSerial.print("RTT: ");
    debugSerial.println(ps.rtt);
    debugSerial.print("IP: ");
    debugSerial.println(ps.remoteIP);
  }
}

void loop() {
    if(started == 0){
        

        started = 1;
      }
}
