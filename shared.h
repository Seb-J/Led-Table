#ifndef DEF_SHARED
#define DEF_SHARED

#define RED 0
#define GREEN 1
#define BLUE 2

#define UP 1
#define DOWN -1
#define RIGHT -2
#define LEFT 2

#define EMPTY 0
#define SNAKE 1
#define APPLE 2



extern unsigned char activity;
extern unsigned char action;

typedef struct coordinate coordinate;
struct coordinate
{
    int x;
    int y;
};


void set_color(coordinate* pixel, unsigned char red, unsigned char green, unsigned char blue);
void stop_activity();

#endif

