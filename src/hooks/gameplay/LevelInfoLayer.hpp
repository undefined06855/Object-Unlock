#pragma once
#include <Geode/modify/LevelInfoLayer.hpp>

class $modify(HookedLevelInfoLayer, LevelInfoLayer) {
    void tryCloneLevel(cocos2d::CCObject* sender);
};
