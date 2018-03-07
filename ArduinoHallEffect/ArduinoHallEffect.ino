/*
  Arduino Hall Effect Sensor Project
  by Brian Bruce

  Code copied from:
  by Arvind Sanjeev
  Please check out  http://diyhacking.com for the tutorial of this project.
  DIY Hacking
*/

// Define pin used for the hall effect sensor out.  Must be an interrupt pin.
// For Mega can be 2, 3, 18, 19, 20, 21
const byte interruptPin = 18; 

// Counter for sensor.  Must be volatile if used in an interrupt function.
volatile byte counter;

// Array used to drive a 7 segment LED display to show the counter
int num_array[10][7] = {  { 1,1,1,1,1,1,0 },    // 0
                          { 0,1,1,0,0,0,0 },    // 1
                          { 1,1,0,1,1,0,1 },    // 2
                          { 1,1,1,1,0,0,1 },    // 3
                          { 0,1,1,0,0,1,1 },    // 4
                          { 1,0,1,1,0,1,1 },    // 5
                          { 1,0,1,1,1,1,1 },    // 6
                          { 1,1,1,0,0,0,0 },    // 7
                          { 1,1,1,1,1,1,1 },    // 8
                          { 1,1,1,0,0,1,1 }};   // 9

//function header
void Num_Write(int);

void setup()
{
  // Must initialize Serial to see Serial.println output
  Serial.begin(115200);

  // Set up interrupt function to be called when the signal on the arduino interrupt pin rises.
  attachInterrupt(digitalPinToInterrupt(interruptPin), magnet_detect, RISING);
  
  // Set pin modes for the led counter display
  pinMode(2, OUTPUT);   
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  
  Serial.println("Starting dude!");
}

void loop() //Measure RPM
{
  // Nothing in the loop.  Using an interrupt function instead.
  
  // Use millis() function to get current time in millis for calculating elapsed times and speeds, etc.
}

//This function is called whenever a magnet/interrupt is detected by the arduino.
void magnet_detect() 
{
  counter++;
  Serial.print("Detected: ");
  Serial.println(counter);
  
  // LED counter
  if (counter > 9) { counter = 0; } // Can only show the ones digit for now.
  Num_Write(counter);
}

// this functions writes values 0-9 to the seven seg led pins  
void Num_Write(int number) 
{
  int pin= 2;
  for (int j=0; j < 7; j++) {
   digitalWrite(pin, num_array[number][j]);
   pin++;
  }
}

