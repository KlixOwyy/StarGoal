#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class StarLayer : public CCLayerColor {
public:
    static StarLayer* create();

    bool init();

    void onClose(CCObject*);

    virtual void registerWithTouchDispatcher();

    bool ccTouchBegan(
        cocos2d::CCTouch* touch,
        cocos2d::CCEvent* event
    );
};