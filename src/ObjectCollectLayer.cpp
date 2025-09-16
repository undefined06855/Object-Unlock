#include "ObjectCollectLayer.hpp"
#include "hooks/GameObject.hpp"

ObjectCollectLayer* ObjectCollectLayer::create(bool skipDelay) {
    auto ret = new ObjectCollectLayer;
    if (ret->init(skipDelay)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

static constexpr auto bgHeight = 23.f;
static constexpr auto clipPad = 3.f;
static constexpr auto clipHeight = bgHeight - clipPad*2.f;

bool ObjectCollectLayer::init(bool skipDelay) {
    if (!CCLayer::init()) return false;
    setID("ObjectCollectLayer"_spr);
    setAnchorPoint({ 0.f, 0.f });

    auto director = cocos2d::CCDirector::get();
    auto bgWidth = director->getScreenRight() - 40.f;
    auto clipWidth = bgWidth - clipPad*2.f;
    auto bgAnchor = geode::Anchor::Bottom;
    auto bgOffset = cocos2d::CCPoint{ 0.f, 18.f };

    if (auto mod = geode::Loader::get()->getLoadedMod("fra4.custom-progress-bar")) {
        if (!mod->getSettingValue<bool>("vertical-progress-bar")) {
            if (mod->getSettingValue<std::string>("Height") == "Bottom") {
                bgAnchor = geode::Anchor::Top;
                bgOffset.y *= -1;
            }
        }
    }

    m_bg = cocos2d::CCLayerColor::create();
    m_bg->ignoreAnchorPointForPosition(false); // FUCK YOU!!
    m_bg->setColor({ 0, 0, 0 });
    m_bg->setOpacity(0);
    m_bg->setSkewX(8.f);
    m_bg->setContentSize({ bgWidth, bgHeight });
    addChildAtPosition(m_bg, bgAnchor, bgOffset);
    
    m_bg->setScaleX(0.f);
    m_bg->setPositionY(m_bg->getPositionY() + 100.f);
    m_bg->runAction(cocos2d::CCSequence::createWithTwoActions(
        cocos2d::CCDelayTime::create(skipDelay ? .2f : 1.f),
        cocos2d::CCSpawn::create(
            cocos2d::CCEaseExponentialOut::create(cocos2d::CCScaleTo::create(.5f, 1.f)),
            cocos2d::CCEaseExponentialOut::create(cocos2d::CCMoveBy::create(.5f, { 0.f, -100.f })),
            cocos2d::CCFadeTo::create(.5f, 72),
            nullptr
        )
    ));

    auto stencil = cocos2d::CCLayerColor::create();
    stencil->setSkewX(8.f);
    stencil->setContentSize({ clipWidth, clipHeight });

    auto clipNode = cocos2d::CCClippingNode::create(stencil);
    clipNode->setAnchorPoint({ .5f, .5f });
    clipNode->setContentSize({ clipWidth, clipHeight });
    addChildAtPosition(clipNode, bgAnchor, bgOffset);

    m_clipContent = cocos2d::CCNodeRGBA::create();
    m_clipContent->setCascadeOpacityEnabled(true);
    m_clipContent->setLayout(
        geode::RowLayout::create()
            ->setAutoGrowAxis(clipWidth)
            ->setGap(0.f)
            ->setAutoScale(false)
            ->setAxisAlignment(geode::AxisAlignment::End)
            ->setAxisReverse(true)
            ->setCrossAxisLineAlignment(geode::AxisAlignment::Start)
    );
    clipNode->addChild(m_clipContent);

    auto padRight = cocos2d::CCNode::create();
    padRight->setContentSize({ 9.f, clipHeight });
    m_clipContent->addChild(padRight);

    return true;
}

bool ObjectCollectLayer::addObject(int objectID) {
    auto object = HookedGameObject::createGameObject(objectID);
    if (!object) return false;

    // wrap in wrapper node so we can add animations without affecting layout
    auto wrapper = cocos2d::CCNodeRGBA::create();
    wrapper->setCascadeOpacityEnabled(true);
    wrapper->setContentSize(object->getScaledContentSize());
    wrapper->addChild(object);
    if (wrapper->getContentHeight() > clipHeight) {
        wrapper->setScale(clipHeight / wrapper->getContentHeight());
    }
    m_clipContent->addChild(wrapper);

    // pad so that it isnt squished against the left of the container
    // this doubles the amount of nodes to calculate but whatever
    auto padLeft = cocos2d::CCNode::create();
    padLeft->setContentSize({ 9.f, clipHeight });
    m_clipContent->addChild(padLeft);

    m_clipContent->updateLayout();

    // oooh fancy animations
    // these dont affect the layout because the object is in a wrapper node
    object->setOpacity(0);
    object->setScaleY(0.f);
    object->setPositionX(object->getPositionX() - 100.f);
    object->runAction(cocos2d::CCFadeIn::create(.5f));
    object->runAction(cocos2d::CCEaseExponentialOut::create(cocos2d::CCScaleTo::create(.5f, 1.f)));
    object->runAction(cocos2d::CCEaseExponentialOut::create(cocos2d::CCMoveBy::create(.5f, { 100.f, 0.f })));

    return true;
}

void ObjectCollectLayer::animateAway() {
    // ensure scaling happens from middle center
    setAnchorPoint({ .5f, 0.f });
    setPositionX(getContentWidth() / 2.f);

    auto sequence = cocos2d::CCSequence::create(
        cocos2d::CCDelayTime::create(1.f),
        cocos2d::CCEaseExponentialIn::create(cocos2d::CCScaleTo::create(1.f, 0.f)),
        cocos2d::CCRemoveSelf::create(),
        nullptr
    );

    auto fadeSequence = cocos2d::CCSequence::createWithTwoActions(
        cocos2d::CCDelayTime::create(1.f),
        cocos2d::CCFadeTo::create(1.f, 0)
    );

    m_bg->runAction(static_cast<cocos2d::CCAction*>(fadeSequence->copy()));
    m_clipContent->runAction(static_cast<cocos2d::CCAction*>(fadeSequence->copy()));
    this->runAction(sequence);
}
