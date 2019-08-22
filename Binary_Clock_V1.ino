#include <Adafruit_NeoPixel.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//NTP Defines
#define NTP_UTC_OFFSET  -14400; // UTC -4 (Offset * 60 * 60)

//LED Defines
#define LED_PIN         D3
#define LED_NUM_LEDS    6
#define LED_BRIGHTNESS  255

const char *ssid     = "SSID";
const char *password = "Password";

const long utcOffsetInSeconds = NTP_UTC_OFFSET;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int seconds[6];
int minutes[6];
int hours[5];

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();

  strip.setBrightness(LED_BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() 
{
  timeClient.update();

  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  ShowTime(timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());
  
  delay(1000);
}

void ShowTime(int hours, int minutes, int seconds)
{

}

int* ConvertToBinaryArray(int value)
{

  if(value > 60 || value < 0) return NULL;
  
  int NumElements = sizeof(value) / sizeof(int);
  int pos = 0;
  int[NumElements] binaryArray;

  while(value > 0)
  {
    binaryArray[pos] = value % 2;
    value = value / 2;
    pos++;
  }

  return binaryArray;
}
