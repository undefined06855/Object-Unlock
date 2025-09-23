#include "EditorUI.hpp"
#include "../../UnlockManager.hpp"
#include "../../cocos/AnimatingLock.hpp"

CreateMenuItem* HookedEditorUI::getCreateBtn(int id, int bg) {
    auto ret = EditorUI::getCreateBtn(id, bg);
    auto manager = UnlockManager::get();
    if (!manager.isInEditor()) return ret;

    auto& objectIDs = manager.m_runState.m_unlockedObjectIDs;
    if (std::find(objectIDs.begin(), objectIDs.end(), id) != objectIDs.end()) {
        // found, don't add lock, just return
        return ret;
    }

    // not found! add lock
    // lock will animate itself
    auto lock = AnimatingLock::create();
    lock->setID("lock"_spr);
    lock->setPosition(ret->getContentSize() / 2.f);
    ret->setEnabled(false);
    ret->addChild(lock);

    return ret;
}
