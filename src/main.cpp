#include <iostream>
#include <cstdlib>
#include <wiringPi.h>
#include <pcf8574.h>
#include <lcd.h>
#include <ADCDevice.hpp>
#include "Keypad.hpp"

// GPIO Pin Numbers (WiringPi)
const int LED_PIN = 2;
const int BUZZER_PIN = 3;
const int PIR_PIN = 0;

// Timing
const unsigned int ADC_READ_INTERVAL_MS = 1000;
const unsigned int LCD_STARTUP_DELAY_MS = 100;
const unsigned int LCD_RETRY_DELAY_MS = 500;
const int LCD_INIT_ATTEMPTS = 3;
const unsigned int MAIN_LOOP_DELAY_MS = 50;

// Keypad Setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] =
{
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {22, 23, 24, 25};
byte colPins[COLS] = {26, 27, 28, 29};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

bool systemArmed = false;

// LCD Setup
const int LCD_ADDRESS = 0x27;
const int BASE = 64;

const int RS = BASE + 0;
const int RW = BASE + 1;
const int EN = BASE + 2;
const int LCD_BACKLIGHT = BASE + 3;
const int D4 = BASE + 4;
const int D5 = BASE + 5;
const int D6 = BASE + 6;
const int D7 = BASE + 7;

int lcdHandle;

// ADC Object
ADCDevice *adc;

// Temperature Change Detection
const int ADC_CHANGE_THRESHOLD = 8;
int baselineADC = 0;
bool baselineSet = false;
bool tempAlarm = false;
unsigned int lastADCReadTime = 0;

int main()
{
	// initialize wiringPi
	if (wiringPiSetup() == -1)
	{
		std::cout << "wiringPi setup failed" << std::endl;
		return 1;
	}
	
	// Keypad Setup
	char key = 0;
	keypad.setDebounceTime(50);
	
	// GPIO Setup
	pinMode(LED_PIN, OUTPUT);
	pinMode(BUZZER_PIN, OUTPUT);
	pinMode(PIR_PIN, INPUT);
	
	digitalWrite(LED_PIN, LOW);
	digitalWrite(BUZZER_PIN, LOW);
	
	// LCD Setup
	pcf8574Setup(BASE, LCD_ADDRESS);
	
	for (int i = 0; i < 8; i++)
	{
		pinMode(BASE + i, OUTPUT);
	}
	
	digitalWrite(LCD_BACKLIGHT, HIGH);
	digitalWrite(RW, LOW);
	
	delay(LCD_STARTUP_DELAY_MS);
	
	lcdHandle = -1;
	
	for (int attempt = 0; attempt < LCD_INIT_ATTEMPTS; attempt++)
	{
		lcdHandle = lcdInit(2, 16, 4, RS, EN, D4, D5, D6, D7, 0, 0, 0, 0);
		
		if (lcdHandle != -1)
		{
			break;
		}
		
		delay (LCD_RETRY_DELAY_MS);
	}
	
	if (lcdHandle == -1)
	{
		std::cout << "LCD Init Failed" << std::endl;
		return 1;
	}
	
	lcdPosition(lcdHandle, 0, 0);
	lcdPuts(lcdHandle, "Burglar Alarm");
	
	lcdPosition(lcdHandle, 0, 1);
	lcdPuts(lcdHandle, "DISARMED");
	
	// ADC Setup
	adc = new ADS7830();
	
	while (1)
	{
		// Get Current Time
		unsigned int currentTime = millis();
		
		// Read Keypad
		key = keypad.getKey();
		if (key == 'A')
		{
			systemArmed = true;
			
			lcdPosition(lcdHandle, 0, 1);
			lcdPuts(lcdHandle, "ARMED   ");
		}
		else if (key == 'D')
		{
			systemArmed = false;
			
			tempAlarm = false;
			baselineSet = false;
			baselineADC = 0;
			
			lcdPosition(lcdHandle, 0, 1);
			lcdPuts(lcdHandle, "DISARMED");
		}
		
		// Read Motion Sensor
		int motionDetected = digitalRead(PIR_PIN);
		
		// Read Temperature Every Second if Armed
		if (systemArmed && currentTime - lastADCReadTime >= ADC_READ_INTERVAL_MS)
		{
			lastADCReadTime = currentTime;
			
			int adcValue = adc->analogRead(0);
			
			if (!baselineSet)
			{
				baselineADC = adcValue;
				baselineSet = true;
			}
			
			int adcChange = abs(adcValue - baselineADC);
			
			// Alarm if Temp Changes Beyond Threshold 
			tempAlarm = (adcChange >= ADC_CHANGE_THRESHOLD);
		}
		
		// Alarm Outputs
		if (systemArmed)
		{
			digitalWrite(BUZZER_PIN, motionDetected ? HIGH : LOW);
			digitalWrite(LED_PIN, tempAlarm ? HIGH : LOW);
		}
		else 
		{
			digitalWrite(BUZZER_PIN, LOW);
			digitalWrite(LED_PIN, LOW);
		}
		
		delay(MAIN_LOOP_DELAY_MS);
	}
	
	return 0;
}
