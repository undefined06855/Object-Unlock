#pragma once

class ObjectCollectLayer : public cocos2d::CCLayer {
public:
    static ObjectCollectLayer* create();
    bool init();

    cocos2d::CCLayerColor* m_bg;
    cocos2d::CCNodeRGBA* m_clipContent;

    bool addObject(int objectID);
    void animateAway();
};
