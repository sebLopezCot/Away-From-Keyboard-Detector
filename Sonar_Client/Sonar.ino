/* SONAR PROGRAM */
int trig = 7;
int echo = 8;
int gLed = 10;
int yLed = 11;

double chairDist;

void setup() 
{
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(gLed, OUTPUT);
  pinMode(yLed, OUTPUT);
  
  // Calibrate Sitting Distance
  Serial.println("Please step away from the chair.");
  Serial.println("Now place your hand near the sensor to calibrate the chair distance.");
  Serial.println("The lights will blink 3 times, show a solid light, and turn off.");
  Serial.println("Once this finishes, the sensor will be calibrated.");
  while(getDistance(trig, echo) > 3) delay(500); // Wait for hand
  Serial.println("\nSTARTING CALIBRATION");
  // Flash leds
  analogWriteLeds(127);
  delay(500);
  analogWriteLeds(0);
  delay(500);
  analogWriteLeds(127);
  delay(500);
  analogWriteLeds(0);
  delay(500);
  analogWriteLeds(127);
  delay(500);
  analogWriteLeds(0);
  delay(500);
  // Solid light
  analogWriteLeds(127);
  delay(750);
  chairDist = getDistance(trig, echo); // Calibrate chair distance
  Serial.print("Chair Distance: ");
  printDouble(chairDist, 3);
  Serial.print(" cm");
  Serial.println();
  delay(750);
  analogWriteLeds(0);
  Serial.println("CHAIR DISTANCE CALIBRATED");
  delay(1000);
}

boolean userPresent = false;
boolean screenSleeping = false;
long startTimeAway = 0;
void loop() 
{ 
  double dist = getDistance(trig, echo);
  // Check to see if user is present
  if(dist >= chairDist && userPresent == true) // Not in chair, but was before
  {
    // Led Output (Yellow)
    analogWrite(yLed, 127);
    delay(175);
    analogWrite(yLed, 0);
    delay(175);
    analogWrite(yLed, 127);
    delay(175);
    analogWrite(yLed, 0);
    startTimeAway = millis(); // Make startTimeAway equal to current amount of milliseconds passed
    userPresent = false;
  }
  else if(dist < chairDist && userPresent == false) // In chair, but wasn't before
  {
    // Led Output (Green)
    analogWrite(gLed, 127);
    delay(175);
    analogWrite(gLed, 0);
    delay(175);
    analogWrite(gLed, 127);
    delay(175);
    analogWrite(gLed, 0);
    // Wake screen
    if(screenSleeping == true)
    {
      Serial.println("WAKE");
      screenSleeping = false;
    }
    startTimeAway = 0; // Reset startTimeAway
    userPresent = true;
  }
  
  // Send signal for screen sleep if user is gone for too long
  long timeAway = millis() - startTimeAway;
  if(timeAway >= 1000 * 30 && timeAway < millis() && screenSleeping == false)
  {
    Serial.println("SLEEP");
    screenSleeping = true;
  }
  delay(5000);
}

double getDistance(int trigPin, int echoPin)
{
  long duration;
  double distance;
  digitalWrite(trigPin, HIGH); // Fire ping
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Wait for pin to go high and time how long it takes to go low
  distance = (duration/2) / 29.1;
  return distance;
}

void analogWriteLeds(int value)
{
  analogWrite(yLed, value);
  analogWrite(gLed, value);
}

void printDouble(double val, byte precision){
  Serial.print (int(val));  //prints the int part
  if( precision > 0) {
    Serial.print("."); // print the decimal point
    unsigned long frac;
    unsigned long mult = 1;
    byte padding = precision -1;
    while(precision--)
       mult *=10;
       
    if(val >= 0)
      frac = (val - int(val)) * mult;
    else
      frac = (int(val)- val ) * mult;
    unsigned long frac1 = frac;
    while( frac1 /= 10 )
      padding--;
    while(  padding--)
      Serial.print("0");
    Serial.print(frac,DEC) ;
  }
}
