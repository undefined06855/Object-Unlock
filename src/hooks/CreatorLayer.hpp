#pragma once
#include <Geode/modify/CreatorLayer.hpp>

class $modify(HookedCreatorLayer, CreatorLayer) {
    void onMyLevels(cocos2d::CCObject* sender);
};
