#pragma once
#include <Geode/modify/CCKeypadDispatcher.hpp>

class $modify(HookedCCKeypadDispatcher, cocos2d::CCKeypadDispatcher) {
    bool dispatchKeypadMSG(cocos2d::ccKeypadMSGType nMsgType);
};
