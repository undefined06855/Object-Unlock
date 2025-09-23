#include "ShakeAction.hpp"

ShakeAction* ShakeAction::create(float duration, float scale) {
    auto ret = new ShakeAction;
    if (ret->init(duration, scale)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool ShakeAction::init(float duration, float scale) {
    if (!CCActionInterval::initWithDuration(duration)) return false;

    m_scale = scale;
    m_origPos = cocos2d::CCPoint{ -1.f, -1.f };

    return true;
}

void ShakeAction::update(float time) {
    if (!m_pTarget) return;

    if (m_origPos == cocos2d::CCPoint{ -1.f, -1.f }) {
        m_origPos = m_pTarget->getPosition();
    }

    auto scale = m_scale * (1.f - (time / m_fDuration));

    auto randX = (std::rand() / (float)RAND_MAX) * 2.f - 1.f;
    auto randY = (std::rand() / (float)RAND_MAX) * 2.f - 1.f;
    auto randOffset = cocos2d::CCPoint{ randX, randY } * scale;

    m_pTarget->setPosition(m_origPos + randOffset);
}
