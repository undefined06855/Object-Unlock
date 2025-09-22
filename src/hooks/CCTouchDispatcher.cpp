#include "CCTouchDispatcher.hpp"
#include "../UnlockManager.hpp"

void HookedCCTouchDispatcher::touches(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent, unsigned int uIndex) {
    if (!UnlockManager::get().m_blockTouches) {
        CCTouchDispatcher::touches(pTouches, pEvent, uIndex);
    }
}
