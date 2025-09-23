#pragma once

class AnimatingLock : public cocos2d::CCSprite {
public:
    static AnimatingLock* create();
    bool init();

    void onEnterTransitionDidFinish();
};
