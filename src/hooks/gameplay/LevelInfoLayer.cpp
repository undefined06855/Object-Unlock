#include "LevelInfoLayer.hpp"
#include "../../UnlockManager.hpp"

void HookedLevelInfoLayer::confirmClone(cocos2d::CCObject* sender) {
    if (!UnlockManager::get().isInRun()) {
        LevelInfoLayer::confirmClone(sender);
        return;
    }

    geode::Notification::create("You cannot access your levels during a run!", geode::NotificationIcon::Info)->show();
}
