#pragma once

#include <Geode/Geode.hpp>

// This file is a placeholder that gets replaced by Geode's own headers
// It allows us to reference the EditorLayer class for modifications

class EditorLayer : public cocos2d::CCLayer {
public:
    // Static accessor - this is how Geode lets you get the current EditorLayer
    static EditorLayer* get();

    // Functions we're planning to modify
    virtual bool init(GJGameLevel* level);
};
