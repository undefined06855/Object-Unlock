#include "GradientClippingNode.hpp"
#include <RenderTexture.hpp>

const char* g_clipShaderVertex = R"(

)";

const char* g_clipShaderFragment = R"(

)";

cocos2d::CCGLProgram* g_clipShader = nullptr;

static unsigned int* getNumberOfDraws() {
    // compiler yells at me if i use #elifdef :broken_heart:
    // thanks to jasmine for ALL of these
#if defined(GEODE_IS_MACOS)
    static_assert(GEODE_COMP_GD_VERSION == 22074, "Please update macOS offsets");
    return reinterpret_cast<unsigned int*>(geode::base::get() + GEODE_ARM_MAC(0x8b0f60) GEODE_INTEL_MAC(0x98bf30));
#elif defined(GEODE_IS_IOS)
    static_assert(GEODE_COMP_GD_VERSION == 22074, "Please update iOS offsets");
    return reinterpret_cast<unsigned int*>(geode::base::get() + 0x8791d0);
#else
    return &g_uNumberOfDraws;
#endif
}

GradientClippingNode::GradientClippingNode()
    : m_clip(nullptr)
    , m_contents(nullptr)
    , m_drawingForPreview(false) {}

GradientClippingNode* GradientClippingNode::create(cocos2d::CCNode* clip) {
    auto ret = new GradientClippingNode;
    if (ret->init(clip)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool GradientClippingNode::init(cocos2d::CCNode* clip) {
    if (!CCLayerColor::init()) return false;

    m_clipNode = clip;

    setShaderProgram(g_clipShader);

    return true;
}

void GradientClippingNode::rerender() {
    auto scale = cocos2d::CCDirector::get()->getContentScaleFactor();
    
    auto clipRender = RenderTexture(
        m_clipNode->getContentWidth() * scale,
        m_clipNode->getContentHeight() * scale,
        GL_RGBA, GL_RGBA
    );
    kmGLPushMatrix();
    transform();
    clipRender.capture(m_clipNode);
    kmGLPopMatrix();
    m_clip = clipRender.intoManagedSprite();

    auto contentsRender = RenderTexture(
        this->getContentWidth() * scale,
        this->getContentHeight() * scale,
        GL_RGBA, GL_RGBA
    );
    m_drawingForPreview = true;
    contentsRender.capture(this);
    m_drawingForPreview = false;
    m_contents = contentsRender.intoManagedSprite();
}

void GradientClippingNode::draw() {
    if (m_drawingForPreview) {
        CCLayerColor::draw();
        return;
    }
    
    if (!m_clip || !m_contents) {
        geode::log::warn("GradientClippingNode::rerender has not been called before first draw (call it)!");
        return;
    }

    // a lot borrowed from cclayercolor
    CC_NODE_DRAW_SETUP();

    cocos2d::ccGLBlendFunc(m_tBlendFunc.src, m_tBlendFunc.dst);

    cocos2d::ccGLBindTexture2DN(0, m_clip->sprite->getTexture()->getName());
    cocos2d::ccGLBindTexture2DN(1, m_contents->sprite->getTexture()->getName());
    cocos2d::ccGLEnableVertexAttribs(cocos2d::kCCVertexAttribFlag_Position | cocos2d::kCCVertexAttribFlag_Color);

    glVertexAttribPointer(cocos2d::kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, m_pSquareVertices);
    glVertexAttribPointer(cocos2d::kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 0, m_pSquareColors);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    *getNumberOfDraws() += 1;
}

void GradientClippingNode::addChild(cocos2d::CCNode* child, int zOrder, int tag) {
    CCLayerColor::addChild(child, zOrder, tag);
    // rerender();
}

