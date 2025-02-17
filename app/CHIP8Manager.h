#ifndef CHIP8_MANAGER_H
#define CHIP8_MANAGER_H

#include <cstdint>
#include <qpushbutton.h>

#include "constants.h"
#include "Peripherials/AudioManager.h"
#include "Peripherials/GraphicsManager.h"
#include "Peripherials/KeyboardHandler.h"

class CHIP8Specification {

public:
    uint8_t memory[4 * KB];     // ROM OF 4KB
    uint16_t PC;                // Program counter
    uint8_t V[NUM_OF_REGISTER]; // General purpose 8-bit registers
    uint16_t I;                 // Memory address register
    uint8_t delayTimerRegister;
    uint8_t soundRegister;
    uint16_t stack[STACK_SIZE];  // Stack for subroutines
    uint8_t sp;                  // Stack pointer
    uint8_t frame[DISPLAY_SIZE]; // Display frame buffer
};

class CHIP8Manager {
    CHIP8Specification context;
    QtRenderer *renderer_;
    KeyboardHandler keypad;
    GraphicsManager gui;
    AudioManager audio;
    uint16_t instruction{};
    uint16_t PCRegisterRecord = 0xFFFF;

public:
    bool systemStatus = RUNNING;

    CHIP8Manager(QtRenderer* renderer,
        uint32_t width,
        uint32_t height,
        const char* soundfile
    );

    ~CHIP8Manager();

    bool loadROM(const char *filename);

    void handleArithmetic(uint8_t VxAddress, uint8_t Vx, uint8_t Vy);

    void handleKeyEvents(uint8_t Vx);

    void handleTimeAndMemory(uint8_t VxAddress, uint8_t Vx);

    void handleInstruction(uint16_t forcedInstruction = 0);

    void handleEvents(QKeyEvent *event);

    void drawOnFrame(uint8_t Vx, uint8_t Vy, uint8_t n);

    void renderFrameToScreen();

    void handleSpecialRegisters();

    CHIP8Specification *getChip8HardwareContext();

private:
    void holdUntilClick(uint8_t VxAddress);

    void loadFontToMemory();

    uint8_t byteRand();
};

#endif //CHIP8_MANAGER_H
