#define ileScalakow 2
#define ilePinow ileScalakow * 8
#define DLUGOSC_KOMBINACJI 5
#include <PCF8574.h>
#include <Wire.h>   
#include <LiquidCrystal_I2C.h> 

int tableOfFrequency[16]= {200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2600, 2800, 3000, 3200, 3400};
PCF8574 expander1;
PCF8574 expander2;
int SER=8; 
int RCLK=9;
int SRCLK=10; 
int rejestr[ilePinow];
LiquidCrystal_I2C lcd(0x3F,16,2);  // Ustawienie adresu ukladu na 0x27
int kombinacja[DLUGOSC_KOMBINACJI];
int rekord = 0;
volatile int number =-1;
volatile int lastNumber =-1;
int pom;


void onPin0()
{
  number=8;
  lastNumber =0;
}

void onPin1()
{
  number=9;
  lastNumber =0;
}

void onPin2()
{
  number=10;
  lastNumber =0;
}

void onPin3()
{
  number=11;
  lastNumber =0;
}

void onPin8()
{
  number=0;
  lastNumber =0;
}

void onPin9()
{
  number=1;
  lastNumber =0;
}

void onPin10()
{
  number=2;
  lastNumber =0;
}

void onPin11()
{
  number=3;
  lastNumber =0;
}

void onPin12()
{
  number=4;
  lastNumber =0;
}
void onPin13()
{
  number=5;
  lastNumber =0;
}
void onPin14()
{
  number=6;
  lastNumber =0;
}
void onPin15()
{
  number=7;
  lastNumber =0;
}
 

void onInterrupt()
{
  expander1.checkForInterrupt();
  expander2.checkForInterrupt();
}

void setup(){
  Serial.begin(9600);

  expander1.begin(0x38);
  expander2.begin(0x39);
 for(int i=0; i<8; i++){
  expander1.pinMode(i, INPUT);
  expander1.pullUp(i);
 }

 for(int i=0; i<3; i++){
  expander2.pinMode(i, INPUT);
  expander2.pullUp(i);
 }


  pinMode(2, INPUT);
  digitalWrite(2, HIGH);

  expander1.enableInterrupt(2, onInterrupt);

  expander1.attachInterrupt(0, onPin0, FALLING);
  expander1.attachInterrupt(1, onPin1, FALLING);
  expander1.attachInterrupt(2, onPin2, FALLING);
  expander1.attachInterrupt(3, onPin3, FALLING);
  expander2.attachInterrupt(0, onPin8, FALLING);
  expander2.attachInterrupt(1, onPin9, FALLING);
  expander2.attachInterrupt(2, onPin10, FALLING);
  expander2.attachInterrupt(3, onPin11, FALLING);
 expander2.attachInterrupt(4, onPin12, FALLING);
 expander2.attachInterrupt(5, onPin13, FALLING);
 expander2.attachInterrupt(6, onPin14, FALLING);
 expander2.attachInterrupt(7, onPin15, FALLING);
  
  pinMode(SER, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  czyscRejestr();
  zapiszRejestr();
  lcd.init();  
  lcd.backlight(); // zalaczenie podwietlenia 
  lcd.setCursor(0,0); 
  lcd.print("LCD & I2C");
  delay(500);
  lcd.setCursor(0,1);
  lcd.print("Rekord sesji: ");
  lcd.print(rekord);  
  randomSeed(analogRead(0));
  for(int i = 0; i<DLUGOSC_KOMBINACJI; i++)
  {
    kombinacja[i] = random(0,12);
  }  
}
void czyscRejestr(){
  for(int i=0; i<ilePinow; i++)
    rejestr[i]=LOW;
}
void zapiszRejestr(){
  digitalWrite(RCLK, LOW); 
  for(int i=ilePinow-1; i>=0; i--){
    digitalWrite(SRCLK, LOW);
    digitalWrite(SER, rejestr[i]);
    digitalWrite(SRCLK, HIGH); 
  }
  digitalWrite(RCLK, HIGH); 
}
void ustawPin(int ktory, int wartosc){
  rejestr[ktory]=wartosc;
}

void wyswietl_do_elementu(int element)
{
  czyscRejestr();
  for(int i = 0; i < element+1; i++)
  {
    tone(A3, tableOfFrequency[kombinacja[i]]);
    delay(100);
    noTone(A3);
    ustawPin(kombinacja[i],HIGH);
    zapiszRejestr();
    delay(800);
    ustawPin(kombinacja[i],LOW);
    zapiszRejestr();
    delay(100);
  }  
}

void loop(){

  int a = -1; //ilosc poprawnych indeksow
  czyscRejestr();
  for(int i = 0; i < DLUGOSC_KOMBINACJI; i++)
  {
    wyswietl_do_elementu(i);
    czyscRejestr();
    while(a != i)
    {
      if(lastNumber == 0)
      {
        if(number == kombinacja[a+1])
        {
          tone(A3, tableOfFrequency[kombinacja[a+1]]);
          delay(100);
          noTone(A3);          
          ustawPin(number,HIGH);
          zapiszRejestr();
          delay(800);
          ustawPin(number,LOW);
          zapiszRejestr();
          czyscRejestr();
          a++;
          lastNumber = -1;
          delay(100);
        }
        else
        {
             tone(A3, tableOfFrequency[0]);
             delay(200);
             noTone(A3);
             delay(200);
             tone(A3, tableOfFrequency[1]);
             delay(200);
             noTone(A3);
             lcd.setCursor(0,0); 
             lcd.print("Blad! Koniec gry");
             lcd.setCursor(0,1);
             lcd.print("Rekord sesji: ");
             lcd.print(rekord);
             while(1){delay(10000);}      
        }
      }
      delay(100);
    }
    tone(A3, tableOfFrequency[7]);
    delay(200);
    noTone(A3);
    delay(100);
    tone(A3, tableOfFrequency[8]);
    delay(200);
    noTone(A3);    
    rekord++;
    lcd.setCursor(0,1);
    lcd.print("Rekord sesji: ");
    lcd.print(rekord);
    a = -1;
    delay(3000);
  }
  lcd.setCursor(0,0); 
  lcd.print("----Sukces!!----");
  lcd.setCursor(0,1);
  lcd.print("Rekord sesji: ");
  lcd.print(rekord);
  while(1){delay(10000);}           
}
