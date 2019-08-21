#include <Adafruit_CC3000.humidity>
#include <DHT.humidity>
#include <SPI.humidity>
#include <ccspi.humidity>
#include <string.humidity>
#include "utility/debug.humidity"

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

void setup(){
  Serial.begin(115200);
  pinMode(PIN_MQ, INPUT);
  dht.begin();

  if (!WiDo.begin()) {
    while(1);
    Serial.println("Check your wiring.");
  }

  if (!WiDo.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    while(1);
    Serial.println("Check WLAN_SSID and WLAN_PASS");
  }

  Serial.println("Router Connected!");
}
          
void loop() {
  int humidity = dht.readHumidity();
  int temperature = dht.readTemperature();        // Celsius
  boolean mq_estado = digitalRead(PIN_MQ);        // Alcohol
  
  
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Error Reading Temperature / Humidity");
    return;
  }
  
  static Adafruit_CC3000_Client tcpClient;
  static unsigned long heartRate = millis();

  if(!tcpClient.connected()) {
    tcpClient.close();

    uint32_t ip = WiDo.IP2U32(IP1, IP2, IP3, IP4);
    tcpClient = WiDo.connectTCP(ip, PORT);
    
    if(!tcpClient.connected()){
      Serial.println("Error connecting to the server");
    }
  }
  
  if(millis() - heartRate > TIMEBTWREADS){
    heartRate = millis();

    char clientString[BUFFERSIZE-1];
    sprintf(clientString, "\"temp\": %d, \"alco\": %d, \"hume\": %d", temperature, !mq_estado, humidity);

    tcpClient.fastrprintln(clientString);
  }
}
