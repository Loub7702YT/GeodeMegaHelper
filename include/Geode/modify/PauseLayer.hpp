#pragma once

#include <Geode/Geode.hpp>

// This file is a placeholder that gets replaced by Geode's own headers
// It allows us to reference the PauseLayer class for modifications

class PauseLayer : public cocos2d::CCLayer {
public:
    // Static accessor - this is how Geode lets you get the current PauseLayer
    static PauseLayer* get();

    // Functions we're planning to modify
    virtual bool init(bool practice);
};

class FLAlertLayer {
public:
    static FLAlertLayer* create(const char* title, const char* description, const char* btn);
    void show();
};

class GameSoundManager {
public:
    static GameSoundManager* sharedManager();
    void playBackgroundMusic(const char* fileName, bool fade, bool restore);
};
