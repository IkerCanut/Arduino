/*
    LCD SCREEN PINS:
    Uno       =   A4 (SDA), A5 (SCL)
    Mega2560  =   20 (SDA), 21 (SCL)
    Leonardo  =   2 (SDA), 3 (SCL)
    Due       =   20 (SDA), 21 (SCL), SDA1, SCL1
*/

#include <Adafruit_CC3000.h>
#include <DHT.h>
#include <SPI.h>
#include <ccspi.h>
#include <string.h>
#include "utility/debug.h"
#include <LiquidCrystal_I2C.h> 

#define DHTPIN          8
#define DHTTYPE         DHT11
#define PIN_MQ          9

#define WiDo_IRQ        7
#define WiDo_VBAT       5
#define WiDo_CS         10

#define WLAN_SSID       "< WLAN_SSID >"           // REPLACE
#define WLAN_PASS       "< WLAN_PASS >"           // REPLACE
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define IP1             192
#define IP2             168
#define IP3             1
#define IP4             100
#define PORT            53888
#define BUFFERSIZE      128

#define TIMEBTWREADS    1000

DHT dht(DHTPIN, DHTTYPE);
Adafruit_CC3000 WiDo = Adafruit_CC3000(WiDo_CS, WiDo_IRQ, WiDo_VBAT, SPI_CLOCK_DIVIDER);
LiquidCrystal_I2C lcd(0x3f,16,2); 

void setup(){
  lcd.init();
  lcd.backlight();
  
  printLCD("Initializing Wido");

  if (!WiDo.begin()) {
    printLCD("Check your wiring.");
    while(1);
  }  
  
  pinMode(PIN_MQ, INPUT);
  dht.begin();
  
  printLCD("Trying to connect to the Router");

  if (!WiDo.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    printLCD("Check WLAN_SSID and WLAN_PASS");
    while(1);
  }

  printLCD("Router Connected!");
}
          
void loop() {
  int humidity = dht.readHumidity();
  int temperature = dht.readTemperature();        // Celsius
  boolean mq_estado = digitalRead(PIN_MQ);        // Alcohol
  
  
  if (isnan(humidity) || isnan(temperature)) {
    printLCD("Error Reading Temperature / Humidity");
    return;
  }
  
  char bufferHT[50];
  sprintf(bufferHT, "Humidity: %d    Temperature: %d", humidity, temperature);
  printLCD(bufferHT);
  
  static Adafruit_CC3000_Client tcpClient;
  static unsigned long heartRate = millis();

  if(!tcpClient.connected()) {
    tcpClient.close();

    printLCD("Connecting to the TCPServer");
    
    uint32_t ip = WiDo.IP2U32(IP1, IP2, IP3, IP4);
    tcpClient = WiDo.connectTCP(ip, PORT);

    
    if(!tcpClient.connected()){
      printLCD("Error connecting to the server");
      delay(1000);
      return;
    }
  }
  
  if(millis() - heartRate > TIMEBTWREADS){
    heartRate = millis();

    char clientString[BUFFERSIZE-1];
    sprintf(clientString, "\"temp\": %d, \"alco\": %d, \"hume\": %d", temperature, !mq_estado, humidity);

    tcpClient.fastrprintln(clientString);
  }
}

void printLCD(char* msg){
    char msg1[17], msg2[17];
    strncpy(msg1, msg, 16);
    msg1[16] = '\0';
    strncpy(msg2, msg+16, 16);
    msg2[16] = '\0';

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(msg1);
    lcd.setCursor(0,1);
    lcd.print(msg2);   
}
