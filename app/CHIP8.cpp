#include "CHIP8Manager.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <Windows.h>
#include <sys/stat.h>

CHIP8Manager::CHIP8Manager(
    const char *title
    , uint32_t width
    , uint32_t height
    , const char* soundFile
    ) : context(), gui(title, width, height), audio(soundFile) {}

CHIP8Manager::~CHIP8Manager() = default;

bool CHIP8Manager::loadROM(const char *filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    file.seekg(0, std::ios::end);
    const uint32_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (ADDRESS_PROGRAM_START + fileSize > sizeof(context.memory)) {
        std::cerr << "Error: File size (" << fileSize << " bytes) exceeds memory bounds." << std::endl;
        return false;
    }


    file.read(reinterpret_cast<char *>(context.memory + ADDRESS_PROGRAM_START), fileSize);

    if (!file) {
        std::cerr << "Error reading file: " << filename << std::endl;
        return false;
    }

    loadFontToMemory();
    context.PC = ADDRESS_PROGRAM_START;
    context.sp = 0;
    for (unsigned short & i : context.stack) {
        i = 0;
    }

    return true;
}

void CHIP8Manager::handleArithmetic(uint8_t VxAddress, uint8_t Vx, uint8_t Vy) {
    switch (instruction & 0xF) {
        case INST_LOAD:
            context.V[VxAddress] = Vy;
            break;

        case INST_OR:
            context.V[VxAddress] |= Vy;
            context.V[0xF] = 0;
            break;

        case INST_AND:
            context.V[VxAddress] &= Vy;
            context.V[0xF] = 0;
            break;

        case INST_XOR:
            context.V[VxAddress] ^= Vy;
            break;

        case INST_ADD:
            context.V[VxAddress] += Vy;
            context.V[0xF] = Vx + Vy > 255 ? 0x01 : 0x00;
            break;

        case INST_SUB:
            context.V[VxAddress]-= Vy;
            context.V[0xF] = Vy > Vx ? 0 : 1;
            break;

        case INST_SHR:
            context.V[0xF] = context.V[VxAddress] & 0x01;
            context.V[VxAddress] >>= 1;
            break;

        case INST_SUBN:
            context.V[0xF] = Vx > Vy ? 0 : 1;
            context.V[VxAddress] = Vy - Vx;

            break;

        case INST_SHL:
            context.V[0xF] = (context.V[VxAddress] >> 7) & 0x1;
            context.V[VxAddress] <<= 1;

            break;
        default:
            std::cerr << "Error: Invalid Instruction 0x" << std::hex << instruction << std::endl;
            break;
    }
}

void CHIP8Manager::handleKeyEvents(uint8_t Vx) {
    switch (instruction & 0xFF) {
        case SKP:
            if (keypad.wasKeyPressed(Vx)) {
                context.PC += 2;
            }
            break;
        case SKNP:
            if (keypad.wasKeyReleased(Vx)) {
                context.PC += 2;
            }
            break;
        default:
            break;
    }
}

void CHIP8Manager::handleTimeAndMemory(uint8_t VxAddress, uint8_t Vx) {
    switch (instruction & 0xFF) {
        case 0x07:
            context.V[VxAddress] = context.delayTimerRegister;
            break;

        case 0x0A:
            holdUntilClick(VxAddress);
            break;

        case 0x15:
            context.delayTimerRegister = context.V[VxAddress];
            break;

        case 0x18:
            context.soundRegister = context.V[VxAddress];
            break;

        case 0x1E:
            context.I += context.V[VxAddress];
            break;

        case 0x29:
            context.I = FONT_START_ADDRESS + (FONT_BYTES * Vx);
            break;
        case 0x33:
            context.memory[context.I] = Vx / 100;
            context.memory[context.I + 1] = (Vx / 10) % 10;
            context.memory[context.I + 2] = Vx % 10;
            break;
        case 0x55:
            for (uint8_t i = 0; i <= VxAddress; i++) {
                context.memory[context.I + i] = context.V[i];
            }
            context.I += VxAddress + 1;
            break;
        case 0x65:
            for (uint8_t i = 0; i <= VxAddress; i++) {
                context.V[i] = context.memory[context.I + i];
            }
            context.I += VxAddress + 1;
            break;

        default:
            std::cout << "Unknown operation: 0x" << std::hex << instruction << std::endl;
            break;
    }
}

