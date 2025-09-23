#include "PlayLayer.hpp"

#define HOOK_TRIGGEROBJECT(className) \
    class $modify(className) { \
        void triggerObject(GJBaseGameLayer* p0, int p1, const gd::vector<int>* p2) { \
            className::triggerObject(p0, p1, p2); \
            auto cast = static_cast<HookedPlayLayer*>(PlayLayer::get()); \
            if (!cast) return; \
            cast->potentiallyAddObject(this, /* skip touch checks */ true); \
        } \
    }

#include <Geode/modify/AdvancedFollowTriggerObject.hpp>
HOOK_TRIGGEROBJECT(AdvancedFollowTriggerObject);

#include <Geode/modify/ArtTriggerGameObject.hpp>
HOOK_TRIGGEROBJECT(ArtTriggerGameObject);

#include <Geode/modify/CameraTriggerGameObject.hpp>
HOOK_TRIGGEROBJECT(CameraTriggerGameObject);

#include <Geode/modify/CheckpointGameObject.hpp>
HOOK_TRIGGEROBJECT(CheckpointGameObject);

#include <Geode/modify/CountTriggerGameObject.hpp>
HOOK_TRIGGEROBJECT(CountTriggerGameObject);

#include <Geode/modify/EffectGameObject.hpp>
HOOK_TRIGGEROBJECT(EffectGameObject);

#include <Geode/modify/EndTriggerGameObject.hpp>
HOOK_TRIGGEROBJECT(EndTriggerGameObject);

#include <Geode/modify/EventLinkTrigger.hpp>
HOOK_TRIGGEROBJECT(EventLinkTrigger);

#include <Geode/modify/ItemTriggerGameObject.hpp>
HOOK_TRIGGEROBJECT(ItemTriggerGameObject);

#include <Geode/modify/RandTriggerGameObject.hpp>
HOOK_TRIGGEROBJECT(RandTriggerGameObject);

#include <Geode/modify/SequenceTriggerGameObject.hpp>
HOOK_TRIGGEROBJECT(SequenceTriggerGameObject);

#include <Geode/modify/SpawnTriggerGameObject.hpp>
HOOK_TRIGGEROBJECT(SpawnTriggerGameObject);

#include <Geode/modify/TimerTriggerGameObject.hpp>
HOOK_TRIGGEROBJECT(TimerTriggerGameObject);

#include <Geode/modify/TransformTriggerGameObject.hpp>
HOOK_TRIGGEROBJECT(TransformTriggerGameObject);

#include <Geode/modify/TriggerControlGameObject.hpp>
HOOK_TRIGGEROBJECT(TriggerControlGameObject);
