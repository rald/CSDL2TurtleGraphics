#ifndef TURTLE_H
#define TURTLE_H

#include <SDL2/SDL.h>

#include <stdbool.h>
#include <math.h>
#include <time.h>

#ifdef TURTLE_IMPLEMENTATION_H

static SDL_Renderer *renderer;
static SDL_Window *window;
static SDL_Texture *layer_top;
static SDL_Texture *layer_bottom;

static SDL_Point points[] = {
    {0, 0},
    {-1, +1},
    {+2, 0},
    {-1, -1},
    {0, 0},
};

static size_t npoints = 5;

typedef struct Turtle Turtle;

struct Turtle
{
    double x, y;
    double heading;
    double size;
    bool isVisible;
    SDL_Color *penColor;
    bool isPenDown;

    SDL_Point *points;
    size_t npoints;
};

static Turtle **turtles = NULL;
static size_t nturtles = 0;

void CreateTurtleWorld(int *w, int *h);
void DestroyTurtleWorld();
void UpdateTurtleWorld();
void PauseTurtleWorld(SDL_Event *event);

Turtle *CreateTurtle(double x, double y, double h, double size, SDL_Color *penColor, bool isVisible);

void DrawTurtles(size_t nturtles, Turtle **turtles);

double drand();
double rnd(double n);
double sgn(double x);

void RotatePoints(size_t n, SDL_Point *p, SDL_Point c, double a);
static void plot(Turtle *turtle, int x, int y);
static void line(Turtle *turtle, int x1, int y1, int x2, int y2);

void Move(Turtle *turtle, double distance);
void Turn(Turtle *turtle, double angle);
void Jump(Turtle *turtle, double newX, double newY);
void Hide(Turtle *turtle);
void Show(Turtle *turtle);
void PenUp(Turtle *turtle);
void PenDown(Turtle *turtle);

void CreateTurtleWorld(int *w, int *h)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(*w, *h, 0, &window, &renderer);
    SDL_GetWindowSize(window, w, h);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    layer_top = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, *w, *h);

    layer_bottom = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, *w, *h);
}

void DestroyTurtleWorld()
{
    SDL_DestroyTexture(layer_top);
    SDL_DestroyTexture(layer_bottom);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void UpdateTurtleWorld()
{
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, layer_bottom, NULL, NULL);
    SDL_RenderCopy(renderer, layer_top, NULL, NULL);
    SDL_RenderPresent(renderer);
    DrawTurtles(nturtles,turtles);
}

void AddTurtle(size_t *nturtles, Turtle ***turtles, Turtle *turtle)
{
    (*turtles) = realloc(*turtles, sizeof(*turtles) * (*nturtles + 1));
    if (*turtles)
    {
        (*turtles)[(*nturtles)++] = turtle;
    }
    else
    {
        printf("Error: AddTurtle: realloc\n");
        exit(1);
    }
}

Turtle *CreateTurtle(double x, double y, double h, double size, SDL_Color *penColor, bool isVisible)
{
    Turtle *turtle = malloc(sizeof(*turtle));
    if (turtle)
    {
        turtle->x = x;
        turtle->y = y;
        turtle->heading = h;
        turtle->size = size;
        turtle->penColor = penColor;
        turtle->isVisible = isVisible;
        turtle->isPenDown = true;

        AddTurtle(&nturtles, &turtles, turtle);
    }
    else
    {
        printf("Error: CreateTurtle: malloc: turtle\n");
        exit(1);
    }
    return turtle;
}

void DestroyTurtle(Turtle **turtle)
{
    free(*turtle);
    (*turtle) = NULL;
}

void RotatePoints(size_t n, SDL_Point *p, SDL_Point c, double a)
{
    double cs = cos(a);
    double sn = sin(a);
    double temp;
    for (int i = 0; i < n; i++)
    {
        temp = ((p[i].x - c.x) * cs - (p[i].y - c.y) * sn) + c.x;
        p[i].y = ((p[i].x - c.x) * sn + (p[i].y - c.y) * cs) + c.y;
        p[i].x = temp;
    }
}

