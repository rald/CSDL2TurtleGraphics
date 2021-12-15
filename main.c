#include <SDL2/SDL.h>

#define TURTLE_IMPLEMENTATION_H

#include "turtle.h"

int w = 640, h = 480;

void DrawStar(Turtle *turtle, double d)
{
    for (int i = 0; i < 5; i++)
    {
        Move(turtle, d);
        Turn(turtle, 144);
    }
}

int main(int argc, char *argv[])
{
    SDL_Event event;

    SDL_Color red={ 255,0,0,255 };
    SDL_Color green={ 0,255,0,255 };

    Turtle *turtle1 = NULL;
    Turtle *turtle2 = NULL;

    srand(time(NULL));

    CreateTurtleWorld(&w, &h);


    turtle1 = CreateTurtle((w-300)/2, h/2, 0, 10, &green, true);

    turtle2 = CreateTurtle((w-300)/2, h/2-500, 0, 10, &red, true);

    DrawStar(turtle1, 300);

    Hide(turtle1);

    DrawStar(turtle2, 300);

    Hide(turtle2);

    UpdateTurtleWorld();

    PauseTurtleWorld(&event);

    DestroyTurtle(&turtle2);
    DestroyTurtle(&turtle1);

    DestroyTurtleWorld();

    return EXIT_SUCCESS;
}
