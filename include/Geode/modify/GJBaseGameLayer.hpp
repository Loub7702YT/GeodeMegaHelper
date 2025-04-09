#pragma once

#include <Geode/Geode.hpp>

// This file is a placeholder that gets replaced by Geode's own headers
// It allows us to reference the GJBaseGameLayer class for modifications

class PlayerObject;
class GameObject;

class GJBaseGameLayer : public cocos2d::CCLayer {
public:
    // Static accessor - this is how Geode lets you get the current GJBaseGameLayer
    static GJBaseGameLayer* get();

    // Functions we're planning to modify
    virtual void destroyPlayer(PlayerObject* player, GameObject* gameObject);
};
