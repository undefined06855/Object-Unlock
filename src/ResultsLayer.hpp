#pragma once

class ResultsLayer : public cocos2d::CCLayer {
public:
    static cocos2d::CCScene* scene();
    static ResultsLayer* create();
    bool init();

    void keyBackClicked();
    void onEnterTransitionDidFinish();
};
