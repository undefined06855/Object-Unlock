#pragma once
#include <Geode/modify/GameObject.hpp>

class $modify(HookedGameObject, GameObject) {
    struct Fields {
        Fields();
        bool m_ignore;
    };

    static cocos2d::ccColor3B colorForChannel(int channel);
    static GameObject* createGameObject(int id);
};
