#include "KeyboardHandler.h"
#include "../CHIP8Manager.h"


void KeyboardHandler::processInput(QKeyEvent* qtEvent, bool* systemStatus, GraphicsManager* graphics) {
    if (!qtEvent) return;

    int keystate = qtEvent->type() == QEvent::KeyPress;

    switch (qtEvent->key()) {
        case Qt::Key_1: case Qt::Key_2: case Qt::Key_3: case Qt::Key_4:
        case Qt::Key_Q: case Qt::Key_W: case Qt::Key_E: case Qt::Key_R:
        case Qt::Key_A: case Qt::Key_S: case Qt::Key_D: case Qt::Key_F:
        case Qt::Key_Z: case Qt::Key_X: case Qt::Key_C: case Qt::Key_V:
            *systemStatus = RUNNING;
        break;
        default:
            break;
    }

    mapKeyToChip8(qtEvent, keystate);
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

void KeyboardHandler::mapKeyToChip8(QKeyEvent* event, uint8_t state) {
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

    switch (event->key()) {
        case Qt::Key_1: keys[0x1] = state; break;
        case Qt::Key_2: keys[0x2] = state; break;
        case Qt::Key_3: keys[0x3] = state; break;
        case Qt::Key_4: keys[0xC] = state; break;
        case Qt::Key_Q: keys[0x4] = state; break;
        case Qt::Key_W: keys[0x5] = state; break;
        case Qt::Key_E: keys[0x6] = state; break;
        case Qt::Key_R: keys[0xD] = state; break;
        case Qt::Key_A: keys[0x7] = state; break;
        case Qt::Key_S: keys[0x8] = state; break;
        case Qt::Key_D: keys[0x9] = state; break;
        case Qt::Key_F: keys[0xE] = state; break;
        case Qt::Key_Z: keys[0xA] = state; break;
        case Qt::Key_X: keys[0x0] = state; break;
        case Qt::Key_C: keys[0xB] = state; break;
        case Qt::Key_V: keys[0xF] = state; break;
        default: break;
    }
}


