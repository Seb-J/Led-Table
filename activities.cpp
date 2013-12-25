//HAS NOT BEEN TESTED YET !

#include "shared.h"
#include "activities.h"
#include "due_tlc5940.h"


void snake()
{
  int snake_speed = 300; //ms/pixel
  coordinate head = {4, 6}, tail = {3, 6}, apple;
  char directions[8][12];
  char content[8][12] = {EMPTY};
  content[head.x][head.y] = SNAKE;
  content[tail.x][tail.y] = SNAKE;
  directions[head.x][head.y] = RIGHT;
  directions[tail.x][tail.y] = RIGHT;
  delay(snake_speed);

  do
  {
    apple.x = random(8);
    apple.y = random(12);
  } while (content[apple.x][apple.y] != EMPTY);
  content[apple.x][apple.y] = APPLE;

  while (activity == 1)
  {
    //Update the head position
    if (directions[head.x][head.y] == UP) head.y = (head.y--) % 12;
    if (directions[head.x][head.y] == DOWN) head.y = (head.y++) % 12;
    if (directions[head.x][head.y] == RIGHT) head.x = (head.x++) % 8;
    if (directions[head.x][head.y] == LEFT) head.x = (head.x--) % 8;
    if (content[head.x][head.y] == APPLE)
    {
      do //Find the new apple's position
      {
        apple.x = random(8);
        apple.y = random(12);
      } while (content[apple.x][apple.y] != EMPTY);
      content[apple.x][apple.y] = APPLE;
    }
    else
    {
      //Update the tail position
      content[tail.x][tail.y] = EMPTY;
      if (directions[tail.x][tail.y] == UP) tail.y = (tail.y--) % 12;
      if (directions[tail.x][tail.y] == DOWN) tail.y = (tail.y++) % 12;
      if (directions[tail.x][tail.y] == RIGHT) tail.x = (tail.x++) % 8;
      if (directions[tail.x][tail.y] == LEFT) tail.x = (tail.x--) % 8;
    }
    content[head.x][head.y] = SNAKE;

    //Update the display
    coordinate pixel;
    for (pixel.x = 0; pixel.x < 8; pixel.x++)
    {
      for (pixel.y = 0; pixel.y < 12; pixel.y++)
      {
        if (content[pixel.x][pixel.y] == EMPTY) set_color(&pixel, 0, 0, 0);//Void is black
        if (content[pixel.x][pixel.y] == SNAKE) set_color(&pixel, 255, 0, 0);//Snake is red
        if (content[pixel.x][pixel.y] == APPLE) set_color(&pixel, 0, 255, 0);//Apple is green
      }
    }
    sendGSData();
    delay(snake_speed);
  }

}

void random_activity()
{
  int r, g, b;
  coordinate pixel;
  while (activity == 2)
  {
    for (pixel.x = 0; pixel.x < 8; pixel.x++)
    {
      for (pixel.y = 0; pixel.y < 12; pixel.y++)
      {
        r = getGSData(24 * pixel.y + 3 * pixel.x + RED) + (random(30) - 15);
        g = getGSData(24 * pixel.y + 3 * pixel.x + GREEN) + (random(30) - 15);
        b = getGSData(24 * pixel.y + 3 * pixel.x + BLUE) + (random(30) - 15);
        if (r > 255) r = 210;
        if (g > 255) g = 210;
        if (b > 255) b = 210;
        set_color(&pixel, r, g, b);
      }

    }
    sendGSData();
  }
}
