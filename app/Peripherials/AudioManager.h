//
// Created by Xandr on 1/26/2025.
//
#include <mem.h>
#include <stdint.h>

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

class AudioManager {
    char soundfileName[50];


 public:
    AudioManager(const char* filename) {
        memcpy(soundfileName, filename, sizeof(soundfileName));
    };

    void playSound(uint8_t *timer, long options) const;

    void stopSound();
};


#endif //AUDIOMANAGER_H
