#include "GameManager.hpp"
#include "../../ResultsLayer.hpp"

void HookedGameManager::returnToLastScene(GJGameLevel* level) {
    if (m_sceneEnum == -8008135) {
        // from us if the game ends during a run
        // setup scene stack so it goes menulayer, creatorlayer, your levels, resultslayer
        // so pressing back on resultslayer goes to your levels

        auto director = cocos2d::CCDirector::get();

        // retain and keep this current scene
        director->m_pRunningScene->retain();

        for (auto scene : geode::cocos::CCArrayExt<cocos2d::CCScene*>(director->m_pobScenesStack)) {
            if (scene->isRunning()) continue;
            scene->cleanup();
        }

        director->m_pobScenesStack->removeAllObjects();

        // and add our scenes
        director->m_pobScenesStack->addObject(MenuLayer::scene(false));
        director->m_pobScenesStack->addObject(CreatorLayer::scene());
        director->m_pobScenesStack->addObject(LevelBrowserLayer::scene(GJSearchObject::create(SearchType::MyLevels)));
        director->m_pobScenesStack->addObject(cocos2d::CCTransitionFade::create(.5f, ResultsLayer::scene()));

        director->m_pNextScene = static_cast<cocos2d::CCScene*>(director->m_pobScenesStack->lastObject());
        
        return;
    }

    GameManager::returnToLastScene(level);
}
