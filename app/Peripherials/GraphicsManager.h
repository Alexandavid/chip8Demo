#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include <array>
#include "../constants.h"
#include "../Drivers/GraphicsDrivers.h"

class GraphicsManager {
    QtRenderer *qtrenderer;
    uint32_t screenWidth, screenHeight;
    std::array<std::array<uint8_t, CHIP8_WIDTH>, CHIP8_HEIGHT> framebuffer = {};
    uint32_t width_;

public:
    explicit GraphicsManager(QtRenderer *qt_renderer, const uint32_t width, const uint32_t height):
        qtrenderer(qt_renderer)
    {
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
