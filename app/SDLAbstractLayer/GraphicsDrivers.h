#ifndef HL_SDL_H
#define HL_SDL_H

#include <SDL.h>

class SDLManager {
public:
    SDLManager();

    ~SDLManager();
};

class Window {
    SDL_Window *window;
    char windowTitle[100]{};

public:
    int w, h;

    Window(const char *title, int width, int height, uint32_t flags);

    ~Window();

    void recreateWindow(char *title, int x, int y, int width, int height, uint32_t flags);

    SDL_Window *get();
};

class Renderer {
    SDL_Renderer *renderer;
    SDL_Window *window;

public:
    Renderer(SDL_Window *win);

    ~Renderer();

    void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    void clear();

    void updateWindow();

    SDL_Renderer *get();

    void drawPixel(SDL_Rect *rect);
};

#endif //HL_SDL_H
