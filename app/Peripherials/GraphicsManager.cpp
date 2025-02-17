#include "GraphicsManager.h"

#include <cstdint>
#include "../constants.h"

void GraphicsManager::renderFrameToScreen() {
    uint32_t scaleX = screenWidth / CHIP8_WIDTH;
    uint32_t scaleY = screenHeight / CHIP8_HEIGHT;

    qtrenderer->setDrawColor(Qt::white);

    for (uint8_t y = 0; y < CHIP8_HEIGHT; y++) {
        for (uint8_t x = 0; x < CHIP8_WIDTH; x++) {
            if (framebuffer[y][x] == 1) {
                int pixelX = x * scaleX;
                int pixelY = y * scaleY;
                qtrenderer->drawPixel(pixelX, pixelY, scaleX, scaleY);
            }
        }
    }

    qtrenderer->repaint();
}

void GraphicsManager::drawSprite(uint8_t* memory, uint8_t* V, uint16_t I,uint8_t Vx, uint8_t Vy, uint8_t n) {
    V[0x0F] = 0;
    uint8_t nbits = 8 * sizeof(uint8_t);

    for (uint8_t row = 0; row < n; row++) {
        uint8_t spriteByte = memory[I + row];
        for (uint8_t col = 0; col < nbits; col++) {
            const uint8_t spritePixel = (spriteByte >> (nbits - (1 + col))) & 0x1;

            const uint8_t pixelX = (Vx + col) % CHIP8_WIDTH;
            const uint8_t pixelY = (Vy + row) % CHIP8_HEIGHT;

            uint8_t screenPixel = framebuffer[pixelY][pixelX];

            if (spritePixel == 1 && screenPixel == 1) {
                V[0xF] = 1;
            }

            framebuffer[pixelY][pixelX] ^= spritePixel;
        }
    }
}

void GraphicsManager::clearVideoBuffer() {
    framebuffer.fill({});
}

void GraphicsManager::clearDisplay() {
    clearVideoBuffer();
    qtrenderer->clear();
}



