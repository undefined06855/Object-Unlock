#include "LevelInfoLayer.hpp"
#include "../UnlockManager.hpp"

void HookedLevelInfoLayer::tryCloneLevel(cocos2d::CCObject* sender) {
    if (!UnlockManager::get().isInRun()) {
        LevelInfoLayer::tryCloneLevel(sender);
        return;
    }

    geode::Notification::create("You cannot access your levels during a run!", geode::NotificationIcon::Info);
}
