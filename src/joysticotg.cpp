#if 1

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Firmata.h>
#include "util/SuperLoop.h"
#include "util/Button.h"

LiquidCrystal_I2C lcd(0x27, 16,2);

#define VRX      A1
#define VRY      A2
#define VRZ      A3

#define A    9
#define B    8
#define C    5
#define D    6
#define E    7
#define F   10
#define G   11
#define DP   4

#define COMMON   2

#define SYSEX_FND_1 0x10
#define SYSEX_FND_2 0x20
#define SYSEX_FND_3 0x30
#define SYSEX_FND_4 0x40
#define SYSEX_FND_5 0x50
#define SYSEX_FND_6 0x60
#define SYSEX_FND_7 0x70
#define SYSEX_FND_8 0x80
#define SYSEX_FND_9 0x90
#define SYSEX_FND_0 0x99

#define SYSEX_VRX   0x11;
#define SYSEX_VRY   0x12;
#define SYSEX_VRZ   0x13;


byte Numeric[] = { 0b11111100, //0
      0b01100000,   //1
      0b11011010,   //2
      0b11110010,   //3
      0b01100110,   //4
      0b10110110,   //5
      0b10111110,   //6
      0b11100000,   //7
      0b11111110,   //8
      0b11110110   //9
      };

byte fndPin[] = { A, B, C, D, E, F, G, DP };

class Joystickx: public SuperLoop {
public:
   void begin(uint32_t cycle) override {
      SuperLoop::begin(cycle);
   }

   void start() {
      setStop(false);
   }

   void stop() {
      setStop(true);
   }
private:
   void job() override {
      int vrx = analogRead(VRX);
      Firmata.sendAnalog(VRX%A0, vrx);
      vrx=map(vrx, 0, 1023, -360, 360);
      lcd.setCursor(0, 0);
      lcd.print("X = ");
      lcd.print(vrx);
      lcd.print("    ");
   }
};
Joystickx joystickx;

class Joysticky: public SuperLoop {
public:
   void begin(uint32_t cycle) override {
      SuperLoop::begin(cycle);
   }

   void start() {
      setStop(false);
   }

   void stop() {
      setStop(true);
   }
private:
   void job() override {
      int vry = analogRead(VRY);
      Firmata.sendAnalog(VRY%A0, vry);
      vry=map(vry, 0, 1023, -360, 360);
      lcd.setCursor(0, 1);
      lcd.print("Y = ");
      lcd.print(vry);
      lcd.print("    ");
   }
};
Joysticky joysticky;


bool onoff = true;
class Joystickz: public Button {
public:
   Joystickz() :
         Button(VRZ) {

   }
private:
   void push() override {
      if(onoff) {
         Firmata.sendString("ON");
         joystickx.start();
         joysticky.start();
         onoff = !onoff;
      } else {
         Firmata.sendString("OFF");
         joystickx.stop();
         joysticky.stop();
         onoff = !onoff;
      }
   }

};

Joystickz joysticz;

class Fnd: public SuperLoop {

   int Num = 0;
public:
   void begin(uint32_t cycle) override {
      SuperLoop::begin(cycle);
      for (int i = 0; i < sizeof(fndPin); i++)
         pinMode(fndPin[i], OUTPUT);
      pinMode(COMMON, OUTPUT);
   }
   void clear() {
      for (int i = 0; i < 8; i++) {
         digitalWrite(fndPin[i], 0); // all off
      }
   }
   void displayNum(byte num) {
      byte value = Numeric[num];
      for (int i = 0; i < 8; i++)
      digitalWrite(fndPin[i], bitRead(value, 7 - i)); //비트 갖고옴
   }

   void job() override {

   }
};

Fnd fnd;


void setup() {
   Firmata.begin(115200);
   lcd.init();
   lcd.backlight();
    joystickx.begin(200); joystickx.stop();
    joysticky.begin(200); joysticky.stop();
    joysticz.begin(50);
    fnd.begin(500);   fnd.stop();

   Firmata.attach(SET_PIN_MODE, [](uint8_t pin, int mode){
      pinMode(pin, mode);
   });

   Firmata.attach(SET_DIGITAL_PIN_VALUE, [](uint8_t pin, int value){
      digitalWrite(pin, value);
   });

   Firmata.attach(STRING_DATA, [](char *str){
      lcd.clear();
      lcd.print(str);
   });

   Firmata.attach(REPORT_ANALOG, [](uint8_t pin, int tf) {
      switch (pin) {
       case VRX%A0:
          tf ? joystickx.start() : joystickx.stop();
          break;
       case VRY%A0:
         tf ? joysticky.start() : joysticky.stop();
          break;
       case VRZ%A0:
         tf? joysticz.start() : joysticz.stop();

          break;
       default:
          break;
    }
   });

   Firmata.attach(START_SYSEX, [](uint8_t command, uint8_t argc, uint8_t *argv) {
      switch (command) {
       case SYSEX_FND_1:
          fnd.start();
          fnd.displayNum(1);
          break;
       case SYSEX_FND_2:
          fnd.start();
          fnd.displayNum(2);
          break;
       case SYSEX_FND_3:
          fnd.start();
          fnd.displayNum(3);
          break;
       case SYSEX_FND_4:
          fnd.start();
          fnd.displayNum(4);
          break;
       case SYSEX_FND_5:
          fnd.start();
          fnd.displayNum(5);
          break;
       case SYSEX_FND_6:
          fnd.start();
          fnd.displayNum(6);
          break;
       case SYSEX_FND_7:
          fnd.start();
          fnd.displayNum(7);
          break;
       case SYSEX_FND_8:
          fnd.start();
          fnd.displayNum(8);
          break;
       case SYSEX_FND_9:
          fnd.start();
          fnd.displayNum(9);
          break;
       case SYSEX_FND_0:
          fnd.start();
          fnd.displayNum(0);
          break;

       default:
          break;
    }
   });

}

void loop() {
   joystickx.loop();
   joysticky.loop();
   joysticz.loop();
   fnd.loop();
   while (Firmata.available())
      Firmata.processInput();
}
#endif
