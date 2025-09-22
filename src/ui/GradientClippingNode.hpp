#pragma once

/*
 * Gradient Clipping Node - a ccclippingnode that allows half transparent clip
 * regions by rendering both the clip and the contents to two textures, then
 * blending the contents by the clip and rendering that.
 *
 * Relies on undefined06855/gd-render-texture
 */

class GradientClippingNode : public cocos2d::CCNode {
    GradientClippingNode();
public:
    static GradientClippingNode* create(cocos2d::CCNode* clip);
    bool init(cocos2d::CCNode* clip);

    cocos2d::CCNode* m_clipNode;
    cocos2d::CCSprite* m_clip;
    cocos2d::CCSprite* m_contents;

    void rerender();

    void draw();
    void addChild(cocos2d::CCNode* child, int zOrder, int tag);
};
