#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <array>
#include <SDL.h>
#include <cstdint>

#include "../constants.h"
#include "GraphicsManager.h"

class KeyboardHandler {
    std::array<uint8_t, KEYCOUNT> keys{};
    SDL_Event event;
public:
    explicit KeyboardHandler() = default;

    void processInput(bool *exit, bool *systemStatus, GraphicsManager *graphics);

    bool wasKeyPressed(uint8_t key) const;

    bool wasKeyReleased(uint8_t key) const;

    void wasAnyKeyPressed(bool* systemStatus, uint8_t* v, uint8_t address) const;

    bool wasAnyKeyPressed() const;

private:
    void mapKeyToChip8(SDL_Keycode key, uint8_t state);
};

#endif
