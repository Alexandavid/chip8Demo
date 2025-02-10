//
// Created by Xandr on 1/26/2025.
//


#include <Windows.h>
#include <sys/stat.h>
#include "AudioManager.h"
#include <QSoundEffect>

#include <cstdint>
#include <string>

AudioManager::AudioManager(const char *filename) {
    memcpy(soundfileName, filename, sizeof(soundfileName));
    player = new QMediaPlayer;
    output = new QAudioOutput;

    player->setAudioOutput(output);
    output->setVolume(0.5);
}

AudioManager::~AudioManager() {
    delete player;
    delete output;
}

void AudioManager::playSound(uint8_t *timer) {
    if (!player) {
        player = new QMediaPlayer;
    }

    QUrl url = QUrl::fromLocalFile(QString::fromStdString(soundfileName));
    player->setSource(url);
    player->play();

    (*timer)--;
}

void AudioManager::stopSound() {
    // PlaySound(NULL, NULL, NULL);
    player->stop();
}
