#include "KeyboardHandler.h"
#include "../CHIP8Manager.h"


void KeyboardHandler::processInput(bool* exit, bool* systemStatus, GraphicsManager* graphics) {
    int keystate = 0;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
            *exit = true;
            break;
        }

        if (event.type == SDL_KEYDOWN) {
            keystate = 1;
            switch (event.key.keysym.sym) {
                case SDLK_1: case SDLK_2: case SDLK_3: case SDLK_4:
                case SDLK_q: case SDLK_w: case SDLK_e: case SDLK_r:
                case SDLK_a: case SDLK_s: case SDLK_d: case SDLK_f:
                case SDLK_z: case SDLK_x: case SDLK_c: case SDLK_v:
                    *systemStatus = RUNNING;
                break;
                default: break;
            }
        }
        else if (event.type == SDL_KEYUP) {
             keystate = 0;
        }
        else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                graphics->setDisplaySize(event.window.data1, event.window.data2);
            }
        }

        mapKeyToChip8(event.key.keysym.sym, keystate);
    }
}


void KeyboardHandler::wasAnyKeyPressed(bool* systemStatus, uint8_t* v, uint8_t address) const {
    *systemStatus = HALT;
    for (int key = 0; key < KEYCOUNT; key++) {
        if (keys[key]) {
            v[address] = keys[key];
            *systemStatus = RUNNING;
            break;
        }
    }
}

bool KeyboardHandler::wasAnyKeyPressed() const {
    for (int key = 0; key < KEYCOUNT; key++) {
        if (keys[key]) {
            return true;
        }
    }
    return false;
}

bool KeyboardHandler::wasKeyPressed(uint8_t key) const {
    return keys[key] == 1;
}

bool KeyboardHandler::wasKeyReleased(uint8_t key) const {
    return keys[key] == 0;
}

void KeyboardHandler::mapKeyToChip8(SDL_Keycode key, uint8_t state) {
    /*
         Chip-8 ( Keypad Mapping to Keyboard )
         Keypad             Keyboard
         +-+-+-+-+          +-+-+-+-+
         |1|2|3|C|          |1|2|3|4|
         +-+-+-+-+          +-+-+-+-+
         |4|5|6|D|          |Q|W|E|R|
         +-+-+-+-+ <<<==>>> +-+-+-+-+
         |7|8|9|E|          |A|S|D|F|
         +-+-+-+-+          +-+-+-+-+
         |A|0|B|F|          |Z|X|C|V|
         +-+-+-+-+          +-+-+-+-+
         */

    switch (key) {
        case SDLK_1: keys[0x1] = state; break;
        case SDLK_2: keys[0x2] = state; break;
        case SDLK_3: keys[0x3] = state; break;
        case SDLK_4: keys[0xC] = state; break;
        case SDLK_q: keys[0x4] = state; break;
        case SDLK_w: keys[0x5] = state; break;
        case SDLK_e: keys[0x6] = state; break;
        case SDLK_r: keys[0xD] = state; break;
        case SDLK_a: keys[0x7] = state; break;
        case SDLK_s: keys[0x8] = state; break;
        case SDLK_d: keys[0x9] = state; break;
        case SDLK_f: keys[0xE] = state; break;
        case SDLK_z: keys[0xA] = state; break;
        case SDLK_x: keys[0x0] = state; break;
        case SDLK_c: keys[0xB] = state; break;
        case SDLK_v: keys[0xF] = state; break;
        default: break;
    }
}


