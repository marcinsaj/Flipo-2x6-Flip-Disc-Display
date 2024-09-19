#include <FlipDisc.h>         // https://github.com/marcinsaj/FlipDisc
#include <avr/wdt.h>
#include <util/delay.h>
#include <RTC_RX8025T.h>      // https://github.com/marcinsaj/RTC_RX8025T
#include <TimeLib.h>          // https://github.com/PaulStoffregen/Time
#include <Wire.h>             // https://arduino.cc/en/Reference/Wire (included with Arduino IDE)
#include <OneButton.h>        // https://github.com/mathertel/OneButton
#include <EEPROM.h>           // https://www.arduino.cc/en/Reference/EEPROM


// 1. Pin declaration for a dedicated controller
#define EN_PIN  10
#define CH_PIN  A1 
#define PL_PIN  A0


// Buttons - counting from the top
#define B1_PIN A3  // Top button
#define B2_PIN A2   // Middle button

// RTC
#define RTC_PIN 2 // RTC interrupt input

static const uint8_t TOP_ROW = 2;
static const uint8_t BOT_ROW = 1;

static const uint8_t CLR_ALL = 0;
static const uint8_t SET_ALL = 63;

uint8_t binaryTopRowAR[6];
uint8_t binaryBotRowAR[6];


int hour_time = 0;
int minute_time = 0;
uint32_t flipDelay = 1;


void setup() 
{
  Flip.Pin(EN_PIN, CH_PIN, PL_PIN);
  Flip.Init(D2X6);

  pinMode(RTC_PIN, INPUT_PULLUP);
  DelayTime(3000);

  hour_time = 11;
  minute_time = 23;

  DisplayTime();
}


void loop() 
{
  for(int i = 0; i < 60; i++)
  {
    minute_time = i;
    DisplayTime();
    delay(1000);
  }
  
  hour_time ++;
  minute_time = 0;
  DisplayTime();
}


void DisplayTime(void)
{
  DisplayData(BOT_ROW, minute_time);
  DisplayData(TOP_ROW, hour_time);
}




void DisplayData(uint8_t row, uint8_t data)
{
  uint32_t waitTime = 0;
  waitTime = flipDelay;
  
  if(row == BOT_ROW)
  {
    DecToBinary(data, binaryBotRowAR);
    for(int i = 1; i <= 6 ; i++) 
    {
      Flip.Disc_2x6(1, i, binaryBotRowAR[i-1]);
      DelayTime(waitTime);
    }
  }

  if(row == TOP_ROW)
  {
    DecToBinary(data, binaryTopRowAR);
    for(int i = 1; i <= 6; i++) 
    {
      Flip.Disc_2x6(1, i+6, binaryTopRowAR[i-1]);
      DelayTime(waitTime);
    }
  }
}




void DelayTime(uint32_t delayTime)
{
  uint32_t millis_time_now = millis();
  do
  {         

  } while(millis() - millis_time_now < delayTime);
}






void DecToBinary(uint8_t decValue, uint8_t binaryArray[]) 
{
  for(int i = 0; i < 6; i++)
  {
    binaryArray[i] = decValue & B00000001;
    decValue = decValue >> 1;
  }
}
