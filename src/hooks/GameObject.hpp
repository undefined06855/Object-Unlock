#pragma once
#include <Geode/modify/GameObject.hpp>

class $modify(HookedGameObject, GameObject) {
    struct Fields {
        Fields();

        bool m_seen;
        geode::WeakRef<GameObject> m_parent;
    };

    static cocos2d::ccColor3B colorForChannel(int channel);
    static GameObject* createGameObject(int id);
};
