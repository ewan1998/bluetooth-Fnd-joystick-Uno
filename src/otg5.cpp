#if 0

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Firmata.h>
#include "util/SuperLoop.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define LED0	9
#define LED1	8
#define LED2	7
#define LED3	6
#define LED4	5
#define LED5	4
#define LED6	3
#define LED7	2

#define POT		A0
#define CDS1	A1
#define CDS2	A2

#define SYSEX_LED_LEFT 	0x10
#define SYSEX_LED_STOP 	0x11
#define SYSEX_LED_RIGHT 0x12

class Led : public SuperLoop {
	byte leds[8] = {
			LED0, LED1, LED2, LED3, LED4, LED5, LED6, LED7
	};
public:
	byte command=SYSEX_LED_STOP;
	void begin(uint32_t cycle) override {
		SuperLoop::begin(cycle);

		for (int i=0; i<8; i++)
			pinMode(leds[i], OUTPUT);

		digitalWrite(leds[3], HIGH);
		digitalWrite(leds[5], HIGH);
		digitalWrite(leds[6], HIGH);
	}
private:
	void job() override {
		switch (command) {
			case SYSEX_LED_LEFT:
				left();
				break;
			case SYSEX_LED_RIGHT:
				right();
				break;

			default:
				break;
		}

	}

	void left() {
		byte keep = digitalRead(leds[0]);
		for (int i=0; i<7; i++)
			digitalWrite(leds[i], digitalRead(leds[i+1]));

		digitalWrite(leds[7], keep);

	}

	void right() {
		byte keep = digitalRead(leds[7]);
		for (int i=7; i>0; i--)
			digitalWrite(leds[i], digitalRead(leds[i-1]));

		digitalWrite(leds[0], keep);
	}

public:
	void start() {
		setStop(false);
	}

	void stop() {
		setStop(true);
	}

};

Led led;

void setup() {
	Firmata.begin(115200);
	lcd.init();
	lcd.backlight();

	led.begin(200);
	led.stop();

	Firmata.attach(SET_PIN_MODE, [](uint8_t pin, int mode) {
		pinMode(pin, mode);
	});

	Firmata.attach(SET_DIGITAL_PIN_VALUE, [](uint8_t pin, int value) {
		digitalWrite(pin, value);
	});

	Firmata.attach(STRING_DATA, [](char *str) {
		lcd.clear();
		lcd.print(str);
	});

	Firmata.attach(START_SYSEX, [](uint8_t command, uint8_t argc, uint8_t *argv) {
		switch (command) {
			case SYSEX_LED_LEFT:
				led.start();
				led.command = SYSEX_LED_LEFT;
				break;
			case SYSEX_LED_STOP:
				led.stop();
				led.command = SYSEX_LED_STOP;
				break;
			case SYSEX_LED_RIGHT:
				led.start();
				led.command = SYSEX_LED_RIGHT;
				break;
			default:
				break;
		}
	});

}

void loop() {
	while (Firmata.available())
		Firmata.processInput();

	led.loop();
}
#endif

