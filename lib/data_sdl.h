#ifndef _DATA_SDL_H_
#define _DATA_SDL_H_

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>   

#define LONG 1080
#define HAUT 720

int mainMenu(SDL_Renderer * pRenderer, SDL_Window * pWindow);
void startNewGame();
int shop(SDL_Renderer * Render, SDL_Window * Window);

void initRect(SDL_Renderer * render ,SDL_Rect * rect, int x, int y , int w, int h,int r,int g,int b);
void initText(SDL_Renderer * render, SDL_Rect rect, SDL_Color color, char * mess, TTF_Font * font);
int isOnButton(SDL_Rect rect);
void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius);

#endif