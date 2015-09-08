#include "pixel_operations.h"

static inline Uint8 *pixelref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8 *)surf->pixels + y *surf->pitch + x *bpp;
}

Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixelref(surface, x, y);
    switch(surface->format->BytesPerPixel) {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32 *)p;
    }
    return 0;
}

void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixelref(surface, x, y);
    switch(surface->format->BytesPerPixel) {
        case 1:
            *p = pixel;
            break;
        case 2:
            *(Uint16 *)p = pixel;
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;
        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

SDL_Surface *display_image(SDL_Surface *img)
{
    SDL_Surface  *screen;
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (!screen)
    {
        printf("Couldn't set %dx%d video mode: %s\n",
               img->w, img->h, SDL_GetError());
        exit(-1);
    }
    
    if (SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        printf("BlitSurface error: %s\n", SDL_GetError());
    
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);
    
    return screen;
}

SDL_Surface* create_screen(int width, int height)
{
    SDL_Surface* img;
    img = SDL_CreateRGBSurface(0, width, height,32,0,0,0,1);
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
            putpixel(img, i, j, SDL_MapRGB(img->format, 255, 255, 255));
    
    if(!img)
    {
        printf("\033[31;1m [ERROR  ]\033[0m Couldn't create screen.\n");
        exit(-1);
    }
    return img;
}