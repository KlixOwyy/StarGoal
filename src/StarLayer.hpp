#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class StarLayer : public CCLayer {
public:
    static StarLayer* create();
    bool init();

    void onClose(CCObject*);
};