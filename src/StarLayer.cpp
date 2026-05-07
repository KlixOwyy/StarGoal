#include "StarLayer.hpp"

#include <Geode/loader/Mod.hpp>

#include <ctime>

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
    if (!CCLayerColor::initWithColor({ 0, 0, 0, 180 }))
        return false;

    // blokowanie kliknięć
    this->setTouchEnabled(true);

    auto winSize =
        CCDirector::sharedDirector()->getWinSize();

    // =========================
    // PANEL
    // =========================

    auto panel = CCLayerColor::create({
        25,
        25,
        25,
        240
        });

    panel->setContentSize({
        320,
        220
        });

    panel->setPosition({
        winSize.width / 2 - 160,
        winSize.height / 2 - 110
        });

    this->addChild(panel);

    // =========================
    // TYTUŁ
    // =========================

    auto title = CCLabelBMFont::create(
        "StarGoal",
        "bigFont.fnt"
    );

    title->setScale(0.7f);

    title->setPosition({
        winSize.width / 2,
        winSize.height / 2 + 80
        });

    this->addChild(title);

    // =========================
    // DATA
    // =========================

    time_t now = time(nullptr);

    tm localTime{};

    localtime_s(&localTime, &now);

    char dateBuffer[32];

    strftime(
        dateBuffer,
        sizeof(dateBuffer),
        "%Y-%m-%d",
        &localTime
    );

    std::string currentDate = dateBuffer;

    // =========================
    // GWIAZDY
    // =========================

    int currentStars =
        GameStatsManager::sharedState()->getStat("6");

    auto mod = Mod::get();

    std::string savedDate =
        mod->getSavedValue<std::string>(
            "saved-date",
            currentDate
        );

    int startOfDayStars =
        mod->getSavedValue<int>(
            "start-day-stars",
            currentStars
        );

    // =========================
    // RESET DNIA
    // =========================

    if (savedDate != currentDate) {
        savedDate = currentDate;

        startOfDayStars = currentStars;

        mod->setSavedValue(
            "saved-date",
            currentDate
        );

        mod->setSavedValue(
            "start-day-stars",
            currentStars
        );
    }

    // =========================
    // TODAY
    // =========================

    int gainedToday =
        currentStars - startOfDayStars;

    if (gainedToday < 0)
        gainedToday = 0;

    // =========================
    // WEEKLY GOAL
    // =========================

    int weeklyGoal =
        mod->getSavedValue<int>(
            "weekly-goal",
            1000
        );

    // dzień tygodnia
    int currentWeekDay =
        localTime.tm_wday;

    if (currentWeekDay == 0)
        currentWeekDay = 7;

    // ile dni zostało
    int daysLeft =
        8 - currentWeekDay;

    if (daysLeft <= 0)
        daysLeft = 1;

    // ile zostało do celu
    int starsLeft =
        weeklyGoal - gainedToday;

    if (starsLeft < 0)
        starsLeft = 0;

    // ile trzeba dziś
    int needToday =
        starsLeft / daysLeft;

    if (needToday < 0)
        needToday = 0;

    // =========================
    // PROCENT
    // =========================

    float progress =
        static_cast<float>(gainedToday) /
        static_cast<float>(weeklyGoal);

    if (progress > 1.f)
        progress = 1.f;

    int percent =
        static_cast<int>(progress * 100.f);

    // =========================
    // LABELS
    // =========================

    auto starsLabel = CCLabelBMFont::create(
        ("Stars: " +
            std::to_string(currentStars)).c_str(),
        "goldFont.fnt"
    );

    starsLabel->setAnchorPoint({ 0.f, 0.5f });

    starsLabel->setScale(0.45f);

    starsLabel->setPosition({
        winSize.width / 2 - 130,
        winSize.height / 2 + 40
        });

    this->addChild(starsLabel);

    auto todayLabel = CCLabelBMFont::create(
        ("Today: +" +
            std::to_string(gainedToday)).c_str(),
        "goldFont.fnt"
    );

    todayLabel->setAnchorPoint({ 0.f, 0.5f });

    todayLabel->setScale(0.45f);

    todayLabel->setPosition({
        winSize.width / 2 - 130,
        winSize.height / 2 + 10
        });

    this->addChild(todayLabel);

    auto goalLabel = CCLabelBMFont::create(
        ("Weekly Goal: " +
            std::to_string(weeklyGoal)).c_str(),
        "goldFont.fnt"
    );

    goalLabel->setAnchorPoint({ 0.f, 0.5f });

    goalLabel->setScale(0.45f);

    goalLabel->setPosition({
        winSize.width / 2 - 130,
        winSize.height / 2 - 20
        });

    this->addChild(goalLabel);

    auto needLabel = CCLabelBMFont::create(
        ("Need Today: " +
            std::to_string(needToday)).c_str(),
        "goldFont.fnt"
    );

    needLabel->setAnchorPoint({ 0.f, 0.5f });

    needLabel->setScale(0.45f);

    needLabel->setPosition({
        winSize.width / 2 - 130,
        winSize.height / 2 - 50
        });

    this->addChild(needLabel);

    auto progressLabel = CCLabelBMFont::create(
        ("Progress: " +
            std::to_string(percent) + "%").c_str(),
        "goldFont.fnt"
    );

    progressLabel->setScale(0.45f);

    progressLabel->setPosition({
        winSize.width / 2,
        winSize.height / 2 - 90
        });

    this->addChild(progressLabel);

    // =========================
    // CLOSE BUTTON
    // =========================

    auto closeBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName(
            "GJ_closeBtn_001.png"
        ),
        this,
        menu_selector(StarLayer::onClose)
    );

    closeBtn->setPosition({
        145,
        95
        });

    auto menu = CCMenu::create();

    menu->setPosition({
        winSize.width / 2,
        winSize.height / 2
        });

    menu->addChild(closeBtn);

    this->addChild(menu);

    return true;
}

void StarLayer::onClose(CCObject*) {
    this->removeFromParentAndCleanup(true);
}

void StarLayer::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()
        ->getTouchDispatcher()
        ->addTargetedDelegate(
            this,
            -999,
            true
        );
}

bool StarLayer::ccTouchBegan(
    CCTouch*,
    CCEvent*
) {
    return true;
}