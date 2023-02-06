#if 0

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Firmata.h>

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
  Firmata.begin(115200);
  lcd.init();
  lcd.backlight();
}

void loop(){
	while(Firmata.available())
		Firmata.processInput();
}



#endif
