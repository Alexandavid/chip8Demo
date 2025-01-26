#include "GraphicsDrivers.h"
#include <cstring>
#include <stdexcept>


SDLManager::SDLManager() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error("SDL_Init failed: " + std::string(SDL_GetError()));
    }
}

SDLManager::~SDLManager() {
    SDL_Quit();
    SDL_Log("SDL quit");
}


Window::Window(const char *title, int width, int height, uint32_t flags) {
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (!window) {
        throw std::runtime_error("SDL_CreateWindow failed: " + std::string(SDL_GetError()));
    }
    std::strcpy(windowTitle, title);
    w = width;
    h = height;
}

Window::~Window() {
    SDL_DestroyWindow(window);
    SDL_Log("Window cleaned");
}

void Window::recreateWindow(char *title, int x, int y, int width, int height, uint32_t flags) {
    SDL_DestroyWindow(window);
    window = SDL_CreateWindow(title, x, y, width, height, flags);
    if (!window) {
        throw std::runtime_error("SDL_CreateWindow failed: " + std::string(SDL_GetError()));
    }
}

SDL_Window *Window::get() {
    return window;
}

// Renderer implementation
Renderer::Renderer(SDL_Window *win) : window(win) {
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        throw std::runtime_error("SDL_CreateRenderer failed: " + std::string(SDL_GetError()));
    }
}

Renderer::~Renderer() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
}

void Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Renderer::clear() {
    SDL_RenderClear(renderer);
}

void Renderer::updateWindow() {
    SDL_RenderPresent(renderer);
}

SDL_Renderer *Renderer::get() {
    return renderer;
}

void Renderer::drawPixel(SDL_Rect *rect) {
    SDL_RenderFillRect(renderer, rect);
}
