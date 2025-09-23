#pragma once
#include <Geode/modify/EndLevelLayer.hpp>

enum class ClipContentDirection { Left, Right };

class $modify(HookedEndLevelLayer, EndLevelLayer) {
    void customSetup();

    cocos2d::CCNodeRGBA* createClip(float rotation);
    void runActionOnClipContent(cocos2d::CCNodeRGBA* clipContent, ClipContentDirection direction, GLubyte endOpacity);
};
