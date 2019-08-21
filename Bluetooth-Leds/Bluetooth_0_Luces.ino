#include <SoftwareSerial.h>
SoftwareSerial Bluetooth(10, 11);   // TX, RX.
int Data;                           // The data received.
 
void setup() {
  Bluetooth.begin(9600);
  Serial.begin(9600);
  Serial.println("Waiting for command...");

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
}
 
void loop() {
  if (Bluetooth.available()){       // Wait until receiving data...
    Data = Bluetooth.read();
    
    if(Data == 'Y'){  
      digitalWrite(2, !digitalRead(2));
      Serial.println("Yellow");
    }
    else if(Data == 'R'){  
      digitalWrite(3, !digitalRead(3));
      Serial.println("Red");
    }
    else if(Data == 'G'){  
      digitalWrite(4, !digitalRead(4));
      Serial.println("Green");
    } 
    else if (Data == 'P'){
      for (int j=2; j<=4; j++){
        digitalWrite(j, LOW);
      }
      for (int i=2; i<=4; i++){
        digitalWrite(i, HIGH);
        delay(100);
      }
    
      for (int i=4; i>=2; i--){
        digitalWrite(i, LOW);
        delay(100);
      }  
    }
  }

  delay(100);
}
