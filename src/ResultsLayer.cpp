#include "ResultsLayer.hpp"

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

    auto content = cocos2d::CCNode::create();
    content->setContentWidth(getContentWidth() - 50.f);
    content->setContentHeight(getContentHeight());
    addChild(content);

    

    return true;
}
