//
// Created by Xandr on 2/8/2025.
//

#ifndef CHIP8EMULATOR_H
#define CHIP8EMULATOR_H

#include <qpushbutton.h>
#include <QTimer>
#include "Chip8Manager.h"

class Chip8EmulatorQtWrapper : public QObject {
    Q_OBJECT
    CHIP8Manager *chip8;

public:
    explicit Chip8EmulatorQtWrapper(QObject *parent = nullptr, QtRenderer *renderer = nullptr);

    void loadROM(const char *filename) const;

    void handleInstruction() const;

    void handleEvents(QKeyEvent *event) const;

    void renderFrameToScreen() const;

    void handleSpecialRegisters() const;

    bool *getSystemStatus() const;

    CHIP8Manager *getChip8unwrapped() const;

public slots:
    void updateEmulation() const;

    void updateScreen() const;

private:
    QtRenderer *renderer;
    QTimer emulationTimer;
    QTimer screenTimer;
};

#endif //CHIP8EMULATOR_H
