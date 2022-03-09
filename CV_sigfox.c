#include <Arduino.h>
#include "BUT_Sigfox_lib.h"

// static constants
#define BUFFER_SIZE 255
#define RX_PIN 69
#define TX_PIN 2

// serial interfaces definitions
HardwareSerial &debugSerial = Serial;
SoftwareSerial sigfoxSerial = SoftwareSerial(RX_PIN, TX_PIN);

/* input serial buffer */
char sigfoxBuffer[BUFFER_SIZE] = {0};

// 0x557725447725335544772533
byte key[] = {0x55, 0x77,  0x25, 0x44, 0x77,  0x25, 0x33, 0x55, 0x44, 0x77,  0x25, 0x33};

/* status flag to indicate only-once run code */ 
uint8_t started = 0;

// instance creation of sigfox library
but_sigfox sigfox(sigfoxSerial, debugSerial, sigfoxBuffer, BUFFER_SIZE);

void vernamCipher(byte message[], byte key[], byte encMessage[], size_t len);
void encodeValue(byte *res, float val);

void encodeValue(byte *res, float val)
{
  int _val = (int)(val*100);
  res[1] = _val & 0xff;
  res[0] = _val >> 8 & 0xff;
}

void vernamCipher(byte message[], byte key[], byte encMessage[], size_t len)
{
  for (int i = 0; i < len; i++) {
    encMessage[i] = message[i] ^ key[i];
  }
}

void setup() {
  // set-up Serial ports
  debugSerial.begin(9600);
  sigfoxSerial.begin(19200);

  // delay for correct power-up of Sigfox module
  delay(1500);

  // test basic commands
  sigfox.enterConfigMode();
  sigfox.getLibVersion();

  // put your setup code here, to run once:
  sigfox.setRepetitionDelay(50);
  sigfox.setTxPower(14.f);

  byte dat[2];
  /*strncpy(dat, "Hello World!", 12+1);
  debugSerial.println("Sending...");
  sigfox.sendData(dat, false);
  debugSerial.println("Done...");*/

  //encodeValue(dat, 5.25);
  //sigfox.sendRawData(dat, 2, false);

  byte msg[] = "Hello World!";
  //vernamCipher(dat, key, "Hello World!", 2);
  vernamCipher(msg, key, msg, 12);
  sigfox.sendRawData(msg, 12, false);
}

void loop() {
  /*char id[10];
  sigfox.getModuleID(id);
  debugSerial.print("ID: ");
  debugSerial.print(id);
  debugSerial.println("");
  delay(5000);*/

  //1. Upravte nastavení měřícího přípravku, aby byl aktivní modul pro technologii Sigfox.
  //2. S využitím knihovny dostupné v online kurzu k předmětu IOT vytvořte program, který bude
  // umožňovat odesílání, šifrování a kódování zpráv pro technologii Sigfox.
  //3. Otestujte funkčnost odesílání nepotvrzovaných zpráv skrze technologii Sigfox pro velikosti 2 a 12 B. - DONE

  // 4. Pro maximální vysílací výkon a zprávu o velikosti 12 B otestujte přenos zpráv s různou
  // periodou repetic. Měření proveďte pro hodnoty 50, 500, 1000 a 2000 ms. Pozorujte, jaký má
  // hodnota periody vliv na přenos zpoždění.
  // Odosielanie trvalo dlhsie -- cca 11s pre 2000ms a cca 1.5s pre 50ms

  // 5. Vytvořte funkci encodeValue umožňující přenos číselných hodnot v hexadecimálním tvaru
  // bez převodu do Tvaru ASCII.
  /*6. Pro zajištění bezpečnosti přenosu vytvořte funkci vernamCipher, která zprávu v bajtové
  podobě zakóduje pomocí klíče k. Výstupem funkce bude kryptogram zprávy, který bude
  přenesen technologií Sigfox a v koncové aplikaci bude pomocí klíče k převeden zpět na zprávu.
  7. Funkčnost vytvořených funkcí bude ověřena na vzdáleném serveru úspěšným dekódováním
  zprávy.
  8. Ukončete serverovou aplikaci, odpojte přípravek od napájení a ukliďte pracoviště.*/
  // put your main code here, to run repeatedly:
}
