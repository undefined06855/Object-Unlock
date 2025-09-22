#include "GradientClippingNode.hpp"
#include <RenderTexture.hpp>

GradientClippingNode::GradientClippingNode()
    : m_clip(nullptr)
    , m_contents(nullptr) {}

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
    if (!CCNode::init()) return false;

    m_clipNode = clip;

    return true;
}

void GradientClippingNode::rerender() {
    auto scale = cocos2d::CCDirector::get()->getContentScaleFactor();
    
    auto capture = RenderTexture(
        m_clipNode->getContentWidth() * scale,
        m_clipNode->getContentHeight() * scale,
        GL_RGBA, GL_RGBA
    ).capture(m_clipNode);
}

void GradientClippingNode::draw() {
    if (!m_clip || !m_contents) {
        geode::log::warn("GradientClippingNode::rerender has not been called before first draw (call it)!");
        return;
    }

    // do the business
}

void GradientClippingNode::addChild(cocos2d::CCNode* child, int zOrder, int tag) {
    CCNode::addChild(child, zOrder, tag);
    // rerender();
}

