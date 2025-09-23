#include "AnimatingLock.hpp"

AnimatingLock* AnimatingLock::create() {
    auto ret = new AnimatingLock;
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool AnimatingLock::init() {
    if (!CCSprite::initWithSpriteFrameName("GJ_lockGray_001.png")) return false;

    setOpacity(0);
    setScale(10.f);
    setRotation(-15.f);

    return true;
}

void AnimatingLock::onEnterTransitionDidFinish() {
    CCSprite::onEnterTransitionDidFinish();

    auto parent = static_cast<CreateMenuItem*>(getParent());
    if (parent->m_pageIndex != 0 || parent->m_tabIndex != 0) {
        setOpacity(255);
        setScale(1.f);
        setRotation(0.f);
        return;
    }

    auto worldPos = getParent()->convertToWorldSpace(getPosition());
    auto delay = worldPos.x / 200.f - worldPos.y / 100.f;

    auto endingRotation = (std::rand() / (float)RAND_MAX) * 10.f - 5.f;

    runAction(cocos2d::CCSequence::createWithTwoActions(
        cocos2d::CCDelayTime::create(delay),
        cocos2d::CCSpawn::create(
            cocos2d::CCFadeIn::create(.3f),
            cocos2d::CCEaseSineIn::create(cocos2d::CCRotateTo::create(.5f, endingRotation)),
            cocos2d::CCEaseSineIn::create(cocos2d::CCScaleTo::create(.5f, 1.f)),
            nullptr
        )
    ));
}
