#pragma once

#include <Geode/Geode.hpp>

// This file is a placeholder that gets replaced by Geode's own headers
// It allows us to reference the PlayLayer class for modifications

class GJGameLevel;
class PlayerObject;
class GameObject;

class PlayLayer : public cocos2d::CCLayer {
protected:
    // These are just placeholders for the real members in the class
    // The real implementation is provided by Geode
    float m_levelLength;
    PlayerObject* m_player1;
    PlayerObject* m_player2;
    bool m_isPracticeMode;
    bool m_antiCheatPassed;
    GJGameLevel* m_level;
    cocos2d::CCParticleSystemQuad* m_particleSystem;

public:
    // Static accessor - this is how Geode lets you get the current PlayLayer
    static PlayLayer* get();

    // Functions we're planning to modify
    virtual bool init(GJGameLevel* level);
    virtual void update(float dt);
    virtual void resetLevel();
    virtual void createCheckpoint();
    virtual void levelComplete();
    virtual void setShowHitboxes(bool show);
};
