#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10  //10
#define RST_PIN 5  //5

#define LED_DENIED_PIN 7
#define LED_ACCESS_PIN 6

#define STATE_PIN 2

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Instance of the class

void setup() 
{
  Serial.begin(9600);
  SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522

  pinMode(STATE_PIN, INPUT);
  pinMode(LED_DENIED_PIN, OUTPUT);
  pinMode(LED_ACCESS_PIN, OUTPUT);

  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  // lcd.clear();
  lcd.print("Show your card:)");
}

void loop() 
{
  bool match = false;
  int state = digitalRead(STATE_PIN);
  // Si el estado es alto, el HC-05 está conectado
  if (state == HIGH) 
  {
    if (mfrc522.PICC_IsNewCardPresent())
    {
      if (mfrc522.PICC_ReadCardSerial())
      {
        match = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tag UID:");
        lcd.setCursor(0, 1);

        for (byte i = 0; i < mfrc522.uid.size; i++) 
        {
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
          Serial.print(mfrc522.uid.uidByte[i], HEX);

          lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          lcd.print(mfrc522.uid.uidByte[i], HEX);
        }

        Serial.println();

        delay(3000);

        lcd.clear();
        lcd.setCursor(0, 0);

        if (match) 
        {
          digitalWrite(LED_ACCESS_PIN, HIGH);
          lcd.print("Bienvenido");
        } else 
        {
          digitalWrite(LED_DENIED_PIN, HIGH);
          lcd.print("Acceso Denegado");
        }

        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();

        delay(3000);
        reset_state();
      }
    }//IF ISNEWCARDISPRESENT
  }//IF PINSTATE
}

void reset_state()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Show your card: ");
  digitalWrite(LED_ACCESS_PIN, LOW);
  digitalWrite(LED_DENIED_PIN, LOW);

}
