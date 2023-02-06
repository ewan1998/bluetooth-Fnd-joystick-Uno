#if 0

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

#define LED0	9
#define LED1	8
#define LED2	7
#define LED3	6
#define LED4	5
#define LED5	4
#define LED6	3
#define LED7	2

#define POT		A0
#define	CDS1	A1
#define	CDS2	A2

void setup(){
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  for(int i=2; i<10;i++){
    pinMode(i,OUTPUT);
    digitalWrite(i,HIGH);
  }
}

void loop(){
  int pot= analogRead(POT);
  int cds1= analogRead(CDS1);
  int cds2= analogRead(CDS2);

  Serial.println(String("POT = ")+pot);
  Serial.println(String("cds1 = ")+cds1);
  Serial.println(String("cds2 = ")+cds2);


  lcd.setCursor(0,0);
  lcd.print(String("POT = ")+pot);
  lcd.setCursor(0,1);
  char buff[20];
    sprintf(buff,"[%4d , %4d]",cds1,cds2);
  lcd.print(buff);

  delay(500);

}



#endif
