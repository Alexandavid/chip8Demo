//
// Created by Xandr on 1/26/2025.
//
#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <stdint.h>
#include <mem.h>

class AudioManager {
    char soundfileName[50];

 public:
    AudioManager(const char* filename) {
        memcpy(soundfileName, filename, sizeof(soundfileName));
    };

    void playSound(uint8_t *timer, long options) ;

    void stopSound();
};


#endif //AUDIOMANAGER_H
