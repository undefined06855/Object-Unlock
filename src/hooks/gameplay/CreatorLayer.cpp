#include "CreatorLayer.hpp"
#include "../../UnlockManager.hpp"

void HookedCreatorLayer::onMyLevels(cocos2d::CCObject* sender) {
    // TODO: CHANGE THIS BACK!! THIS IS FOR DEBUGGING
    // if (!UnlockManager::get().isInRun()) {
        CreatorLayer::onMyLevels(sender);
        return;
    // }

    // TODO: change all notifications to dialoglayers or something? seems a bit boring
    // see levelinfolayer too
    geode::Notification::create("You cannot access your levels during a run!", geode::NotificationIcon::Info)->show();
}
