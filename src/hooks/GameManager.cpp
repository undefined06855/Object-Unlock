#include "GameManager.hpp"
#include "../ResultsLayer.hpp"

void HookedGameManager::returnToLastScene(GJGameLevel* level) {
    if (m_sceneEnum == -8008135) {
        // from us if the game ends during a run
        // setup scene stack so first is menulayer and second is our layer
        auto director = cocos2d::CCDirector::get();
        director->popToRootScene();

        // clean up this current last scene, add our two new scenes
        auto lastScene = static_cast<cocos2d::CCScene*>(director->m_pobScenesStack->lastObject());
        if (lastScene->isRunning()) {
            lastScene->onExitTransitionDidStart();
            lastScene->onExit();
        }
        lastScene->cleanup();

        director->m_pobScenesStack->removeAllObjects();

        director->m_pobScenesStack->addObject(MenuLayer::create());
        director->m_pobScenesStack->addObject(ResultsLayer::create());

        director->m_pNextScene = static_cast<cocos2d::CCScene*>(director->m_pobScenesStack->lastObject());
        
        return;
    }

    GameManager::returnToLastScene(level);
}
