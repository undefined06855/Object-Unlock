#include "MenuLayer.hpp"
#include "../UnlockManager.hpp"

// TODO: remove this

void HookedMenuLayer::onMoreGames(cocos2d::CCObject* sender) {
    UnlockManager::get().startRun({
        .m_length = std::chrono::minutes(2)
    });
}
