#pragma once
#include <Geode/modify/PlayLayer.hpp>
#include "../ObjectCollectLayer.hpp"

class $modify(HookedPlayLayer, PlayLayer) {
    struct Fields {
        Fields();

        std::vector<int> m_objectIDsSeen;
        ObjectCollectLayer* m_overlayLayer;
        bool m_started;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects);
    static cocos2d::CCScene* scene(GJGameLevel* level, bool useReplay, bool dontCreateObjects);

    void startGame();
    void updateVisibility(float dt);
    
    void playEndAnimationToPos(cocos2d::CCPoint pos);
    void playPlatformerEndAnimationToPos(cocos2d::CCPoint pos, bool instant);

    void fullReset();

    void sharedInit(bool skipDelay = false);
    void potentiallyAddObject(GameObject* object, bool skipTouchChecks = false);
};
