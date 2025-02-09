//
// Created by Xandr on 1/26/2025.
//
#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <stdint.h>
#include <QMediaPlayer>
#include <QAudioOutput>

#include <QUrl>

class AudioManager {
    char soundfileName[50];
    QMediaPlayer* player;
    QAudioOutput* output;

 public:
    AudioManager(const char* filename);
    ~AudioManager();

    void playSound(uint8_t *timer) ;

    void stopSound();
};


#endif //AUDIOMANAGER_H
