#include "PlayLayer.hpp"
#include "GameObject.hpp"

HookedPlayLayer::Fields::Fields()
    : m_objectIDsSeen({})
    , m_overlayLayer(nullptr)
    , m_started(false) {}

bool HookedPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
    if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

    if (m_player1)               reinterpret_cast<HookedGameObject*>(m_player1)->m_fields->m_seen = true;
    if (m_player1CollisionBlock) reinterpret_cast<HookedGameObject*>(m_player1CollisionBlock)->m_fields->m_seen = true;
    if (m_player2)               reinterpret_cast<HookedGameObject*>(m_player2)->m_fields->m_seen = true;
    if (m_player2CollisionBlock) reinterpret_cast<HookedGameObject*>(m_player2CollisionBlock)->m_fields->m_seen = true;
    if (m_anticheatSpike)        reinterpret_cast<HookedGameObject*>(m_anticheatSpike)->m_fields->m_seen = true;

    m_fields->m_overlayLayer = ObjectCollectLayer::create();

    return true;
}

cocos2d::CCScene* HookedPlayLayer::scene(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
    auto ret = PlayLayer::scene(level, useReplay, dontCreateObjects);

    auto playLayer = ret->getChildByIndex(0);
    ret->addChild(static_cast<HookedPlayLayer*>(playLayer)->m_fields->m_overlayLayer, 10);

    return ret;
}

void HookedPlayLayer::startGame() {
    PlayLayer::startGame();
    m_fields->m_started = true;
}

void HookedPlayLayer::updateVisibility(float dt) {
    PlayLayer::updateVisibility(dt);
    
    auto fields = m_fields.self();
    if (!fields->m_started) return;

    // m_activeObjectsCount != m_activeObjects.size()
    // m_activeObjectsIndex == m_activeObjects.size() (max size of m_activeObjects)
    // (rob doesnt believe in std::vector::max_size i guess)
    
    // TODO: maybe make it so its only the objects you touch?

    for (unsigned int i = 0; i < m_activeObjectsCount; i++) {
        auto object = static_cast<HookedGameObject*>(m_activeObjects[i]);
        auto objectFields = object->m_fields.self();

        if (objectFields->m_seen) continue;
        objectFields->m_seen = true;

        if (std::find(fields->m_objectIDsSeen.begin(), fields->m_objectIDsSeen.end(), object->m_objectID) != fields->m_objectIDsSeen.end()) continue;

        bool isValidObject = fields->m_overlayLayer->addObject(object->m_objectID);
        if (isValidObject) {
            fields->m_objectIDsSeen.push_back(object->m_objectID);
        }
    }
}

void HookedPlayLayer::playEndAnimationToPos(cocos2d::CCPoint pos) {
    PlayLayer::playEndAnimationToPos(pos);
    m_fields->m_overlayLayer->animateAway();
}

void HookedPlayLayer::playPlatformerEndAnimationToPos(cocos2d::CCPoint pos, bool instant) {
    PlayLayer::playPlatformerEndAnimationToPos(pos, instant);
    m_fields->m_overlayLayer->animateAway();
}

void HookedPlayLayer::fullReset() {
    PlayLayer::fullReset();

    if (m_player1)               reinterpret_cast<HookedGameObject*>(m_player1)->m_fields->m_seen = true;
    if (m_player1CollisionBlock) reinterpret_cast<HookedGameObject*>(m_player1CollisionBlock)->m_fields->m_seen = true;
    if (m_player2)               reinterpret_cast<HookedGameObject*>(m_player2)->m_fields->m_seen = true;
    if (m_player2CollisionBlock) reinterpret_cast<HookedGameObject*>(m_player2CollisionBlock)->m_fields->m_seen = true;
    if (m_anticheatSpike)        reinterpret_cast<HookedGameObject*>(m_anticheatSpike)->m_fields->m_seen = true;

    auto fields = m_fields.self();
    fields->m_overlayLayer = ObjectCollectLayer::create();
    cocos2d::CCScene::get()->addChild(fields->m_overlayLayer, 10);
}
