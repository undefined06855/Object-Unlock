#pragma once
#include <Geode/modify/CCMouseDispatcher.hpp>

class $modify(HookedCCMouseDispatcher, cocos2d::CCMouseDispatcher) {
    bool dispatchScrollMSG(float x, float y);
};
