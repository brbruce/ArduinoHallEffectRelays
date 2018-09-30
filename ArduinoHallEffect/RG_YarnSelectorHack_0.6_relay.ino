/*
  Yarn Selector Hack
  Controlling Saurer 350 power loom's yarn selection unit digitally.
  Created March 2018
  By Simon Gylden
  http://www.randgylden.com/
*/


#include "LiquidCrystal.h" //include LDC library
LiquidCrystal lcd(8,9,4,5,6,7); // initialize the library

//Physical Inputs & Outputs:
int HALL = A0;
int POTENTIOMETER = A1;
const int buttonPin = 2;
int Relay1 = 31;
int Relay2 = 33;
int Relay3 = 35;
int Relay4 = 37;
int Relay5 = 39;
int Relay6 = 41;

//Variables:
int HESvalue = 0;
int counter = -1 ; //-1 for array's zero indexing
int currentPatternLength = 0;
String progName = "";
byte currentYarn = 0;
int patternSelect = 0; 
int counterPosition = 3;
int counterPositionLength = 11;
int buttonCount = 0;
int buttonState = 0;
byte confirmMarker = 1;
int currentRelay = 0;




// Patterns put in this section (PROGMEM to save to flash memory)
//***********************************************************************

// Pattern Array #1 (0-341)
const PROGMEM byte Pattern01[] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}; 

// Pattern Array #2 (342-682)
const PROGMEM byte Pattern02[] = {3, 3, 3, 4, 4, 4, 5, 5, 5}; 

// Pattern Array #3 (683-1023)
const PROGMEM byte Pattern03[] = {0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6}; 

//***********************************************************************




//-------------------------------------------------------------------------
// SETUP SECTION - CHOOSING PATTERN THROUGH ROTARY KNOB AND CONFIRM BUTTON
//-------------------------------------------------------------------------

void setup()
  {        

//Relay pins
   pinMode(Relay1, OUTPUT);
   digitalWrite(Relay1, HIGH); 
   pinMode(Relay2, OUTPUT);
   digitalWrite(Relay2, HIGH); 
   pinMode(Relay3, OUTPUT);
   digitalWrite(Relay3, HIGH); 
   pinMode(Relay4, OUTPUT);
   digitalWrite(Relay4, HIGH); 
   pinMode(Relay5, OUTPUT);
   digitalWrite(Relay5, HIGH); 
   pinMode(Relay6, OUTPUT);
   digitalWrite(Relay6, HIGH); 

//Hall pin
    pinMode (HALL,INPUT); //Initiating Hall Effect pin input  

//LCD definition initialization   
    lcd.begin(16,2); 
    lcd.setCursor(0,0);
    lcd.print(progName); //print program name
    lcd.setCursor(3,1); 
    lcd.print("0000 of");

    while (digitalRead(buttonPin) == LOW) {
    
//--------------------- Potentiometer section  -----------------------

  int POTvalue = analogRead(POTENTIOMETER); //Initiating potentiometer
  delay(10);        // debouncing potentiometer

//-------- Array section - choose, read, print and set length --------

  if (POTvalue >= 683)
    { 
      patternSelect = Pattern01;
      currentPatternLength = sizeof(Pattern01)-1;
      progName = "01) The Fade  ";
      } 

  else if ((POTvalue >= 342) && (POTvalue <= 682))
    {
      patternSelect = Pattern02;
      currentPatternLength = sizeof(Pattern02)-1;
      progName = "02) One Two   ";
      }

  else if (POTvalue <= 341)
    {
      patternSelect = Pattern03;  
      currentPatternLength = sizeof(Pattern03)-1;
      progName = "03) Dazzle    ";
      }  

//print program name and pattern length:
    
   lcd.setCursor(0,0);
   lcd.print(progName);
   lcd.setCursor(11,1);
   lcd.print("    ");
   lcd.setCursor(11,1);
   lcd.print(currentPatternLength+1);
   }
        }

//---------------------------------------------------------------------------
// LOOP SECTION - READING HALL EFFECT, CHOOSING YARN/RELAY AND PRINTING INFO
//---------------------------------------------------------------------------

void loop()
  {

//Printing Confirmation Marker (only at start-up)
if (confirmMarker == 1) {
  lcd.setCursor(15,0); 
  lcd.print("*");
  confirmMarker = 0;
}
    
//------------------- Hall Effect Sensor section  --------------------

    HESvalue = analogRead(HALL);
    if(HESvalue < 490) //sensor threshold
    {
      counter++;
      while(HESvalue < 500 ) //debouncing sensor
      {
        delay(10); //debouncing sensor
        HESvalue = analogRead(HALL);
        }
  
//-------------------------- LCD DISPLAY -----------------------------

  currentYarn = (pgm_read_byte(patternSelect + counter)); 

//print current yarn (for debugging):
  lcd.setCursor(15,0); 
  lcd.print(currentYarn);

//zero padding & counter printing:
 if (counter+1 > 999) {counterPosition=3;} 
 else if ((counter+1 >= 100) && (counter+1 <= 999)) {counterPosition=4;} 
 else if ((counter+1 >= 10) && (counter+1 <= 99)) {counterPosition=5;}
 else if (counter+1 < 10) {counterPosition=6;}

//print counter:
  lcd.setCursor(3,1);  
  lcd.print("0000");
  lcd.setCursor(counterPosition,1);  
  lcd.print(counter+1);

//zero padding & pattern length printing:
 if (currentPatternLength+1 > 999) {counterPositionLength=11;} 
 else if ((currentPatternLength+1 >= 100) && (currentPatternLength+1 <= 999)) {counterPositionLength=12;} 
 else if ((currentPatternLength+1 >= 10) && (currentPatternLength+1 <= 99)) {counterPositionLength=13;}
 else if (currentPatternLength+1 < 10) {counterPositionLength=14;}

//print total pattern length:
  lcd.setCursor(11,1);
  lcd.print("0000"); 
  lcd.setCursor(counterPositionLength,1); 
  lcd.print(currentPatternLength+1);


//-------------------------- RELAY SECTION ----------------------------

  if (currentYarn == 0) {
    currentRelay = 0;
    }
  else if (currentYarn == 1) {
    currentRelay = Relay1;
    }
  else if (currentYarn == 2) {
    currentRelay = Relay2;
    }
  else if (currentYarn == 3) {
    currentRelay = Relay3;
    }
  else if (currentYarn == 4) {
    currentRelay = Relay4;
    }
  else if (currentYarn == 5) {
    currentRelay = Relay5;
    }
  else if (currentYarn == 6) {
    currentRelay = Relay6;
    }

  digitalWrite(currentRelay, LOW);
  delay(100);
  digitalWrite(currentRelay, HIGH);

  
//------------------ Reset Counter at end of Array --------------------

  if (counter == currentPatternLength) 
  {
    counter = -1;
    }
}
}

