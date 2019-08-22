#include <Adafruit_NeoPixel.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//Global Defines
#define DELAY_BETWEEN_UNITS	4000

//NTP Defines
#define NTP_UTC_OFFSET		-14400 // UTC -4 (Offset * 60 * 60)

//LED Defines
#define LED_PIN				D3
#define LED_NUM_LEDS		6
#define LED_BRIGHTNESS		255

const char* ssid = "SSID";
const char* password = "PASSWORD";


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", NTP_UTC_OFFSET);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
	Serial.begin(115200);

	initStrip();

	initWifi();

	timeClient.begin();
}

void initWifi()
{
	stripSolidColor(strip.Color(255, 0, 0));
	Serial.print("Connecting to: ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	stripSolidColor(strip.Color(0, 255, 0));
}

void initStrip()
{
	strip.setBrightness(LED_BRIGHTNESS);
	strip.begin();

	for (int i = 0; i < LED_NUM_LEDS; i++)
	{
		strip.setPixelColor(i, strip.Color(0, 0, 0));
	}

	strip.show(); // Initialize all pixels to 'off'
}

void loop()
{
	timeClient.update();

	Serial.print(timeClient.getHours());
	Serial.print(":");
	Serial.print(timeClient.getMinutes());
	Serial.print(":");
	Serial.println(timeClient.getSeconds());
	showTime(timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());
}

void showTime(int hours, int minutes, int seconds)
{
	showDigit(hours, 0, strip.Color(255,0,0));
	delay(DELAY_BETWEEN_UNITS);
	showDigit(minutes, 0, strip.Color(0,255,0));
	delay(DELAY_BETWEEN_UNITS);
	showDigit(seconds, 0, strip.Color(0,0,255));
	delay(DELAY_BETWEEN_UNITS);
}

void showDigit(int value, int offset, uint32_t on)
{
	uint32_t off = strip.Color(0, 0, 0);
	for (int pixelNum = 0; pixelNum < 6; pixelNum++)
	{
		strip.setPixelColor(offset + pixelNum, (value & (1 << pixelNum)) ? on : off);
	}
	strip.show();
}

void stripSolidColor(uint32_t color)
{
	for (int i = 0; i < LED_NUM_LEDS; i++)
	{
		strip.setPixelColor(i, color);
	}
	strip.show();
}