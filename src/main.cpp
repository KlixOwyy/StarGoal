#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "StarLayer.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto sprite = CCSprite::create("icon.png"_spr);
        sprite->setScale(0.7f);

        auto btn = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(MyMenuLayer::onStarGoal)
        );

        btn->setPosition({ 0, -100 });

        auto menu = this->getChildByID("bottom-menu");
        if (menu) {
            menu->addChild(btn);
        }

        return true;
    }

    void onStarGoal(CCObject*) {
        auto layer = StarLayer::create();
        CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 999);
    }
};