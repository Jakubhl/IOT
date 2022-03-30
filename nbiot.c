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

void setup() {
  debugSerial.begin(9600);
  saraSerial.begin(9600);
  delay(2000);

  /*1. Nastavte vývojovou desku, aby byl aktivní komunikační modul pro technologii NB-IoT.*/
  /*2. S využitím knihovny dostupné v online kurzu k předmětu IOT vytvořte program, který bude
  umožňovat automatické připojení modulu do mobilní sítě.*/
  /*3. Z dokumentace dostupné na stránkách výrobce modulu či v online kurzu předmětu nastudujte
  možnosti přenosu dat pomocí technologie NB-IoT. Zaměřte se zejména na možnosti přenosu
  z různými příznaky (flags).*/

  nbiot.rebootModule();
  delay(5000);

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
  testCv6(sock);
  //

  /*7. Pro měření zpoždění vyzkoušejte také nástroj ping (Packet InterNet Groper). Použijte rozsah
  velikostí zpráv z bodu 5. Výsledky porovnejte s časy získanými v předchozím bodě.*/

  /*8. Prozkoumejte, jak se liší doba přechodu z aktivního režimu do režimu eDRX při použití různých
  příznaků při přenosu zprávy.*/

  /*9. Na komunikačním modulu povolte přechod do PSM (Power Saving Mode) režimu a otestujte
  funkčnost automatického přechodu do režimu spánku. Přechod do režimu spánku otestujte
  pro hodnotu aktivního časovače (T3324 – GPRS Timer 2) 6 s, 14 s a 30 s. Zaměřte se zejména
  na rozdíly přechodu do režimu spánku po odeslání zprávy při použití různých příznaků.*/

  /*10. Proveďte odregistrování zařízení ze sítě, odpojte měřící přípravek od napájení a ukliďte
  pracoviště.*/

}

void testCv6(uint8_t sock)
{
  int msg_size[] = {12, 64, 128, 256, 450};
  int time[5];

  for (int j = 0; j < 5; j++) {
    char msg[450];
    char msg_recv[460];

    for (int k = 0; k < msg_size[j]; k++) {
      msg[k] = random('a', 'z');
    }
    msg[msg_size[j]] = '\0';

    char flag[] = "0x000";
    for (int i = 0; i < 10; i++) {
      uint16_t msg_recv_size = 0;
      uint16_t msg_recv_rem_size = 0;

      uint16_t msg_size = 0;
      uint16_t remote_port = 0;
      uint32_t timeout = 5000;
      auto t1 = millis();
      nbiot.sendMessage(msg, REMOTE_IP, REMOTE_PORT, flag, sock);
      nbiot.waitForMessage(&sock, &msg_size, timeout);
      auto t2 = millis();
      nbiot.readMessage(msg_recv, sock, (uint16_t)strlen(msg), REMOTE_IP, &remote_port, &msg_recv_size, &msg_recv_rem_size);
      time[j] += t2-t1;

      debugSerial.print("Received size: ");
      debugSerial.print(msg_recv_size);
      debugSerial.print(" received remaining size: ");
      debugSerial.print(msg_recv_rem_size);
      debugSerial.print(" message: ");
      debugSerial.println(msg_recv);
    }
  }

  for (int i = 0; i < 5; i++) {
    debugSerial.print("Iteration: ");
    debugSerial.println(i);
    debugSerial.print("RTT:");
    debugSerial.println(time[i]);
  }
}

void testCv5(uint8_t sock) {
  int msg_size[] = {12, 64, 128, 256, 450};
  int rssi_arr[5];
  int snr_arr[5];
  int txpow_arr[5];

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

    for (int i = 0; i < 10; i++) {  
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
    debugSerial.println(rssi_arr[i] / 100.f);

    debugSerial.print("Radio stat: SNR: ");
    debugSerial.println(snr_arr[i] / 100.f);

    debugSerial.print("Radio stat: TX Power: ");
    debugSerial.println(txpow_arr[i] / 100.f);
  }
}

void loop() {
    if(started == 0){
        

        started = 1;
      }
}