void CHIP8Manager::handleInstruction(uint16_t forcedInstruction) {

    if (forcedInstruction != 0x0000) {
        instruction = forcedInstruction;
    } else {
        instruction = context.memory[context.PC] << 8 | context.memory[context.PC + 1];
    }

    uint8_t VxAddress = (instruction & 0x0F00) >> 8;
    uint8_t Vx = context.V[VxAddress];
    uint8_t VyAddress = (instruction & 0x00F0) >> 4;
    uint8_t Vy = context.V[VyAddress];

    context.PC += 2;

    switch ((instruction & 0xF000) >> 12) {
        case 0x0:
            if (CLEAR_DISPLAY == instruction) {
                gui.clearDisplay();
            } else if (instruction == RET) {
                context.PC = context.stack[--context.sp];
            } else {
                std::cerr << "Error: Invalid Instruction" << std::endl;
            }
            break;

        case JP_ADDR:
            context.PC = instruction & 0x0FFF;
            break;

        case CALL_ADDR:
            context.stack[context.sp++] = context.PC;
            context.PC = instruction & 0x0FFF;
            break;

        case SEVxByte:
            if (Vx == (instruction & 0xFF)) context.PC += 2;
            break;

        case SNE:
            if (Vx != (instruction & 0xFF)) context.PC += 2;
            break;

        case SEVxVy:
            if (Vx == Vy) context.PC += 2;
            break;
        case LOAD:
            context.V[VxAddress] = instruction & 0xFF;
            break;
        case ADDVxByte:
            context.V[VxAddress] += instruction & 0xFF;
            break;
        case 0x8:
            handleArithmetic(VxAddress, Vx, Vy);
            break;
        case 0x9:
            if (Vx != Vy) context.PC += 2;
            break;

        case 0xA:
            context.I = instruction & 0x0FFF;
            break;

        case 0xB:
            context.PC = context.V[0] + instruction & 0x0FFF;
            break;

        case RND:
            context.V[VxAddress] = byteRand() & (instruction & 0xFF);
            break;

        case DISP:
            drawOnFrame(Vx, Vy, instruction & 0x000F);
            break;
        case 0xE:
            handleKeyEvents(Vx);
            break;
        case 0xF:
            handleTimeAndMemory(VxAddress, Vx);
            break;
        default:
            std::cout << "UNKNOWN INSTRUCTION: " <<std::hex << instruction << std::endl;
            break;
    }
}


void CHIP8Manager::handleSpecialRegisters() {
    context.delayTimerRegister > 0 ? --context.delayTimerRegister : NULL;
    context.soundRegister > 0 ? audio.playSound(&context.soundRegister, SND_FILENAME | SND_LOOP | SND_ASYNC) : audio.stopSound();
}

void CHIP8Manager::holdUntilClick(uint8_t regAddress) {
    keypad.wasAnyKeyPressed(&systemStatus, context.V, regAddress);

    if (systemStatus == HALT) {
        context.PC -= 2;
    }
}

void CHIP8Manager::handleEvents(bool *exit) {
    keypad.processInput(exit, &systemStatus, &gui);
}

void CHIP8Manager::drawOnFrame(uint8_t Vx, uint8_t Vy, uint8_t n) {
    gui.drawSprite(context.memory, context.V, context.I, Vx, Vy, n);
}

void CHIP8Manager::renderFrameToScreen() {
    gui.renderFrameToScreen();
}

void CHIP8Manager::loadFontToMemory() {
    uint8_t address = FONT_START_ADDRESS;
    for (auto hex: font) {
        for (uint8_t i = 0; i < FONT_BYTES; i++) {
            context.memory[address + i] = hex[i];
        }
        address += FONT_BYTES;
    }
}

uint8_t CHIP8Manager::byteRand() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint16_t> dist(0, 255);
    return static_cast<uint8_t>(dist(gen));
}