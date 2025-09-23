#pragma once
#include <RenderTexture.hpp>

/*
 * Gradient Clipping Node - a ccclippingnode that allows half transparent clip
 * regions by rendering both the clip and the contents to two textures, then
 * blending the contents by the clip and rendering that.
 *
 * Relies on undefined06855/gd-render-texture
 */

extern const char* g_clipShaderVertex;
extern const char* g_clipShaderFragment;
extern cocos2d::CCGLProgram* g_clipShader;

class GradientClippingNode : public cocos2d::CCLayerColor {
    GradientClippingNode();
    
    std::shared_ptr<RenderTexture::Sprite> m_clip;
    std::shared_ptr<RenderTexture::Sprite> m_contents;
    bool m_drawingForPreview;

public:
    static GradientClippingNode* create(cocos2d::CCNode* clip);
    bool init(cocos2d::CCNode* clip);

    geode::Ref<cocos2d::CCNode> m_clipNode;

    void rerender();

    void draw();
    void addChild(cocos2d::CCNode* child, int zOrder, int tag);
};
