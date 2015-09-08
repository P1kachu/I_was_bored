#ifndef PIXEL_OPERATIONS_H
#  define PIXEL_OPERATIONS_H
 
#  include <stdlib.h>
#  include <SDL.h>

Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y);
void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
SDL_Surface *display_image(SDL_Surface *img);
SDL_Surface* create_screen(int width, int height); 

# endif
