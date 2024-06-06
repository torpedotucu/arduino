//RFID

#include <SPI.h> // para la comunicación con el bus SPI.
#include <MFRC522.h> //Módulo RFID
#include <Wire.h>  //Comunicación I2C
#include <LiquidCrystal_I2C.h> //Pantalla LCD

//PINES RFID
#define SS_PIN 10  //PIN SLAVE SELECTOR
#define RST_PIN 5  //PIN RESET
//PINES LED
#define LED_DENIED_PIN 7
#define LED_ACCESS_PIN 6

LiquidCrystal_I2C lcd(0x27, 16, 2);//PANTALLA LCD
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Instancia RC522

void setup() 
{
    Serial.begin(9600); //Se inicializa la comunicación serial en 9600
    SPI.begin();  // Init SPI bus
    
    mfrc522.PCD_Init();  // Init MFRC522
    
    pinMode(LED_DENIED_PIN, OUTPUT);
    pinMode(LED_ACCESS_PIN, OUTPUT);

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.clear();

    lcd.print("Show your card:)");
}


void loop() {
    bool match = false;
    if (mfrc522.PICC_IsNewCardPresent())
    {
        if (mfrc522.PICC_ReadCardSerial())
        {
            match = true;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Tag UID:");

            lcd.setCursor(0, 1);

            for (byte i = 0; i < mfrc522.uid.size; i++) {
                Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : "");
                Serial.print(mfrc522.uid.uidByte[i], HEX);

                lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                lcd.print(mfrc522.uid.uidByte[i], HEX);
            }

            Serial.println();

            delay(3000);

            lcd.clear();
            lcd.setCursor(0, 0);

            /*
            if (match) 
            {
                digitalWrite(LED_ACCESS_PIN, HIGH);
                lcd.print("Authorized access");
            }
            else
            {
                digitalWrite(LED_DENIED_PIN, HIGH);
                lcd.print(" Access denied  ");
            }*/

            mfrc522.PICC_HaltA();
            mfrc522.PCD_StopCrypto1();

            delay(3000);
            reset_state();
        }
        else //NUEVA IMPLEMENTACION 
        {
            match=false;
        }
        if (match) 
            {
                digitalWrite(LED_ACCESS_PIN, HIGH);
                lcd.print("Authorized access");
            }
            else
            {
                digitalWrite(LED_DENIED_PIN, HIGH);
                lcd.print(" Access denied  ");
            }
    }
}

void reset_state()
{
    lcd.clear();
    lcd.setCursor(0, 0);

    lcd.print("Show your card: ");

    digitalWrite(LED_ACCESS_PIN, LOW);
    digitalWrite(LED_DENIED_PIN, LOW);
}
