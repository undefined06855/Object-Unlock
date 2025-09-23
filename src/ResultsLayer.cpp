#include "ResultsLayer.hpp"
#include "UnlockManager.hpp"
#include "hooks/util/GameObject.hpp"
#include <alphalaneous.object_popup_api/include/ObjectNames.hpp>

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

    // horizontal scroll seems interesting and unique
    auto scroll = geode::ScrollLayer::create(content->getContentSize(), true, false);
    content->addChildAtPosition(scroll, geode::Anchor::Center);

    // layout for m_contentLayer is set below

    static constexpr int nodeHeight1u = 60.f;
    static constexpr int nodeWidth1u = 60.f;

    float totalWidth = 0.f;
    auto& unlockedObjectIDs = UnlockManager::get().m_runState.m_unlockedObjectIDs;
    for (auto objectID : unlockedObjectIDs) {
        auto node = cocos2d::CCNode::create();

        auto object = HookedGameObject::createGameObject(objectID);
        
        // keep object at max 1u by 1u size
        if (object->getContentWidth() > object->getContentHeight()) {
            // scale by width
            object->setScale(nodeWidth1u / object->getContentWidth());
        } else {
            // scale by height
            object->setScale(nodeHeight1u / object->getContentHeight());
        }

        node->addChildAtPosition(object, geode::Anchor::Left, { 30.f, 0.f });

        auto name = ObjectNames::get()->nameForID(objectID);
        auto label = cocos2d::CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
        label->setAnchorPoint({ 0.f, .5f });
        label->setScale(.6f);
        
        node->addChildAtPosition(label, geode::Anchor::Left, { 65.f, 0.f });

        // keep width in increments of 1u so it tiles nicely
        auto unframedNodeWidth = nodeWidth1u + 5.f + label->getScaledContentWidth();
        auto framedNodeWidth = std::ceil(unframedNodeWidth / nodeWidth1u) * nodeWidth1u;

        node->setContentWidth(framedNodeWidth);
        node->setContentHeight(nodeHeight1u);

        totalWidth += framedNodeWidth;

        scroll->m_contentLayer->addChild(node);
    }

    // set layout options depending on total width and count so there's always 3
    // or 5 rows

    if (unlockedObjectIDs.size() < 5) {
        // wow you did really bad
        // just use column layout lol
        // this is disappointing
        scroll->m_contentLayer->setLayout(
            geode::ColumnLayout::create()
                ->setGap(0.f)
                ->setAutoScale(false)
                ->setAutoGrowAxis(0.f)
        );
    } else {
        // calculate needed width of row layout in order to have five rows
        // this will most likely give five rows but there is a chance it won't
        // is there a better way to do this?
        auto widthInUnits = totalWidth / nodeWidth1u;
        auto divvied = std::floor(widthInUnits / 5.f);
        auto widthPerRow = divvied * nodeWidth1u;

        scroll->m_contentLayer->setLayout(
            geode::RowLayout::create()
                ->setGap(5.f)
                ->setAutoScale(false)
                ->setAutoGrowAxis(false)
                ->setCrossAxisOverflow(true)
        );
    }

    scroll->m_contentLayer->updateLayout();

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
