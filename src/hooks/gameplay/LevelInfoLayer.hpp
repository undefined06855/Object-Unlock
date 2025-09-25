#pragma once
#include <Geode/modify/LevelInfoLayer.hpp>

class $modify(HookedLevelInfoLayer, LevelInfoLayer) {
    void confirmClone(cocos2d::CCObject* sender);
};