void DrawTurtles(size_t nturtles, Turtle **turtles)
{
    SDL_SetRenderTarget(renderer, layer_top);
    SDL_SetTextureBlendMode(layer_top, SDL_BLENDMODE_ADD);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    for (int i = 0; i < nturtles; i++)
    {
        if (turtles[i]->isVisible)
        {
            SDL_Point *w = malloc(sizeof(*w) * npoints);
            SDL_Point c;

            for (int j = 0; j < npoints; j++)
            {
                w[j].x = points[j].x * turtles[i]->size + turtles[i]->x;
                w[j].y = points[j].y * turtles[i]->size + turtles[i]->y;
            }

            c.x = w[0].x;
            c.y = w[0].y;

            RotatePoints(npoints, w, c, turtles[i]->heading * M_PI / 180);

            SDL_SetRenderDrawColor(renderer, turtles[i]->penColor->r, turtles[i]->penColor->g, turtles[i]->penColor->b, turtles[i]->penColor->a);

            SDL_RenderDrawLines(renderer, w, npoints);
        }
    }
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, layer_bottom, NULL, NULL);
    SDL_RenderCopy(renderer, layer_top, NULL, NULL);
    SDL_RenderPresent(renderer);
}

double drand()
{
    return rand() / (RAND_MAX + 1.0);
}

double rnd(double x)
{
    return floor(drand() * x);
}

double sgn(double x)
{
    if (x > 0.0)
        return 1.0;
    if (x < 0.0)
        return -1.0;
    return x;
}

static void plot(Turtle *turtle, int x, int y)
{
    turtle->x = x;
    turtle->y = y;

    SDL_SetRenderTarget(renderer, layer_bottom);
    SDL_SetTextureBlendMode(layer_bottom, SDL_BLENDMODE_ADD);
    SDL_SetRenderDrawColor(renderer, turtle->penColor->r, turtle->penColor->g, turtle->penColor->b, turtle->penColor->a);
    SDL_RenderDrawPoint(renderer, turtle->x, turtle->y);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, layer_bottom, NULL, NULL);
    SDL_RenderCopy(renderer, layer_top, NULL, NULL);
    SDL_RenderPresent(renderer);

    DrawTurtles(nturtles, turtles);
}

static void line(Turtle *turtle, int x1, int y1, int x2, int y2)
{
    int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;

    dx = x2 - x1; /* the horizontal distance of the line */
    dy = y2 - y1; /* the vertical distance of the line */
    dxabs = abs(dx);
    dyabs = abs(dy);
    sdx = sgn(dx);
    sdy = sgn(dy);
    x = dyabs >> 1;
    y = dxabs >> 1;
    px = x1;
    py = y1;

    plot(turtle, px, py);

    if (dxabs >= dyabs)
    { /* the line is more horizontal than vertical */
        for (i = 0; i < dxabs; i++)
        {
            y += dyabs;
            if (y >= dxabs)
            {
                y -= dxabs;
                py += sdy;
            }
            px += sdx;
            plot(turtle, px, py);
        }
    }
    else
    { /* the line is more vertical than horizontal */
        for (i = 0; i < dyabs; i++)
        {
            x += dxabs;
            if (x >= dyabs)
            {
                x -= dyabs;
                px += sdx;
            }
            py += sdy;
            plot(turtle, px, py);
        }
    }
}

void Move(Turtle *turtle, double distance)
{
    double newX = distance * cos(turtle->heading * M_PI / 180) + turtle->x;
    double newY = distance * sin(turtle->heading * M_PI / 180) + turtle->y;
    if (turtle->isPenDown)
    {
        line(turtle, turtle->x, turtle->y, newX, newY);
    }
    turtle->x = newX;
    turtle->y = newY;
}

void Turn(Turtle *turtle, double angle)
{
    for (int i = 0; i < fabs(angle); i++)
    {
        turtle->heading += (angle >= sgn(angle) ? sgn(angle) : angle);
        DrawTurtles(nturtles, turtles);
    }
}

void Jump(Turtle *turtle, double newX, double newY)
{
    if (turtle->isPenDown)
    {
        line(turtle, turtle->x, turtle->y, newX, newY);
    }
    turtle->x = newX;
    turtle->y = newY;
}

void Hide(Turtle *turtle)
{
    turtle->isVisible = false;
    DrawTurtles(nturtles, turtles);
}

void Show(Turtle *turtle)
{
    turtle->isVisible = true;
    DrawTurtles(nturtles, turtles);
}

void PenUp(Turtle *turtle)
{
    turtle->isPenDown = false;
}

void PenDown(Turtle *turtle)
{
    turtle->isPenDown = true;
}

void PenColor(Turtle *turtle, SDL_Color *penColor)
{
    turtle->penColor = penColor;
}

void PauseTurtleWorld(SDL_Event *event)
{
    while (1)
    {
        if (SDL_PollEvent(event) && event->type == SDL_QUIT)
        {
            break;
        }
    }
}

#endif

#endif
