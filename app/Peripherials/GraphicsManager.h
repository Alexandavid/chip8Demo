#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include <array>
#include "../constants.h"
#include "../SDLAbstractLayer/GraphicsDrivers.h"

class GraphicsManager {
    SDLManager sdlManager;
    Window window;
    Renderer renderer;
    uint32_t screenWidth, screenHeight;
    std::array<std::array<uint8_t, CHIP8_WIDTH>, CHIP8_HEIGHT> framebuffer = {};
    uint32_t width_;

public:
    explicit GraphicsManager(const char *title, const uint32_t width, const uint32_t height):
        window(title, width, height, SDL_WINDOW_RESIZABLE),
        renderer(window.get()), width_(width) {
        screenWidth = width;
        screenHeight = height;
    }

    void renderFrameToScreen();

    void drawSprite(uint8_t* mem, uint8_t* vRegisters, uint16_t I,uint8_t Vx, uint8_t Vy, uint8_t n);

    void clearDisplay();

    void setDisplaySize(uint16_t width, uint16_t height);

private:
    void clearVideoBuffer();
};

#endif
