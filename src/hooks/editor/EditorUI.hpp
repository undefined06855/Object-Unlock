#pragma once
#include <Geode/modify/EditorUI.hpp>

class $modify(HookedEditorUI, EditorUI) {
    CreateMenuItem* getCreateBtn(int id, int bg);
};
