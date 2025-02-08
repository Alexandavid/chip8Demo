//
// Created by Xandr on 1/26/2025.
//


#include <Windows.h>
#include <sys/stat.h>
#include "AudioManager.h"

#include <cstdint>
#include <string>

void AudioManager::playSound(uint8_t* timer, long options) {
    std::wstring wideSoundFileName = std::wstring(soundfileName, soundfileName + strlen(soundfileName));

    // Play sound with the wide-character string
    PlaySound(wideSoundFileName.c_str(), NULL, options);
    (*timer)--;
}

void AudioManager::stopSound() {
    PlaySound(NULL, NULL, NULL);
}