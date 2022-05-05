#include <LiquidCrystal.h>
#include <Wire.h>
#include "RH_ASK.h"
#include <SPI.h>

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
RH_ASK driver;
int lcd_disp_time=0;
float consumer_current=0;
void setup() {
driver.init();
Serial.begin(9600);
}
void loop() {
unsigned long tm=millis();
while (millis()-tm<50) {
float cc = (float (analogRead(A5)*5)/1024.0-2.4911)/(0.098*7.346)/sqrt(2); 
if(cc>consumer_current){
  lcd.begin(16, 2);
  consumer_current=cc;
}
}
if (millis()-lcd_disp_time> 200) {
  lcd_disp_time=millis();

  lcd.setCursor(0, 1);
  if(consumer_current<2.45){
  Serial.print("Consumer Current = ");
  Serial.print(consumer_current);
  Serial.println(" A");
  lcd.print("Consumer Current = ");
  lcd.print(consumer_current);
  lcd.println(" A");
  }else{
    lcd.print("Consumer Current = MAX");
    }
}

String str = (String)consumer_current; 
uint8_t data[sizeof(str)];
for (byte i=0; i<sizeof(str); i++) { 
  data[i]=str[i];
}
driver.send(data, sizeof(data));
driver.waitPacketSent();
}
