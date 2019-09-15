#include <Adafruit_NeoPixel.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//Global Defines
#define UPDATE_DELAY			1000

//NTP Defines
#define NTP_UTC_OFFSET			-14400 // UTC -4 (Offset * 60 * 60)

//LED Defines
#define LED_PIN					D3
#define LED_NUM_LEDS			6
#define LED_BRIGHTNESS			255

const char* ssid = "<YourSSID>";
const char* password = "<YourPassword>";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", NTP_UTC_OFFSET);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

const uint32_t red = strip.Color(200, 0, 0);
const uint32_t green = strip.Color(0, 225, 0);
const uint32_t blue = strip.Color(0, 0, 255);
const uint32_t off = strip.Color(0, 0, 0);

void setup()
{
	Serial.begin(115200);

	initStrip();

	initWifi();

	timeClient.begin();
}

void initWifi()
{
	stripSolidColor(red);
	Serial.print("Connecting to: ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	stripSolidColor(green);
	delay(500);
}

void initStrip()
{
	strip.setBrightness(LED_BRIGHTNESS);
	strip.begin();

	for (int i = 0; i < LED_NUM_LEDS; i++)
	{
		strip.setPixelColor(i, off);
	}

	strip.show(); // Initialize all pixels to 'off'
}

void loop()
{
  unsigned long lastRun = millis();
	timeClient.update();

	Serial.print(timeClient.getHours());
	Serial.print(":");
	Serial.print(timeClient.getMinutes());
	Serial.print(":");
	Serial.println(timeClient.getSeconds());
	showTime(timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());

  while(lastRun + UPDATE_DELAY > millis())
  {
    
  }
}

void showTime(int hours, int minutes, int seconds)
{
	for (int pixelNum = 0; pixelNum < 6; pixelNum++)
	{
		strip.setPixelColor(pixelNum, strip.Color((seconds & (1 << pixelNum)) ? 255 : 0, (minutes & (1 << pixelNum)) ? 255 : 0, (hours & (1 << pixelNum)) ? 255 : 0));
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
