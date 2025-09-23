#include "UnlockManager.hpp"
#include "cocos/ShakeAction.hpp"

UnlockManager::UnlockManager()
    : m_state(ObjectUnlockState::None)
    , m_runState()
    , m_label(nullptr)
    
    , m_blockTouches(false) {}

UnlockManager& UnlockManager::get() {
    static UnlockManager instance;
    return instance;
}

bool UnlockManager::isInRun() { return m_state == ObjectUnlockState::InRun; }
bool UnlockManager::isInEditor() { return m_state == ObjectUnlockState::InEditor; }

void UnlockManager::startRun(const RunConfig& config) {
    m_state = ObjectUnlockState::InRun;
    
    auto director = cocos2d::CCDirector::get();

    m_runState = RunState{
        .m_unlockedObjectIDs = {},
        .m_endTime = std::chrono::high_resolution_clock::now() + config.m_length
    };
    
    m_label = cocos2d::CCLabelBMFont::create("...", "bigFont.fnt");
    m_label->setID("duration-label"_spr);
    m_label->setOpacity(0);
    m_label->setPositionX(director->getScreenRight() / 2.f);
    m_label->setPositionY(director->getScreenTop());
    m_label->setScale(.7f);

    m_label->runAction(
        cocos2d::CCSpawn::createWithTwoActions(
            cocos2d::CCEaseExponentialOut::create(cocos2d::CCMoveBy::create(.7f, { 0.f, -20.f })),
            cocos2d::CCFadeTo::create(.7f, 170)
        )
    );

    geode::SceneManager::get()->keepAcrossScenes(m_label);

    update(0.f); // update label
}

void UnlockManager::endRun() {
    m_state = ObjectUnlockState::InEditor;

    m_blockTouches = true;

    if (auto pl = PlayLayer::get()) {
        pl->pauseGame(true);
        auto pauseLayer = cocos2d::CCScene::get()->getChildByType<PauseLayer>(0);
        pauseLayer->setVisible(false);
    }

    geode::SceneManager::get()->forget(m_label);
    m_label->removeFromParent();

    // easier to completely recreate the label
    m_label = cocos2d::CCLabelBMFont::create("Game Over!", "bigFont.fnt");
    m_label->setID("game-over-label"_spr);
    m_label->setPosition(cocos2d::CCDirector::get()->getWinSize() / 2.f);
    m_label->setPositionY(m_label->getPositionY() + 13.f);
    m_label->setOpacity(0);
    m_label->setRotation(-20.f);
    m_label->setScale(60.f);
    cocos2d::CCScene::get()->addChild(m_label);

    // animation borrowed from minesweeperplus
    // go play it https://minesweeper.undefined0.dev/
    // (you can make your own custom tiles with javascript!)
    
    // https://minesweeper.undefined0.dev/class/screen/Title.js (ctrl+f "// title")
    // immediate mode ui so need to refactor for cocos rm ui
    m_label->runAction(cocos2d::CCSequence::create(
        cocos2d::CCSpawn::create(
            cocos2d::CCFadeIn::create(.3f),
            // TODO: touch up this easing
            cocos2d::CCEaseSineIn::create(cocos2d::CCRotateTo::create(.5f, 5.f)),
            cocos2d::CCEaseSineIn::create(cocos2d::CCScaleTo::create(.5f, 1.3f)),
            nullptr
        ),
        geode::cocos::CallFuncExt::create([] {
            cocos2d::CCScene::get()->runAction(ShakeAction::create(1.f, 7.f));
        }),
        cocos2d::CCDelayTime::create(3.f),
        geode::cocos::CallFuncExt::create([] {
            auto gameManager = GameManager::get();
            gameManager->m_sceneEnum = -8008135; // check gamemanager hooks

            if (auto pl = PlayLayer::get()) {
                pl->onQuit(); // calls returnToLastScene
            } else {
                gameManager->returnToLastScene(nullptr);
            }
        }),
        nullptr
    ));
}

void UnlockManager::endEntirely() {
    m_state = ObjectUnlockState::None;
}

void UnlockManager::update(float dt) {
    if (!isInRun()) return;

    auto now = std::chrono::high_resolution_clock::now();

    if (now > m_runState.m_endTime) {
        // done
        endRun();
        return;
    }

    // fmt::format has formats for std::chrono but they're really ass
    // not sure how creation rotation casts it to an int to get it to format
    // properly without decimals

    auto timeRemaining = m_runState.m_endTime - now;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(timeRemaining).count();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timeRemaining).count() % 60;
    std::string formatString;

    if (minutes == 0) {
        formatString = fmt::format("{:02}", seconds);
    } else {
        formatString = fmt::format("{}:{:02}", minutes, seconds);
    }

    if (minutes == 1 && seconds == 0 && !m_runState.m_playedMinuteAnimation) {
        m_runState.m_playedMinuteAnimation = true;
        m_label->runAction(
            cocos2d::CCEaseExponentialOut::create(
                cocos2d::CCScaleBy::create(.3f, 1.25f)
            )
        );
    }

    if (minutes == 0 && seconds == 10 && !m_runState.m_playedTenSecondAnimation) {
        m_runState.m_playedTenSecondAnimation = true;
        m_label->runAction(cocos2d::CCSpawn::createWithTwoActions(
            cocos2d::CCSequence::createWithTwoActions(
                // scaling effect
                cocos2d::CCEaseExponentialOut::create(cocos2d::CCScaleBy::create(.375f, 1.5f)),
                cocos2d::CCEaseExponentialIn::create(cocos2d::CCScaleBy::create(.375f, 1.f / 1.5f))
            ),
            cocos2d::CCSequence::create(
                // left/right rotate wobble effect
                cocos2d::CCRotateTo::create(.15f, -20.f),
                cocos2d::CCRotateTo::create(.15f, 20.f),
                cocos2d::CCRotateTo::create(.15f, -20.f),
                cocos2d::CCRotateTo::create(.15f, 20.f),
                cocos2d::CCRotateTo::create(.15f, 0.f),
                nullptr
            )
        ));
    }

    m_label->setString(formatString.c_str());
    
    auto highestZOrder = cocos2d::CCScene::get()->getHighestChildZ();
    if (highestZOrder != INT_MAX) { // mods better not be doing this in 2025
        m_label->setZOrder(highestZOrder + 1);
    }
}

$on_mod(Loaded) {
    cocos2d::CCScheduler::get()->scheduleUpdateForTarget(&UnlockManager::get(), 0, false);
}
