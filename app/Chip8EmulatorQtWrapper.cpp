//
// Created by Xandr on 2/8/2025.
//

#include "Chip8EmulatorQtWrapper.h"

#include <QPushButton>

#include "constants.h"

Chip8EmulatorQtWrapper::Chip8EmulatorQtWrapper(QObject *parent, QtRenderer *renderer_) : QObject(parent),
    renderer(renderer_) {
    connect(&emulationTimer, &QTimer::timeout, this, &Chip8EmulatorQtWrapper::updateEmulation);
    connect(&screenTimer, &QTimer::timeout, this, &Chip8EmulatorQtWrapper::updateScreen);

    chip8 = new CHIP8Manager(renderer,
                             SUPER_CHIP_WIDTH,SUPER_CHIP_HEIGHT,
                             "..\\audio\\beep.wav"
    );

    int frameDelay = ONE_SECOND / CHIP8_FREQUENCY;
    int screenUpdateDelay = ONE_SECOND / SCREEN_REFRESH_RATE;

    emulationTimer.start(frameDelay);
    screenTimer.start(screenUpdateDelay);
}


void Chip8EmulatorQtWrapper::loadROM(const char *filename) {
    chip8->loadROM(filename);
}

void Chip8EmulatorQtWrapper::handleInstruction() {
    chip8->handleInstruction();
}

void Chip8EmulatorQtWrapper::handleEvents(QKeyEvent *event) {
    chip8->handleEvents(event);
}

void Chip8EmulatorQtWrapper::renderFrameToScreen() {
    chip8->renderFrameToScreen();
}

void Chip8EmulatorQtWrapper::handleSpecialRegisters() {
    chip8->handleSpecialRegisters();
}

bool *Chip8EmulatorQtWrapper::getSystemStatus() {
    return &chip8->systemStatus;
}

CHIP8Manager *Chip8EmulatorQtWrapper::getChip8unwrapped() {
    return chip8;
}

void Chip8EmulatorQtWrapper::updateEmulation() {
    handleInstruction();
}

void Chip8EmulatorQtWrapper::updateScreen() {
    renderFrameToScreen();
    handleSpecialRegisters();
}
