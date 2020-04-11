/*----------------------------------------------------------------------
 * ESP8266 NEOPIXEL RING 35 LED NTP INTERNET CLOCK BY TUENHIDIY.
----------------------------------------------------------------------*/
#include <Adafruit_NeoPixel.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <time.h>
#include "timezone.h"

#define RING                D4    // NEOPIXEL RING 35

#define _RED                Adafruit_NeoPixel::Color(255, 0, 0)           //    RED
#define _GREEN              Adafruit_NeoPixel::Color(0, 255, 0)           //    GREEN
#define _BLUE               Adafruit_NeoPixel::Color(0, 0, 255)           //    BLUE
#define _ORANGE             Adafruit_NeoPixel::Color(255, 165, 0)         //    ORANGE
#define _YELLOW             Adafruit_NeoPixel::Color(255, 255, 0)         //    YELLOW
#define _MAGENTA            Adafruit_NeoPixel::Color(255, 0, 255)         //    MAGENTA
#define _PURPLE             Adafruit_NeoPixel::Color(128, 0, 128)         //    PURPLE
#define _CYAN               Adafruit_NeoPixel::Color(0, 255, 255)         //    CYAN
#define _PINK               Adafruit_NeoPixel::Color(255, 192, 203)       //    PINK
#define _WHITE              Adafruit_NeoPixel::Color(255, 255, 255)       //    WHITE
#define _BLACK              Adafruit_NeoPixel::Color(0, 0, 0)             //    BLACK

byte HHHH[4] = {16, 17, 18, 19};                    // Hour - 4 Bit Binary Number
byte M0[5] = {14, 13, 12, 11, 10};                  // Minute - Ten digit
byte M1[9] = {9, 8, 7, 6, 5, 4, 3, 2, 1};           // Minute - Unit digit
byte S0[5] = {21, 22, 23, 24, 25};                  // Minute - Ten digit
byte S1[9] = {26, 27, 28, 29, 30, 31, 32, 33, 34};  // Minute - Unit digit
byte SEPERATOR[3] = {0, 15, 20};                    // 3 Seperator leds

const char *WIFI_NETWORK_NAME = "FTPABCDEF"; // Change to your wifi network name
const char *WIFI_PASSWORD     = "12345678";      // Change to your wifi password

const char *TIME_SERVER       = "asia.pool.ntp.org";
int myTimeZone = VST;   // Change this to your time zone (see in timezone.h)

byte HH_Number, M1_Number, M0_Number, S1_Number, S0_Number;

byte s0, s1, m0, m1, hh;
byte prves0, prves1, prvem0, prvem1, prvehh;

unsigned long samplingtimes0 = 0;
unsigned long samplingtimes1 = 0;
unsigned long samplingtimem0 = 0;
unsigned long samplingtimem1 = 0;
unsigned long samplingtimehh = 0;
unsigned long samplingtimese = 0;

void DrawDot(byte number, byte coordinates[], byte maximum, uint32_t frontcolor);
void DrawBinary(byte number, byte coordinates[4], uint32_t frontcolor);
void Effect_S0(uint32_t frontcolor);
void Effect_S1(uint32_t frontcolor);
void Effect_M0(uint32_t frontcolor);
void Effect_M1(uint32_t frontcolor);
void Effect_HH(uint32_t frontcolor);
void Effect_SEP(uint32_t frontcolor);

time_t now;
struct tm *timeinfo;

int hour;
int mins;
int sec;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(35, RING, NEO_GRB + NEO_KHZ800);

void setup()
{
  WiFi.begin(WIFI_NETWORK_NAME, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  configTime(3600*myTimeZone, 0, TIME_SERVER);

  while (now < EPOCH_1_1_2019)
  {
    now = time(nullptr);
    delay(500);
  }
  strip.begin();
  strip.setBrightness(50);
  strip.show();
  pinMode(RING, OUTPUT);  
  
  if (WiFi.status() == WL_CONNECTED)
  {
    for(int i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, _RED);         
      strip.show();
      delay(200);                       
    }
  }
  for(int i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, _BLACK);         
      strip.show();
    }
}

