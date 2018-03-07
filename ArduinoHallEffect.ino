/*
  Arduino Hall Effect Sensor Project
  by Arvind Sanjeev
  Please check out  http://diyhacking.com for the tutorial of this project.
  DIY Hacking
*/

const byte interruptPin = 18; // For Mega can be 2, 3, 18, 19, 20, 21
volatile byte counter;
volatile byte half_revolutions;
unsigned int rpm;
unsigned long timeold;

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
  Serial.begin(115200);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin), magnet_detect, RISING);//Initialize the intterrupt pin (Arduino digital pin 2)
  
  // set pin modes
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

void magnet_detect() //This function is called whenever a magnet/interrupt is detected by the arduino.
{
  counter++;
  Serial.print("Detected: ");
  Serial.println(counter);
  // LED counter
  if (counter > 9) { counter = 0; }
  Num_Write(counter);
}

// this functions writes values to the sev seg pins  
void Num_Write(int number) 
{
  int pin= 2;
  for (int j=0; j < 7; j++) {
   digitalWrite(pin, num_array[number][j]);
   pin++;
  }
}

