#include <chrono>
#include <fstream>
#include <SDL.h>
#include <iostream>
#include "constants.h"
#include "CHIP8Manager.h"

#define ONE_SECOND 1000
#define CHIP8_FREQUENCY 60 //Hz
#define SCREEN_REFRESH_RATE 60

void chip8demo_app();
void println(const char* str);


CHIP8Manager chip8 = CHIP8Manager("CHIP-8 EMULATOR", SUPER_CHIP_WIDTH, SUPER_CHIP_HEIGHT);
uint8_t screenHeight, screenWidth;
SDL_Event event;
static bool quit = false;

int main() {
    try {
        chip8demo_app();
    } catch (const std::exception &e) {
        println(e.what());
        return 1;
    }
    return 0;
}


void chip8demo_app() {
    const char *romFile = "../games/TicTacToe.ch8";
    const int frameDelay = ONE_SECOND / CHIP8_FREQUENCY;
    const int screenUpdateDelay = ONE_SECOND / SCREEN_REFRESH_RATE;
    uint32_t lastTime = SDL_GetTicks();
    uint32_t lastScreenUpdate = SDL_GetTicks();

    chip8.loadROM(romFile);

    while (!quit) {
        uint32_t currentTime = SDL_GetTicks();

        if (currentTime - lastTime >= frameDelay) {
            lastTime = currentTime;
            chip8.handleInstruction(0);

        }

        // Only update the screen at 60Hz
        if (currentTime - lastScreenUpdate >= screenUpdateDelay) {
            chip8.handleEvents(&quit);
            lastScreenUpdate = currentTime;
            chip8.renderFrameToScreen();
            chip8.handleSpecialRegisters();
        }
    }
}

void println(const char* str) {
    std::cout << str << std::endl;
}
