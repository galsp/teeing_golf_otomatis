#include <Arduino.h>

#include <Bonezegei_A4988.h>

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <EEPROM.h>
#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW //::FC16_HW

#define MAX_DEVICES 8
#define CS_PIN 11
#define DATA_PIN 10
#define CLK_PIN 12

// MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
MD_MAX72XX mx = MD_MAX72XX(MD_MAX72XX::PAROLA_HW, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define FORWARD 1
#define REVERSE 0

#define PIN_DIR 8
#define PIN_STEP 9

Bonezegei_A4988 stepper(PIN_DIR, PIN_STEP);

void print1(int value);
void print2(int value);

int count = 0;
int bar = 0;

#define pinup 6
#define pindown 5
#define pinrest 7
#define pinsw 4
int posisi = 0;
int range = 100;
void setup()
{
  stepper.begin();
  mx.begin();
  mx.clear();

  pinMode(pinup, INPUT_PULLUP);
  pinMode(pindown, INPUT_PULLUP);
  pinMode(pinrest, INPUT_PULLUP);
  pinMode(pinsw, INPUT_PULLUP);
  // myDisplay.begin();
  // myDisplay.setIntensity(0);
  // myDisplay.displayClear();
  print1(count);
  print2(bar);
  stepper._speed = 10000;
}

void loop()
{

  if (digitalRead(pinsw) == 0 && posisi == 1)
  {
    posisi = 0;
    stepper.step(FORWARD, 100 + bar);
  }
  else if (digitalRead(pinsw) == 1 && posisi == 0)
  {
    count++;
    print1(count);
    posisi = 1;
    stepper.step(REVERSE, 100 + bar);
  }
  else
    ;

  if (digitalRead(pinup) == 0)
  {
    bar++;
    stepper.step(FORWARD, 1);
    // mx.clear();
    print2(bar);
    delay(100);
  }

  if (digitalRead(pindown) == 0)
  {
    bar--;
    stepper.step(REVERSE, 1);
    // mx.clear();
    print2(bar);
    delay(100);
  }

  if (digitalRead(pinrest) == 0)
  {
    count = 0;

    // mx.clear();
    print1(count);
    delay(100);
    unsigned long lastmillis = millis();
    while (digitalRead(pinrest) == 0)
    {
      if (millis() - lastmillis > 3000)
      {
        bar = 0;
        print2(bar);
      }
    }
  }
}
void print1(int value)
{
  int space = 8;
  int geser = 1;
  int start = 63;
  start -= geser;

  // 192
  int digit[3];
  digit[0] = value / 100;       // 1
  digit[1] = (value / 10) % 10; // 9
  digit[2] = value % 10;        // 2

  if (digit[2] == 1)
  {
    mx.clear(4);
  }
  else if (digit[1] == 1)
  {
    mx.clear(5);
  }
  else if (digit[0] == 1)
  {
    mx.clear(6);
  }

  mx.setChar(start, 'C');
  mx.setChar(start - space, '0' + digit[0]);
  mx.setChar(start - space * 2, '0' + digit[1]);
  mx.setChar(start - space * 3, '0' + digit[2]);
}

void print2(int value)
{
  value += 500;
  int space = 8;
  int geser = 1;
  int start = 31;
  start -= geser;

  int digit[3];
  digit[0] = value / 100;       // 1
  digit[1] = (value / 10) % 10; // 9
  digit[2] = value % 10;        // 2

  if (digit[2] == 1)
  {
    mx.clear(0);
  }
  else if (digit[1] == 1)
  {
    mx.clear(1);
  }
  else if (digit[0] == 1)
  {
    mx.clear(2);
  }

  mx.setChar(start, 'T');
  mx.setChar(start - space, '0' + digit[0]);
  mx.setChar(start - space * 2, '0' + digit[1]);
  mx.setChar(start - space * 3, '0' + digit[2]);
}

void saveIntEEPROM(int alamat, int nilai)
{
  EEPROM.put(alamat, nilai);
}

int readIntEEPROM(int alamat)
{
  int nilai;
  EEPROM.get(alamat, nilai);
  return nilai;
}
