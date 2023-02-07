#include <AES.h>

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid     = "gioggio";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "piergiorgio";     // The password of the Wi-Fi network

MFRC522 rfidReader(D8, D0);
LiquidCrystal_I2C lcd(0x27,16,2);

void setup()
{
    //BEGIN SERIAL PORT
    Serial.begin(9600);

    //INIT LCD
    lcd.init();
    lcd.backlight();

    //WRITE LCD
    lcd.setCursor(0,0);
    lcd.print("ZAGUS BANK");

    WiFi.begin(ssid, password);

    int i=0;

    while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
        delay(400);
        lcd.setCursor(i,1);
        lcd.print("=");
        i++;
        if(i > 16)
        {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("ZAGUS BANK");
            i=0;
        }
    }

    //INIT RFID
    SPI.begin();
    rfidReader.PCD_Init();

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Terminale");
    lcd.setCursor(0,1);
    lcd.print("pronto");
}

void loop()
{
    if(!rfidReader.PICC_IsNewCardPresent())
    {
        return;
    }

    if(!rfidReader.PICC_ReadCardSerial())
    {
        return;
    }

    WiFiClient client;

    int connectionTime = 0;
    while(!client.connect("192.168.1.7", 2007))
    {
        if(connectionTime > 6)
        {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Connessione");
            lcd.setCursor(0,1);
            lcd.print("fallita");
            delay(3000);
            return;
        }
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Tentativo di");
        lcd.setCursor(0,1);
        lcd.print("connessione");
        connectionTime++;
    }

    String content = "";
    int cardUID = 0;

    for(int i = 0; i < rfidReader.uid.size; i++)
    {
        byte b = rfidReader.uid.uidByte[i];
        content += b;
    }

    cardUID = content.toInt();

    client.print(content);
    client.stop();

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Grazie per aver");
    lcd.setCursor(0,1);
    lcd.print("usato ZAGUSpay");

    delay(2000);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Terminale");
    lcd.setCursor(0,1);
    lcd.print("pronto");
}