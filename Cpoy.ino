//for LCD display
#include <LiquidCrystal.h>
#include <Wire.h>
//for RX module
#include "RH_ASK.h"
#include <SPI.h>

//lcd pin configuration
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7; 
LiquidCrystal lcd (rs, en, d4, d5, d6, d7);

//tx module declaration
RH_ASK driver;

//for error response protection 
float previous_source_current=0;
float source_current = 0;

void setup() {
lcd.begin (20, 4);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Source current ");
lcd.setCursor(0,1);
lcd.print("Consumer ");
lcd.setCursor(0,2);
lcd.print("Theft");

//rx modules init
driver.init();

Serial.begin(9600);
}

void loop() {
uint8_t buf[12];
uint8_t buflen = sizeof(buf);
if (driver.recv(buf, &buflen)){
//whole further algorithm is done only if the data came to the receiver
float source_current=0;
unsigned long tm=millis(); // tm will stored system time on beginning of while (millis()-tm<50) cycle
}
unsigned long tm=millis(); // tm will stored system time on beginning of while (millis()-tm<50) cycle
while (millis()-tm<50) {//during 50 ms we read the ADC data, convert it to source current (sc) value (ms ac value) and looking t] 
  float sc = (float(analogRead (A5)*5)/1024.0-2.4911)/(0.098*7.346)/sqrt(2);
if(sc>source_current) {
   source_current=sc;
if( (source_current-previous_source_current> 0 ? source_current-previous_source_current : previous_source_current-source_current))
lcd.setCursor(15, 0);
if (source_current<2.45) {
// if measured value do not exceed wcs2702 current limit
lcd.print (source_current); 
lcd.setCursor (19,0);
lcd.print("A");
}
else{
lcd.print("  MAX");
}
String received=(char*)buf;
float consumer_current = received.toFloat();

lcd.setCursor(15, 1);
if (consumer_current<2.45){
// if measured value do not exceed wcs2702 current limit
lcd.print (consumer_current);
lcd.setCursor (19, 1);
lcd.print("A");
}else{
  lcd.print("  MAX");
}
lcd.setCursor(6,2);
if(source_current<2.45 && consumer_current<2.45){
// if measured value do not exceed wcs2702 current limit
if(((source_current-consumer_current)>=0 ? (source_current-consumer_current):(consumer_current-source_current))< source_current*0.05)
{// finding module source_current-consumer_current and 5% is need becase ADC convert error (in other words consumer current may few)
lcd.print("none     ");
}else{
lcd.print (source_current-consumer_current);
lcd.print(" A");
Serial.print("Theft = "); 
Serial.print(source_current-consumer_current);
Serial.println(" A");
}
}else{
lcd.print("-----     ");//if measured currents exceed wcs2702 current limit impossible to find power theft
}
}else{
  previous_source_current=source_current;
}
}
}
