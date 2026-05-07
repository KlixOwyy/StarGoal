#include "StarLayer.hpp"

#include <Geode/utils/cocos.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

StarLayer* StarLayer::create() {
    auto ret = new StarLayer();

    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool StarLayer::init() {
    if (!CCLayer::init())
        return false;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    // =========================
    // TŁO
    // =========================

    auto bg = CCLayerColor::create({ 0, 0, 0, 180 });
    this->addChild(bg);

    // =========================
    // TYTUŁ
    // =========================

    auto title = CCLabelBMFont::create(
        "StarGoal",
        "bigFont.fnt"
    );

    title->setPosition({
        winSize.width / 2,
        winSize.height - 50
        });

    this->addChild(title);

    // =========================
    // GWIAZDY
    // =========================

    int stars = GameStatsManager::sharedState()->getStat("6");

    // poprzednio zapisane
    int savedStars =
        Mod::get()->getSavedValue<int>(
            "saved-stars",
            stars
        );

    // zdobyte od ostatniego zapisu
    int gainedToday = stars - savedStars;

    if (gainedToday < 0)
        gainedToday = 0;

    // =========================
    // CELE
    // =========================

    int weeklyGoal = 1000;

    int daysLeft = 7;

    int starsLeft = weeklyGoal - gainedToday;

    if (starsLeft < 0)
        starsLeft = 0;

    int needToday = starsLeft / daysLeft;

    // =========================
    // TEKST
    // =========================

    std::string text =
        "Stars: " + std::to_string(stars) +
        "\nToday: +" + std::to_string(gainedToday) +
        "\nWeekly Goal: " + std::to_string(weeklyGoal) +
        "\nNeed Today: " + std::to_string(needToday);

    auto info = CCLabelBMFont::create(
        text.c_str(),
        "goldFont.fnt"
    );

    info->setScale(0.5f);

    info->setPosition({
        winSize.width / 2,
        winSize.height / 2
        });

    this->addChild(info);

    // =========================
    // ZAPIS
    // =========================

    Mod::get()->setSavedValue(
        "saved-stars",
        stars
    );

    // =========================
    // PRZYCISK ZAMKNIĘCIA
    // =========================

    auto closeBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName(
            "GJ_closeBtn_001.png"
        ),
        this,
        menu_selector(StarLayer::onClose)
    );

    closeBtn->setPosition({
        winSize.width - 30,
        winSize.height - 30
        });

    auto menu = CCMenu::create();

    menu->setPosition({ 0, 0 });

    menu->addChild(closeBtn);

    this->addChild(menu);

    return true;
}

void StarLayer::onClose(CCObject*) {
    this->removeFromParentAndCleanup(true);
}