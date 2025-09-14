#include "GameObject.hpp"

HookedGameObject::Fields::Fields()
    : m_seen(false)
    , m_parent(nullptr) {}

// default colours for channels in a default level
cocos2d::ccColor3B HookedGameObject::colorForChannel(int channel) {
    switch (channel) {
        case 1002:
        case 1003:
        case 1004:
        case 1011:
        case 1012:
            return { 255, 255, 255 };
        
        case 1000:
        case 1013:
        case 1014:
            return { 40, 125, 255 };

        case 1001:
        case 1009:
            return { 0, 102, 255 };
        
        case 1010:
            return { 0, 0, 0 };
    }

    return { 255, 255, 255 };
}
    
// this function does not bring me joy
GameObject* HookedGameObject::createGameObject(int id) {
    auto gjbgl = GJBaseGameLayer::get();
    if (!gjbgl) return nullptr;

    std::vector<std::string> values = {};
    std::vector<void*> valuesExist = {};
    values.resize(600, "");
    valuesExist.resize(600, 0x0);

    values[1] = fmt::to_string(id);
    valuesExist[1] = gjbgl;

    // TODO: fix special child in toggle trigger

    auto object = GameObject::objectFromVector(values, valuesExist, gjbgl, false);
    if (!object) return nullptr;
    
    // set up some stuff for animated objects
    if (auto cast = geode::cast::typeinfo_cast<AnimatedGameObject*>(object)) {
        auto animatedSprite = cast->m_animatedSprite;

        cast->setContentSize(cast->m_lastSize);
        animatedSprite->setPosition(cast->m_lastSize / 2.f); // recenter with corrected size

        cast->m_isActivated = true;
        cast->m_playingAnimation = true;
        cast->updateObjectAnimation();

        // also set cascade opacity correctly
        // like half the children dont have members so
        animatedSprite->setCascadeOpacityEnabled(true);
        for (auto child : animatedSprite->getChildrenExt()) {
            if (auto cast = geode::cast::typeinfo_cast<cocos2d::CCNodeRGBA*>(child)) cast->setCascadeOpacityEnabled(true);
            for (auto child : child->getChildrenExt()) {
                if (auto cast = geode::cast::typeinfo_cast<cocos2d::CCNodeRGBA*>(child)) cast->setCascadeOpacityEnabled(true);
            }
        }
    }

    object->updateMainColor(colorForChannel(object->m_activeMainColorID));
    object->updateSecondaryColor(colorForChannel(object->m_activeDetailColorID));

    object->setCascadeOpacityEnabled(true);
    object->setPosition({ 0.f, 0.f });
    object->setAnchorPoint({ 0.f, 0.f });

    return object;
}