void loop() 
{
    time(&now);
    timeinfo    = localtime(&now);

    hour        = timeinfo->tm_hour;
    mins        = timeinfo->tm_min;
    sec         = timeinfo->tm_sec;
  
    if (hour == 0) HH_Number = 12;              // Midnight
    else if (hour > 12) HH_Number = hour - 12;
    else HH_Number = hour;              

    M0_Number = ((mins/10) %10);
    M1_Number = (mins%10);
    
    S0_Number = ((sec/10) %10);
    S1_Number = (sec%10);
    
    Effect_S0(_GREEN);
    Effect_S1(_RED);
    
    Effect_M0(_BLUE);
    Effect_M1(_YELLOW);
    Effect_HH(_ORANGE);
    Effect_SEP(_WHITE);
}

void DrawDot(byte number, byte coordinates[], byte maximum, uint32_t frontcolor)
{ 
  for(int i=0; i< number; i++)
  {
    strip.setPixelColor(coordinates[i], frontcolor);
    strip.show();        
  }
  if((number==0))
  {
    for(int i=0; i< maximum; i++)
    {
      strip.setPixelColor(coordinates[i], _BLACK);
      strip.show();
    }
  } 
}

void DrawBinary(byte number, byte coordinates[4], uint32_t frontcolor)
{     
  for (int i = 0; i < 4; i++)
  {
    if (bitRead(number, i))
    {
      strip.setPixelColor(coordinates[i], frontcolor);
      strip.show();
    }
    else
    {
      strip.setPixelColor(coordinates[i], _BLACK);
      strip.show();
    }
  }
}

void Effect_HH(uint32_t frontcolor)
{ 
  if ( (unsigned long) (micros() - samplingtimehh) > 455  )
  {    
    hh = HH_Number;    
    if (hh != prvehh)  
      {   
        DrawBinary(HH_Number, HHHH, frontcolor);
        prvehh = hh;
      }
      samplingtimehh = micros(); 
    }
} 

void Effect_M1(uint32_t frontcolor)
{ 
  if ( (unsigned long) (micros() - samplingtimem1) > 755  )
  {     
    m1 = M1_Number;
    if (m1 != prvem1)  
      {      
        DrawDot(M1_Number, M1, 9, frontcolor);            
        prvem1 = m1;
      }
      samplingtimem1 = micros(); 
    }
}   

void Effect_M0(uint32_t frontcolor)
{ 
  if ( (unsigned long) (micros() - samplingtimem0) > 655  )
  {
    m0 = M0_Number;
    if (m0!=prvem0)  
      {         
        DrawDot(M0_Number, M0, 5, frontcolor);  
        prvem0 = m0;
      }
      samplingtimem0 = micros(); 
    }
} 

void Effect_S1(uint32_t frontcolor)
{ 
  if ( (unsigned long) (micros() - samplingtimes1) > 555  )
  {

    s1 = S1_Number;
    if (s1 != prves1)  
      {          
        DrawDot(S1_Number, S1, 9, frontcolor); 
        prves1 = s1;
      }
      samplingtimes1 = micros(); 
    }
}       

void Effect_S0(uint32_t frontcolor)
{ 
  if ( (unsigned long) (micros() - samplingtimes0) > 455  )
  {    
    s0 = S0_Number;    
    if (s0 != prves0)  
      {   
        DrawDot(S0_Number, S0, 5, frontcolor); 
        prves0 = s0;
      }
      samplingtimes0 = micros(); 
    }
}

void Effect_SEP(uint32_t frontcolor)
{ 
  if ( (unsigned long) (micros() - samplingtimese) > 999  )  
    DrawDot(3, SEPERATOR, 3, frontcolor); 
    samplingtimese = micros(); 
 }
