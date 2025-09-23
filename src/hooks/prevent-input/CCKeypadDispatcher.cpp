#include "CCKeypadDispatcher.hpp"
#include "../../UnlockManager.hpp"

bool HookedCCKeypadDispatcher::dispatchKeypadMSG(cocos2d::ccKeypadMSGType nMsgType) {
    if (m_pDelegates->count() <= 0 || nMsgType  != cocos2d::kTypeBackClicked) {
        return CCKeypadDispatcher::dispatchKeypadMSG(nMsgType);
    }

    if (UnlockManager::get().m_blockTouches) return true;

    return CCKeypadDispatcher::dispatchKeypadMSG(nMsgType);
}
