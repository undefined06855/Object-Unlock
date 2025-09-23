#include "CCMouseDispatcher.hpp"
#include "../../UnlockManager.hpp"

bool HookedCCMouseDispatcher::dispatchScrollMSG(float x, float y) {
    if (UnlockManager::get().m_blockTouches) return true;
    return CCMouseDispatcher::dispatchScrollMSG(x, y);
}
