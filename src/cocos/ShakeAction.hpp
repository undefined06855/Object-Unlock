#pragma once

class ShakeAction : public cocos2d::CCActionInterval {
public:
    static ShakeAction* create(float duration, float scale);
    bool init(float duration, float scale);

    float m_scale;
    cocos2d::CCPoint m_origPos;

    void update(float time);
};
