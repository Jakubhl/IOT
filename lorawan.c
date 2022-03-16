#include <Arduino.h>
#include "BUT_LoRa_lib.h"

#define BUFFER_SIZE 500

#define DEVADDR (char *) "2601183E"
#define DEVEUI  (char *) "70B3D57ED004DF1A"
#define NWKSKEY (char *) "D0CD473C7B0B7C34C8CADFC3851BF5F4"
#define APPSKEY (char *) "A4D2B3F7009BFF10375EB5BF195A3C41"


HardwareSerial &debugSerial = Serial;
HardwareSerial &loraSerial = Serial3;

char loraBuffer[BUFFER_SIZE] = {0};
uint8_t started = 0;

but_lora lora(loraSerial, debugSerial, loraBuffer, BUFFER_SIZE);

void setup() {
  debugSerial.begin(9600);
  loraSerial.begin(57600);
  delay(3000);

  lora.macReset(868);
  lora.macSetDevAddr(DEVADDR);
  lora.macSetDevEUI(DEVEUI);
  lora.macSetNetworkKey(NWKSKEY);
  lora.macSetApplicatioSessionKey(APPSKEY);

  lora.macSetAdaptiveDataRate(false);
  lora.macSetAutomaticReply(false);
  lora.macSetPowerIndex(PWRIDX_5);
  lora.macSetChannelFrequency(CHANNEL_7, 867900000);

  //lora.radioSetFrequency(uint32_t(8685*100000));
  //lora.radioSetSpreadingFactor(SF_7);
  //lora.radioSetWatchdog(456416546+1);
  delay(1000);
}

void loop() {
  if(started == 0) {


    /*char data[512];
    lora.radioRx(0, data);
    debugSerial.print("data: ");
    debugSerial.println(data);
    debugSerial.println("=============== Signal properties =============");
    lora.radioGetSNR();
    lora.radioGetBandWidth();*/
    //started = 1;
  }
}
/*1. Upravte nastavení měřícího přípravku, aby byl aktivní modul pro technologii LoRaWAN.
2. S využitím knihovny dostupné v online kurzu k předmětu IOT vytvořte program, který bude
umožňovat automatické měření parametrů fyzické a MAC vrstvy, tzn. LoRa respektive
LoRaWAN.*/

/*3. Otestujte přenos dat na fyzické vrstvě technologie LoRa. Z přijatých dat zjistěte hodnotu SNR
(Signal to Noise Ratio) a také dobu vysílání zprávy. Měření proveďte pro všechny hodnoty SF
(Spreading Factor) jež jsou určeny pro Evropu. Vyzkoušejte také jaký vliv má šířka pásma na
výslednou dobu vysílání a hodnotu SNR. Otestujte šířky pásma 125 kHz a 250 kHz. Velikost
zprávy ponechte na hodnotě 64 B pro všechna měření.
// vysledky
// BW[kHz], Time[ms]
// 125 -> 240, 338, 512, 2020, 1682, 2914
// 250 -> 180, 230, 317, 471,   779, 1518

4. Pro vámi zvolenou šířku pásma změřte vliv vysílacího výkonu na odstup signálu od šumu.
Měření provádějte pro vysílací výkon -3 dBm až 14 dBm a SF7.
// -3 dBm  -> 6 db
// -2 dBm  -> 6 db
// -1 dBm  -> 6 db
//  0 dBm  -> 6 db
//  14 dBm -> 6 db

5. Zjistěte hodnotu HW EUI komunikačního modulu a na základě tohoto údaje nalezněte
v tabulce příslušné klíče nutné pro připojení do sítě.
6. Proveďte připojení do sítě za pomocí metody ABP (Activation By Personalization).
7. Dle regulí platných pro evropské ISM (Industrial, Scientific and Medical) pásmo nastavte
frekvenční plán pro všech 8 komunikačních kanálů.
8. Připojte se na vzdálený server a spusťte aplikaci pro připojení do platformy The Things
Network.
9. Proveďte měření přenosu zpráv ve směru uplink pro všechny přenosové rychlosti (DR0 – DR5).
• Použijte zprávu o velikosti 12 B z důvodu limitace ze strany duty cycle, z tohoto
důvodu musíte zvolit i vhodnou dobu periody zpráv.
• Měření proveďte pro potvrzovaný i nepotvrzovaný mód přenosu, porovnejte rozdíly
v době přenosu zprávy.
• Na straně serveru zjistěte také úroveň RSSI (Received Signal Strength Indicator) a SNR
pro všechny rychlostní módy.
• U každé zprávy porovnejte, zda doba přenosu zprávy (Airtime) odpovídá výpočtu
podle vzorce.
10. Ukončete serverovou aplikaci, odpojte přípravek od napájení a ukliďte pracoviště.*/
