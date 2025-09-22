#pragma once

struct RunConfig {
    std::chrono::minutes m_length;
};

struct RunState {
    std::vector<int> m_unlockedObjectIDs;
    std::chrono::high_resolution_clock::time_point m_endTime;
    bool m_playedMinuteAnimation;
    bool m_playedTenSecondAnimation;
};

enum class ObjectUnlockState {
    None = 0,
    InRun,
    InEditor
};

class UnlockManager : public cocos2d::CCObject {
    UnlockManager();
public:
    static UnlockManager& get();

    RunState m_runState;
    cocos2d::CCLabelBMFont* m_label;
    ObjectUnlockState m_state;

    bool m_blockTouches;

    bool isInRun();
    bool isInEditor();

    void startRun(const RunConfig& config);
    void endRun();
    void endEntirely();

    void update(float dt);
};
