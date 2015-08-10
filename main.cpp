#include <avr/io.h>
#include <avr/interrupt.h>
#include <vector>

#include "WProgram.h"
#include "leds.h"
#include "Modes/BaseMode.h"
#include "Modes/ColorMode.h"
#include "Modes/MeltMode.h"

#define LED_PIN 13
#define BRIGHTNESS_PIN 1
#define MODE_PIN 0
#define RANDOM_PIN 3

bool modeChange = false;
unsigned long lastActionTime = millis();
void modeButtonChange() {
    if ((digitalRead(MODE_PIN) == HIGH) && (millis() >= lastActionTime + 200)) {
        modeChange = true;
        lastActionTime = millis();
    }
}

extern "C" int main(void)
{
    Serial.begin(9600);
	static bool reboot = false;
	int brightness = LED_DEFAULT_BRIGHTNESS;

    pinMode(RANDOM_PIN, INPUT);
    randomSeed(analogRead(RANDOM_PIN));
    random16_add_entropy(random());

//	analogReference(DEFAULT);
//	analogReadResolution(10); // anything more seemed like overkill
//	analogReadAveraging(10);

	pinMode(LED_PIN, OUTPUT);
	pinMode(15, INPUT);

    // <cgerstle> button
    pinMode(MODE_PIN, INPUT);
    attachInterrupt(MODE_PIN, modeButtonChange, RISING);

	begin:

	FastLED.setBrightness(LED_DEFAULT_BRIGHTNESS);
	for (uint8_t i = 0; i < nLEDS; i++)
	    leds[i] = CHSV(0, 0, 0);
	led_init();


    std::vector<BaseMode*> modes;
    modes.push_back(new MeltMode());
    modes.push_back(new ColorMode("purple", CHSV(192, 255, 255), 125));
    modes.push_back(new ColorMode("orange", CHSV(32, 255, 255), 125));
    modes.push_back(new ColorMode("white", CHSV(0, 0, 255), 0));
    std::vector<BaseMode*>::iterator modeIterator = modes.begin();

    Serial.printf("mode count: %d\n", modes.size());
    for (std::vector<BaseMode*>::iterator it = modes.begin() ; it != modes.end(); ++it)
    {
        Serial.printf("mode: %s\n", (*it)->name());
    }

	for (uint8_t i = 0; i < nLEDS; i++)
	    leds[i] = CHSV(0, 0, 0);
	led_show();

	// Get a random seed
	randomSeed(analogRead(8));

	while (1) {
		if (reboot)
			goto reboot;

		brightness = analogRead(BRIGHTNESS_PIN);
		//Serial.print(brightness); Serial.print("\t");
		brightness = map(brightness, 525, 1023, 0, 255);
		if (brightness < 15)
		    brightness = 0;
		//Serial.println(brightness);
		FastLED.setBrightness(brightness);

		(*modeIterator)->frame();
		led_show();

		if (modeChange)
		{
		    modeChange = false;
		    digitalWrite(LED_PIN, HIGH);   // set the LED on

		    modeIterator++;
		    if (modeIterator == modes.end())
		    {
		        Serial.println("at back, starting over");
		        modeIterator = modes.begin();
		    }

		    Serial.println((*modeIterator)->name());
		    digitalWrite(LED_PIN, LOW);    // set the LED off
		}
	}

	reboot:

	// Attempt teardown
	reboot = false;
	for(std::vector<BaseMode*>::iterator i = modes.begin(); i != modes.end(); ++i )
	{
    	delete *i;
	}
	modes.clear();

    for (int i = 0; i < nLEDS; i++)
        leds[i] = 0;
	led_show();

    goto begin;
}

