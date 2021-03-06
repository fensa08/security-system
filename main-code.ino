/***************************************************
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
int Con = 150;
LiquidCrystal lcd(12, 11, 5, 4, 10, 8);

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(2, 3);

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1

#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// ===================================================================================== setup() =====================================================================================

void setup()
{
  Serial.begin(9600);
  analogWrite(9, 500); // ova e background contrast
  analogWrite(6, Con);
  lcd.begin(16, 2);

  while (!Serial)
    ; // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1)
    {
      delay(1);
    }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x"));
  Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x"));
  Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: "));
  Serial.println(finger.capacity);
  Serial.print(F("Security level: "));
  Serial.println(finger.security_level);
  Serial.print(F("Device address: "));
  Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: "));
  Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: "));
  Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0)
  {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else
  {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains ");
    Serial.print(finger.templateCount);
    Serial.println(" templates");
  }
}

// ===================================================================================== loop() =====================================================================================

void loop() // run over and over again
{
  getFingerprintID();
  delay(2500); //don't ned to run this at full speed.

}

// ===================================================================================== gerFingerpintID() =====================================================================================
uint8_t getFingerprintID()
{
  uint8_t p = finger.getImage();
  switch (p)
  {
  case FINGERPRINT_OK:
    Serial.println("Image taken");
    break;
  case FINGERPRINT_NOFINGER:
    Serial.println("No finger detected");
    Serial.println("================================================\n\n\n");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_IMAGEFAIL:
    Serial.println("Imaging error");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p)
  {
  case FINGERPRINT_OK:
    Serial.println("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK)
  {
    Serial.println("Found a print match!");
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    Serial.println("Communication error");
    return p;
  }
  else if (p == FINGERPRINT_NOTFOUND)
  {
    Serial.println("Did not find a match");
    noMatch();
    return p;
  }
  else
  {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  printName(finger.fingerID);
  return finger.fingerID;
}

void printName(int num)
{

  if (num == 1)
  {
    Serial.print("\nUser: User1; \n index: 165038\n");
    printLCD(num);
    return;
  }
  if (num == 2)
  {
    Serial.print("\nUser: User2 ; \n index: 166050\n");
    printLCD(num);
    return;
  }
  if (num == 3)
  {
    Serial.print("\nUser: User3; \n index: 142009\n");
    printLCD(num);
    return;
  }
  if (num == 4)
  {
    Serial.print("\nUser: User4; \n index: 165011\n");
    printLCD(num);
    return;
  }
  if (num == 5)
  {
    Serial.print("\nUser: User5; \n index: 155001\n");
    printLCD(num);
    return;
  }
  if (num == 6)
  {
    Serial.print("\n User6; \n index: 191010\n");
    printLCD(num);
    return;
  }
  if (num == 7)
  {
    Serial.print("\nUser:  User7; \n index: 185008\n");
    printLCD(num);
    return;
  }
  else
  {
    noMatch();
    return;
  }
  return;
}

void noMatch()
{
  Serial.print("\Nepoznato - Obidete se potvorno! \n");
  printLCD(-1);
  return;
}

void printLCD(int num)
{

  lcd.clear();
  lcd.setCursor(0, 0);
  if (num == 1)
  {
    lcd.print("Welcome User1!");
    lcd.setCursor(0, 1);
    lcd.print("Index: 165038");
    delay(10000);
    lcd.clear();
    return;
  }
  if (num == 2)
  {
    lcd.print("Welcome User2!");
    lcd.setCursor(0, 1);
    lcd.print("Index: 166050");
    delay(10000);
    lcd.clear();
    return;
  }
  if (num == 3)
  {
    lcd.print("Welcome User3!");
    lcd.setCursor(0, 1);
    lcd.print("Index: 142009");
    delay(10000);
    lcd.clear();
    return;
  }
  if (num == 4)
  {
    lcd.print("Welcome User4!");
    lcd.setCursor(0, 1);
    lcd.print("Index: 165011");
    delay(10000);
    lcd.clear();
    return;
  }
  if (num == 5)
  {
    lcd.print("Welcome Ivan!");
    lcd.setCursor(0, 1);
    lcd.print("Index: 155001");
    delay(10000);
    lcd.clear();
    return;
  }
  if (num == 6)
  {
    lcd.print("Welcome User!");
    lcd.setCursor(0, 1);
    lcd.print("Index: 191010");
    delay(10000);
    lcd.clear();
    return;
  }
  if (num == 7)
  {
   lcd.print("Welcome User!");
    lcd.setCursor(0, 1);
    lcd.print("Index: 185008");
    delay(10000);
    lcd.clear();
    return;
  }
  if (num == -1)
  {
    llcd.print("Welcome User!");
    lcd.setCursor(0, 1);
    lcd.print("povtorno");
    delay(3000);
    lcd.clear();
    return;
  }
  delay(5000);
  lcd.clear();
  return;
}

// ===================================================================================== getFingerPrintIDez() =====================================================================================

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
    return -1;

  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  return finger.fingerID;
}
