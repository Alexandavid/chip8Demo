//
// Created by Xandr on 2/8/2025.
//

#ifndef CHIP8EMULATOR_H
#define CHIP8EMULATOR_H

#include <qpushbutton.h>
#include <QTimer>
#include "Chip8Manager.h"

class Chip8EmulatorQtWrapper: public QObject {
    Q_OBJECT

    CHIP8Manager* chip8;
    public:
    explicit Chip8EmulatorQtWrapper(QObject* parent = nullptr, QtRenderer* renderer = nullptr);
    void loadROM(const char* filename);
    void handleInstruction();
    void handleEvents(QKeyEvent *event);
    void renderFrameToScreen();
    void handleSpecialRegisters();
    bool* getSystemStatus();

    CHIP8Manager* getChip8unwrapped();

    public slots:
    void updateEmulation();
    void updateScreen();

private:
    QtRenderer* renderer;
    QTimer emulationTimer;
    QTimer screenTimer;
    bool quit = false;
};

#endif //CHIP8EMULATOR_H
