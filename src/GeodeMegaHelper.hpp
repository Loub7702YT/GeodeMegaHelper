#include "GeodeMegaHelper.hpp"
#include <Geode/utils/web.hpp>
#include <sstream>
#include <iomanip>

using namespace geode::prelude;

$execute {
    // Register settings
    Mod::get()->addHook<&PlayLayer::init>(Mod::Priority::High);
    Mod::get()->addHook<&PlayLayer::update>(Mod::Priority::High);
    Mod::get()->addHook<&PauseLayer::init>(Mod::Priority::High);
    Mod::get()->addHook<&PlayLayer::resetLevel>(Mod::Priority::High);
    Mod::get()->addHook<&GJBaseGameLayer::destroyPlayer>(Mod::Priority::High);
    Mod::get()->addHook<&PlayLayer::createCheckpoint>(Mod::Priority::High);
}

bool GeodeMegaHelper::setup() {
    // Log mod initialization
    log::info("Setting up GeodeMegaHelper mod");
    
    // Setup main UI hooks
    return true;
}

class GeodeMegaHelperHooks {
private:
    inline static CCLabelBMFont* s_percentageLabel = nullptr;
};

class $modify(MyPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level) {
        if (!PlayLayer::init(level)) return false;
        
        // Setup accurate percentage label if enabled
        if (Mod::get()->getSettingValue<bool>("accurate_percentage")) {
            GeodeMegaHelperHooks::s_percentageLabel = CCLabelBMFont::create("0.000%", "bigFont.fnt");
            GeodeMegaHelperHooks::s_percentageLabel->setScale(0.4f);
            GeodeMegaHelperHooks::s_percentageLabel->setPosition(ccp(30, 30));
            GeodeMegaHelperHooks::s_percentageLabel->setAnchorPoint(ccp(0, 0));
            GeodeMegaHelperHooks::s_percentageLabel->setZOrder(999);
            this->addChild(GeodeMegaHelperHooks::s_percentageLabel);
        }
        
        return true;
    }
    
    void update(float dt) {
        // Apply noclip if enabled
        if (Mod::get()->getSettingValue<bool>("noclip")) {
            m_player1->m_isDead = false;
            if (m_player2) {
                m_player2->m_isDead = false;
            }
        }
        
        // Update accurate percentage
        if (GeodeMegaHelperHooks::s_percentageLabel && Mod::get()->getSettingValue<bool>("accurate_percentage")) {
            float percentage = (m_player1->getPositionX() / m_levelLength) * 100.0f;
            std::stringstream stream;
            stream << std::fixed << std::setprecision(3) << percentage << "%";
            GeodeMegaHelperHooks::s_percentageLabel->setString(stream.str().c_str());
        }
        
        // Apply instant complete
        if (Mod::get()->getSettingValue<bool>("instant_complete") && 
            m_player1->getPositionX() > m_levelLength - 10) {
            this->levelComplete();
        }
        
        // Set game speed
        float playerSpeed = Mod::get()->getSettingValue<float>("player_speed");
        if (playerSpeed != 1.0f) {
            CCDirector::sharedDirector()->getScheduler()->setTimeScale(playerSpeed);
        }
        
        // Call original update method
        PlayLayer::update(dt);
    }
    
    void resetLevel() {
        PlayLayer::resetLevel();
        
        // Apply practice music fix
        if (Mod::get()->getSettingValue<bool>("practice_music") && m_isPracticeMode) {
            GameSoundManager::sharedManager()->playBackgroundMusic(m_level->getAudioFileName(), true, true);
        }
    }
    
    void createCheckpoint() {
        PlayLayer::createCheckpoint();
        
        // Apply auto safe mode
        if (Mod::get()->getSettingValue<bool>("auto_safe_mode")) {
            m_antiCheatPassed = true;
        }
    }
};

class $modify(MyGJBaseGameLayer, GJBaseGameLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* gameObject) {
        // Apply disable death effects
        if (Mod::get()->getSettingValue<bool>("disable_death_effects")) {
            player->m_isDead = true;
            player->setVisible(false);
            return;
        }
        GJBaseGameLayer::destroyPlayer(player, gameObject);
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    bool init(bool practice) {
        if (!PauseLayer::init(practice)) return false;
        
        // Add button to access mod menu
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto menu = CCMenu::create();
        menu->setPosition(ccp(winSize.width - 40, 40));
        
        auto button = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png"),
            this,
            menu_selector(MyPauseLayer::onGeodeMegaHelperButton)
        );
        button->setScale(0.8f);
        menu->addChild(button);
        
        this->addChild(menu);
        
        // Apply show hitboxes if enabled
        if (Mod::get()->getSettingValue<bool>("hitboxes")) {
            if (auto playLayer = PlayLayer::get()) {
                playLayer->setShowHitboxes(true);
            }
        }
        
        // Apply disable particles if enabled
        if (Mod::get()->getSettingValue<bool>("disable_particles")) {
            if (auto playLayer = PlayLayer::get()) {
                if (playLayer->m_particleSystem) {
                    playLayer->m_particleSystem->setVisible(false);
                }
            }
        }
        
        return true;
    }
    
    void onGeodeMegaHelperButton(CCObject* sender) {
        FLAlertLayer::create(
            "GeodeMegaHelper",
            "Access all features through Geode's mod settings!\n\nCurrent status:\n"
            "- Noclip: " + std::string(Mod::get()->getSettingValue<bool>("noclip") ? "ON" : "OFF") + "\n"
            "- Hitboxes: " + std::string(Mod::get()->getSettingValue<bool>("hitboxes") ? "ON" : "OFF") + "\n"
            "- Accurate %: " + std::string(Mod::get()->getSettingValue<bool>("accurate_percentage") ? "ON" : "OFF") + "\n"
            "- Practice Music: " + std::string(Mod::get()->getSettingValue<bool>("practice_music") ? "ON" : "OFF") + "\n"
            "- Speed: " + std::to_string(Mod::get()->getSettingValue<float>("player_speed")) + "x",
            "OK"
        )->show();
    }
};
