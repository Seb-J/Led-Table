//HAS NOT BEEN TESTED YET !

#include "shared.h"
#include "activities.h"
#include "due_tlc5940.h"

//8 case long side : X
//12 case long side : Y


//Setup
void setup()
{
  Serial1.begin(9600);
  initTLC5940();
  randomSeed(analogRead(0) + 4564459);

  //Only works with an Arduino Due.
  attachInterrupt(19, stop_activity, CHANGE);
}


//Main Loop
void loop()
{
  switch (activity)
  {
    case 1:
      snake();
      break;

    case 2:
      random_activity();
      break;

      //etc..

  }
}







//Functions :

void set_color(coordinate* pixel, unsigned char red, unsigned char green, unsigned char blue)
{
  int led_case = 24 * pixel->y + 3 * pixel->x;
  setGSData(led_case + RED, red << 4); //The TLC5940 takes a value between 0 and 4095 while red is a value between 0 and 255
  setGSData(led_case + GREEN, green << 4);
  setGSData(led_case + BLUE, blue << 4);
}

//TO BE TESTED
void stop_activity()
{
  while (Serial1.available() > 0)
  {
    //Two byte should arrive :
    //If the first byte is different from 0 the activity has to be changed.
    //The second byte is for interactive games such as Snake
    byte incoming_byte = Serial1.read();
    if (incoming_byte != 0) activity = incoming_byte;
    //Second byte
    incoming_byte = Serial1.read();
    action = incoming_byte;
  }

}


