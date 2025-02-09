#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <array>
#include <cstdint>

#include "../constants.h"
#include "GraphicsManager.h"

#include <QKeyEvent>
#include <qpushbutton.h>

class KeyboardHandler {
    public:
    std::array<uint8_t, KEYCOUNT> keys{};

    explicit KeyboardHandler() = default;

    void processInput(QKeyEvent *qtEvent, bool *systemStatus);

    bool wasKeyPressed(uint8_t key) const;

    bool wasKeyReleased(uint8_t key) const;

    void wasAnyKeyPressed(bool* systemStatus, uint8_t* v, uint8_t address) const;

    bool wasAnyKeyPressed() const;

private:
    void mapKeyToChip8(QKeyEvent *event, uint8_t state);
};

#endif
