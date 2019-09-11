#include <DHT.h>

#define DHTPIN 8
#define DHTTYPE DHT11
#define LEDPIN 2
#define TIMEBTWREAD 5000
#define TMPLIMIT 25

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(LEDPIN, OUTPUT);
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("ERROR: Reading data");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("\t Humidity: ");
  Serial.println(humidity);
  
  if(temperature > TMPLIMIT) {
    digitalWrite(LEDPIN,HIGH);
  } else {
    digitalWrite(LEDPIN,LOW);
  }
  
  delay(TIMEBTWREAD);
}
