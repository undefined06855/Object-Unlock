#include "EndLevelLayer.hpp"
#include "PlayLayer.hpp"
#include "GameObject.hpp"
#include <alphalaneous.object_popup_api/include/ObjectNames.hpp>

void HookedEndLevelLayer::customSetup() {
    EndLevelLayer::customSetup();

    auto playLayer = static_cast<HookedPlayLayer*>(m_playLayer);
    auto playLayerFields = playLayer->m_fields.self();

    // top clip content
    {
        auto clipContent = createClip(4.f);
        clipContent->getParent()->setPositionY(187.f);
        clipContent->setScale(1.5f);
    
        for (auto objectID : playLayerFields->m_objectIDsSeen) {
            auto object = HookedGameObject::createGameObject(objectID);
            if (!object) continue;
    
            if (object->getContentHeight() > 60.f) {
                object->setScale(60.f / object->getContentHeight());
            }
    
            clipContent->addChild(object);
        }
    
        clipContent->updateLayout();
    
        runActionOnClipContent(clipContent, ClipContentDirection::Left, 255);
    }

    // bottom clip content
    {
        auto clipContent = createClip(-4.f);
        clipContent->getParent()->setPositionY(107.f);
        clipContent->setScale(.2f);
    
        for (auto objectID : playLayerFields->m_objectIDsSeen) {
            auto label = cocos2d::CCLabelBMFont::create(ObjectNames::get()->nameForID(objectID).c_str(), "bigFont.fnt");
            label->setScale(60.f / label->getContentHeight());
            clipContent->addChild(label);
        }

        if (playLayerFields->m_objectIDsSeen.empty()) {
            auto label = cocos2d::CCLabelBMFont::create("no new objects unlocked...", "bigFont.fnt");
            label->setScale(60.f / label->getContentHeight());
            clipContent->addChild(label);
        }

        clipContent->updateLayout();
    
        runActionOnClipContent(clipContent, ClipContentDirection::Right, 52);
    }
}

cocos2d::CCNodeRGBA* HookedEndLevelLayer::createClip(float rotation) {
    auto width = m_listLayer->getContentWidth();
    auto height = 60.f;

    auto stencil = cocos2d::CCLayerColor::create();
    stencil->setSkewX(-rotation);
    stencil->setContentSize({ width, height });

    auto clipNode = cocos2d::CCClippingNode::create(stencil);
    clipNode->setAnchorPoint({ .5f, .5f });
    clipNode->setContentSize({ width, height });
    clipNode->setRotation(rotation);
    clipNode->setPositionX(width / 2.f);
    m_listLayer->addChild(clipNode); // adding to listLayer ensures its behind the bars

    auto clipContent = cocos2d::CCNodeRGBA::create();
    clipContent->setCascadeOpacityEnabled(true);
    clipContent->setLayout(
        geode::RowLayout::create()
            ->setAutoGrowAxis(0.f)
            ->setGap(15.f)
            ->setAutoScale(false)
            ->setAxisAlignment(geode::AxisAlignment::End)
            ->setAxisReverse(true)
            ->setCrossAxisLineAlignment(geode::AxisAlignment::Start)
    );
    clipNode->addChild(clipContent);

    auto padPre = cocos2d::CCNode::create();
    padPre->setContentSize({ 15.f, 60.f });
    padPre->setZOrder(-100);
    clipContent->addChild(padPre);

    auto padPost = cocos2d::CCNode::create();
    padPost->setContentSize({ 15.f, 60.f });
    padPost->setZOrder(100);
    clipContent->addChild(padPost);

    return clipContent;
}

void HookedEndLevelLayer::runActionOnClipContent(cocos2d::CCNodeRGBA* clipContent, ClipContentDirection direction, GLubyte endOpacity) {
    auto contentWidth = clipContent->getScaledContentWidth();
    auto parentWidth = clipContent->getParent()->getScaledContentWidth();
    auto movementAmountNeeded = std::max(0.f, contentWidth - parentWidth);
    auto speed = 50.f; // cocos units per second?
    auto movementLength = movementAmountNeeded / speed; // time = distance ÷ speed

    if (direction == ClipContentDirection::Right) movementAmountNeeded *= -1.f;

    // if its wide enough, move left and right
    cocos2d::CCActionInterval* shuffleAction = cocos2d::CCDelayTime::create(0.f);
    if (movementAmountNeeded != 0.f) {
        shuffleAction = cocos2d::CCRepeat::create(
            cocos2d::CCSequence::create(
                cocos2d::CCMoveBy::create(movementLength, { -movementAmountNeeded, 0.f }),
                cocos2d::CCDelayTime::create(2.f),
                cocos2d::CCMoveBy::create(movementLength, { movementAmountNeeded, 0.f }),
                cocos2d::CCDelayTime::create(2.f),
                nullptr
            ), 999999 // i dont trust ccrepeatforever
        );
    }

    auto initialAnimMovement = 100.f;
    
    if (movementAmountNeeded != 0.f) {
        if (direction == ClipContentDirection::Right) {
            // i dont remember why this worked to be honest do the maths in your head
            clipContent->setPositionX(-clipContent->getScaledContentWidth() - initialAnimMovement + parentWidth);
            initialAnimMovement *= -1.f;
        } else {
            clipContent->setPositionX(initialAnimMovement);
        }
    } else {
        // just put in middle if we dont have any movement amount needed
        clipContent->setPositionX(parentWidth / 2.f);
    }

    clipContent->setOpacity(0);

    clipContent->runAction(cocos2d::CCSequence::createWithTwoActions(
        cocos2d::CCDelayTime::create(.5f),
        cocos2d::CCSpawn::createWithTwoActions(
            cocos2d::CCSpawn::createWithTwoActions(
                cocos2d::CCFadeTo::create(.5f, endOpacity),
                cocos2d::CCEaseExponentialOut::create(cocos2d::CCMoveBy::create(1.f, { -initialAnimMovement, 0.f }))
            ),
            // start this slightly earlier for a smoother transition
            cocos2d::CCSpawn::createWithTwoActions(
                cocos2d::CCDelayTime::create(.97f),
                shuffleAction
            )
        )
    ));
}
