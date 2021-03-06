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
  lora.macSetPowerIndex(PWRIDX_1);
  //lora.macSetChannelFrequency(CHANNEL_3, 867100000);

  lora.setFrequencyPlan(BUT_DEFAULT);

  lora.macSetChannelStatus(CHANNEL_0, true);
  //lora.macSetChannelDutyCycle(CHANNEL_3, 99);
  lora.macSetChannelStatus(CHANNEL_1, true);
  lora.macSetChannelStatus(CHANNEL_2, true);

  lora.macSetChannelDataRateRange(CHANNEL_0, 0, 5);
  lora.macSetChannelDataRateRange(CHANNEL_1, 0, 5);
  lora.macSetChannelDataRateRange(CHANNEL_2, 0, 5);

  lora.macSetUplinkCounter(209);
  //lora.macSetRxWindow2DRFreq(DR_0, 869525000);

  lora.macSave();
  lora.macJoin(ABP_JOIN);

  //lora.radioSetFrequency(uint32_t(8685*100000));
  //lora.radioSetSpreadingFactor(SF_7);
  //lora.radioSetWatchdog(456416546+1);
  delay(1000);
}

void loop() {
  if(started == 0) {
    char data[] = "Hello World!";

    // DR0 - 3761 ms
    // DR1 - 3339 ms
    // DR2 - 2710 ms
    // DR3 - 2506 ms
    // DR4 - 2412 ms
    // DR5 - 1278 ms
    int min_cnt = 226;
    for (int i = 0; i <= 5; i++) {
      lora.macSetUplinkCounter(min_cnt+i);
      lora.macSetDataRate((DataRate)i);

      unsigned long t1=millis();
      lora.macTx(false, 99, data);
      lora.macWaitForMessage(5000);
      unsigned long t2=millis();
    
      debugSerial.print("DR:");
      debugSerial.println(i);
      debugSerial.print("Time [ms]: ");
      debugSerial.println(t2-t1);
      delay(20000);
    }
    /*lora.radioRx(0, data);
    debugSerial.print("data: ");
    debugSerial.println(data);
    debugSerial.println("=============== Signal properties =============");
    lora.radioGetSNR();
    lora.radioGetBandWidth();*/
    started = 1;
  }
}
/*1. Upravte nastaven?? m??????c??ho p????pravku, aby byl aktivn?? modul pro technologii LoRaWAN.
2. S vyu??it??m knihovny dostupn?? v online kurzu k p??edm??tu IOT vytvo??te program, kter?? bude
umo????ovat automatick?? m????en?? parametr?? fyzick?? a MAC vrstvy, tzn. LoRa respektive
LoRaWAN.*/

/*3. Otestujte p??enos dat na fyzick?? vrstv?? technologie LoRa. Z p??ijat??ch dat zjist??te hodnotu SNR
(Signal to Noise Ratio) a tak?? dobu vys??l??n?? zpr??vy. M????en?? prove??te pro v??echny hodnoty SF
(Spreading Factor) je?? jsou ur??eny pro Evropu. Vyzkou??ejte tak?? jak?? vliv m?? ??????ka p??sma na
v??slednou dobu vys??l??n?? a hodnotu SNR. Otestujte ??????ky p??sma 125 kHz a 250 kHz. Velikost
zpr??vy ponechte na hodnot?? 64 B pro v??echna m????en??.
// vysledky
// BW[kHz], Time[ms]
// 125 -> 240, 338, 512, 2020, 1682, 2914
// 250 -> 180, 230, 317, 471,   779, 1518
4. Pro v??mi zvolenou ??????ku p??sma zm????te vliv vys??lac??ho v??konu na odstup sign??lu od ??umu.
M????en?? prov??d??jte pro vys??lac?? v??kon -3 dBm a?? 14 dBm a SF7.
// -3 dBm  -> 6 db
// -2 dBm  -> 6 db
// -1 dBm  -> 6 db
//  0 dBm  -> 6 db
//  14 dBm -> 6 db
5. Zjist??te hodnotu HW EUI komunika??n??ho modulu a na z??klad?? tohoto ??daje nalezn??te
v tabulce p????slu??n?? kl????e nutn?? pro p??ipojen?? do s??t??.
6. Prove??te p??ipojen?? do s??t?? za pomoc?? metody ABP (Activation By Personalization).
7. Dle regul?? platn??ch pro evropsk?? ISM (Industrial, Scientific and Medical) p??smo nastavte
frekven??n?? pl??n pro v??ech 8 komunika??n??ch kan??l??.
8. P??ipojte se na vzd??len?? server a spus??te aplikaci pro p??ipojen?? do platformy The Things
Network.
9. Prove??te m????en?? p??enosu zpr??v ve sm??ru uplink pro v??echny p??enosov?? rychlosti (DR0 ??? DR5).
??? Pou??ijte zpr??vu o velikosti 12 B z d??vodu limitace ze strany duty cycle, z tohoto
d??vodu mus??te zvolit i vhodnou dobu periody zpr??v.
??? M????en?? prove??te pro potvrzovan?? i nepotvrzovan?? m??d p??enosu, porovnejte rozd??ly
v dob?? p??enosu zpr??vy.
??? Na stran?? serveru zjist??te tak?? ??rove?? RSSI (Received Signal Strength Indicator) a SNR
pro v??echny rychlostn?? m??dy.
??? U ka??d?? zpr??vy porovnejte, zda doba p??enosu zpr??vy (Airtime) odpov??d?? v??po??tu
podle vzorce.
10. Ukon??ete serverovou aplikaci, odpojte p????pravek od nap??jen?? a ukli??te pracovi??t??.*/
