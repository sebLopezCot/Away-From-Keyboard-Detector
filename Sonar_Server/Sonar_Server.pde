/* SONAR SERVER */
import processing.serial.*;

boolean screenSleeping = false;

Serial serial;
void setup()
{
  size(300, 100);
  serial = new Serial(this, "COM18", 9600);
  serial.bufferUntil('\n');
}

void draw()
{
  if(screenSleeping == true)
  {
    background(0,0,0);
  }
  else
  {
    background(255,255,255);
  }
}

void serialEvent(Serial serial)
{
  String input = serial.readStringUntil('\n');
  print(input);
  
  input = input.substring(0, input.length() - 1); // Take off line break
  int sleepIndex = input.indexOf("SLEEP");
  int wakeIndex = input.indexOf("WAKE");
  if(sleepIndex > -1) // Sleep
  {
    screenSleeping = true;
  }
  
  if(wakeIndex > -1) // Wake
  {
    screenSleeping = false;
  }
}
