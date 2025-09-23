#include "ResultsLayer.hpp"
#include "UnlockManager.hpp"

cocos2d::CCScene* ResultsLayer::scene() {
    auto ret = cocos2d::CCScene::create();
    ret->addChild(ResultsLayer::create());
    return ret;
}

ResultsLayer* ResultsLayer::create() {
    auto ret = new ResultsLayer;
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool ResultsLayer::init() {
    if (!CCLayer::init()) return false;
    setID("ResultsLayer"_spr);

    addChild(geode::createLayerBG());
    geode::addSideArt(this, geode::SideArt::All);

    auto winSize = cocos2d::CCDirector::get()->getWinSize();

    auto content = cocos2d::CCNode::create();
    content->setContentWidth(getContentWidth() - 50.f);
    content->setContentHeight(getContentHeight());
    content->setPositionX(25.f);
    addChild(content);

    auto buttonMenu = cocos2d::CCMenu::create();
    content->addChildAtPosition(buttonMenu, geode::Anchor::Center);

    auto continueButton = geode::cocos::CCMenuItemExt::createSpriteExtra(
        ButtonSprite::create("Make a Level!"),
        [](auto) {
            cocos2d::CCDirector::get()->popSceneWithTransition(.5f, cocos2d::kPopTransitionFade);
        }
    );
    buttonMenu->addChildAtPosition(continueButton, geode::Anchor::Bottom, { 0.f, 22.f });

    setKeypadEnabled(true);

    return true;
}

void ResultsLayer::keyBackClicked() {
    cocos2d::CCDirector::get()->popSceneWithTransition(.5f, cocos2d::kPopTransitionFade);
}

void ResultsLayer::onEnterTransitionDidFinish() {
    CCLayer::onEnterTransitionDidFinish();
    UnlockManager::get().m_blockTouches = false;
}
