#include "GameObject.hpp"

HookedGameObject::Fields::Fields()
    : m_ignore(false) {}

// TODO: gameplay blocks (h blocks etc) with cclabelbmfont

// default colours for channels in a default level
// mostly just used for white/black
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
// see leveleditorlayer/editorui createObject for references about a few things here
GameObject* HookedGameObject::createGameObject(int id) {
    auto gjbgl = GJBaseGameLayer::get();
    if (!gjbgl) return nullptr;

    // gameobject createWithKey isnt the most consistent function
    // since they are passed as references they must be gd:: and not std:: 
    std::vector<std::string> values = {};
    std::vector<void*> valuesExist = {};
    values.resize(600, "");
    valuesExist.resize(600, 0x0);

    values[1] = fmt::to_string(id);
    valuesExist[1] = gjbgl;

    // this code makes me sad
    // .resize doesnt work
    gd::vector<gd::string> gdVectorValues;
    gdVectorValues.reserve(600);
    for (int i = 0; i < 600; i++) { gdVectorValues.push_back(values[i]); }

    gd::vector<void*> gdVectorValuesExist = valuesExist;

    auto object = GameObject::objectFromVector(gdVectorValues, gdVectorValuesExist, gjbgl, false);
    if (!object) return nullptr;
    
    // set up some stuff for animated objects
    // m_lastSize stores the correct size set up in animatedgameobject init
    // https://discord.com/channels/911701438269386882/911702535373475870/1416784124324221048
    if (auto cast = geode::cast::typeinfo_cast<AnimatedGameObject*>(object)) {
        auto animatedSprite = cast->m_animatedSprite;

        cast->setContentSize(cast->m_lastSize);
        animatedSprite->setPosition(cast->m_lastSize / 2.f); // recenter with corrected size

        // start default idle01 animation
        cast->m_isActivated = true; // animated gameobjects only animate if active (onscreen)
        cast->m_playingAnimation = true;
        cast->updateObjectAnimation();
    }

    // toggle trigger :(
    // cant get colorsprite to work
    if (id == 1049) {
        auto sprite = cocos2d::CCSprite::createWithSpriteFrameName("edit_eToggleBtn2_001.png");
        sprite->setColor({ 255, 63, 63 });
        sprite->setZOrder(-1);
        sprite->setTag(998);
        sprite->setPosition(object->getContentSize() / 2.f);
        // object->m_colorSprite = sprite; // crashes in gameobject setScaleY
        object->addChild(sprite);
    }

    object->updateMainColor(colorForChannel(object->m_activeMainColorID));
    object->updateSecondaryColor(colorForChannel(object->m_activeDetailColorID));
    if (object->m_pixelScaleX != 1.f || object->m_pixelScaleY != 1.f) {
        object->updateCustomScaleX(object->m_pixelScaleX);
        object->updateCustomScaleY(object->m_pixelScaleY);
    }

    // also set cascade opacity correctly just in case
    // there's loads of nodes without members and loads with so i cant be bothered
    object->setCascadeOpacityEnabled(true);
    for (auto child : object->getChildrenExt()) {
        if (auto cast = geode::cast::typeinfo_cast<cocos2d::CCNodeRGBA*>(child)) cast->setCascadeOpacityEnabled(true);
        for (auto child : child->getChildrenExt()) {
            if (auto cast = geode::cast::typeinfo_cast<cocos2d::CCNodeRGBA*>(child)) cast->setCascadeOpacityEnabled(true);
        }
    }

    object->setPosition({ 0.f, 0.f });
    object->setAnchorPoint({ 0.f, 0.f });

    object->setTag(id); // for further debugging later

    return object;
}
