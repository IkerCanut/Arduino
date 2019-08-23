/* 
  Board I2C / TWI pins
  Uno       =   A4 (SDA), A5 (SCL)
  Mega2560  =   20 (SDA), 21 (SCL)
  Leonardo  =   2 (SDA), 3 (SCL)
  Due       =   20 (SDA), 21 (SCL), SDA1, SCL1
*/

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3f,16,2);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("==> HELLO <=="); 
  lcd.setCursor (0,1);
  lcd.print("==> WORLD  <==");
}

void loop() { 
  lcd.display();
  delay(500);
  
  lcd.noDisplay();
  delay(500);
}
