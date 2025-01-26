//
// Created by Xandr on 1/26/2025.
//

#include <Windows.h>
#include <sys/stat.h>
#include "AudioManager.h"

#include <cstdint>

void AudioManager::playSound(uint8_t* timer, long options) const {
    PlaySound(TEXT(soundfileName), NULL, options);
    (*timer)--;
}

void AudioManager::stopSound() {
    PlaySound(NULL, NULL, NULL);
}