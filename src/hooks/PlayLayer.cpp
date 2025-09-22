#include "PlayLayer.hpp"
#include "GameObject.hpp"
#include "../UnlockManager.hpp"

HookedPlayLayer::Fields::Fields()
    : m_objectIDsSeen({})
    , m_overlayLayer(nullptr)
    , m_started(false) {}

bool HookedPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
    if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

    sharedInit();

    return true;
}

cocos2d::CCScene* HookedPlayLayer::scene(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
    auto ret = PlayLayer::scene(level, useReplay, dontCreateObjects);

    if (UnlockManager::get().isInRun()) {
        auto playLayer = ret->getChildByIndex(0);
        ret->addChild(static_cast<HookedPlayLayer*>(playLayer)->m_fields->m_overlayLayer, 10);
    }

    return ret;
}

void HookedPlayLayer::startGame() {
    PlayLayer::startGame();
    m_fields->m_started = true;
}

void HookedPlayLayer::updateVisibility(float dt) {
    PlayLayer::updateVisibility(dt);

    if (!UnlockManager::get().isInRun()) return;
    
    auto fields = m_fields.self();
    if (!fields->m_started) return; // whether we're in the 1 second wait or not

    // m_activeObjectsCount != m_activeObjects.size()
    // m_activeObjectsIndex == m_activeObjects.size() (max size of m_activeObjects)
    // (rob doesnt believe in std::vector::max_size i guess)
    
    for (unsigned int i = 0; i < m_activeObjectsCount; i++) {
        auto object = static_cast<HookedGameObject*>(m_activeObjects[i]);
        potentiallyAddObject(object);
    }
}

void HookedPlayLayer::playEndAnimationToPos(cocos2d::CCPoint pos) {
    PlayLayer::playEndAnimationToPos(pos);
    sharedEnd();
}

void HookedPlayLayer::playPlatformerEndAnimationToPos(cocos2d::CCPoint pos, bool instant) {
    PlayLayer::playPlatformerEndAnimationToPos(pos, instant);
    sharedEnd();
}

void HookedPlayLayer::fullReset() {
    PlayLayer::fullReset();

    if (!UnlockManager::get().isInRun()) return;

    sharedInit(/* skip delay */ true);

    auto fields = m_fields.self();
    cocos2d::CCScene::get()->addChild(fields->m_overlayLayer, 10);
}

void HookedPlayLayer::onQuit() {
    PlayLayer::onQuit();
    sharedEnd();
}

void HookedPlayLayer::sharedEnd() {
    if (!UnlockManager::get().isInRun()) return;
    auto fields = m_fields.self();

    auto& unlockedObjectIDs = UnlockManager::get().m_runState.m_unlockedObjectIDs;
    unlockedObjectIDs.insert(unlockedObjectIDs.end(), fields->m_objectIDsSeen.begin(), fields->m_objectIDsSeen.end());
    
    fields->m_overlayLayer->animateAway();
}

void HookedPlayLayer::sharedInit(bool skipDelay) {
    if (!UnlockManager::get().isInRun()) return;

    if (m_player1)               reinterpret_cast<HookedGameObject*>(m_player1)->m_fields->m_ignore = true;
    if (m_player1CollisionBlock) reinterpret_cast<HookedGameObject*>(m_player1CollisionBlock)->m_fields->m_ignore = true;
    if (m_player2)               reinterpret_cast<HookedGameObject*>(m_player2)->m_fields->m_ignore = true;
    if (m_player2CollisionBlock) reinterpret_cast<HookedGameObject*>(m_player2CollisionBlock)->m_fields->m_ignore = true;
    if (m_anticheatSpike)        reinterpret_cast<HookedGameObject*>(m_anticheatSpike)->m_fields->m_ignore = true;

    m_fields->m_overlayLayer = ObjectCollectLayer::create(skipDelay);
}

void HookedPlayLayer::potentiallyAddObject(GameObject* object, bool skipTouchChecks) {
    auto fields = m_fields.self();
    auto cast = static_cast<HookedGameObject*>(object);
    auto objectFields = cast->m_fields.self();

    if (objectFields->m_ignore) return;

    if (std::find(fields->m_objectIDsSeen.begin(), fields->m_objectIDsSeen.end(), object->m_objectID) != fields->m_objectIDsSeen.end()) {
        // already seen an object id, dont calculate
        return;
    }

    if (!skipTouchChecks) {
        auto obb2d = cast->getOrientedBox();
        if (!obb2d) return;
    
        if (!obb2d->overlaps(m_player1->getOrientedBox())) {
            return;
        }
    }

    bool isValidObject = fields->m_overlayLayer->addObject(object->m_objectID);
    if (isValidObject) {
        fields->m_objectIDsSeen.push_back(object->m_objectID);
    }
}
