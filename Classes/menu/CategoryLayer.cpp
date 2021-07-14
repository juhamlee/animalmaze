//
//  CategoryLayer.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 05/01/2019.
//

#include "CategoryLayer.h"
#include "TitleLayer.h"
#include "StageSelectLayer.h"

#include "PluginAdMob/PluginAdMob.h"

static const int FONT_SIZE_TEXT = 50;
static const int FONT_SIZE_LEVEL = 30;

static const int TOP_Y = 1210;
static const int BTN_Y = 950;
static const int BTN_Y_OFFSET = -205;
static const int BTN_TEXT_X = 45;
static const int BTN_ICON_X = 372;
static const int BTN_PERCENT_X = 487;

static const Size BTN_SIZE = Size(569, 150);
static const Color3B BTN_COLORS[] = {
    Color3B(132, 252, 215),
    Color3B(146, 192, 254),
    Color3B(255, 166, 195),
    Color3B(179, 53, 249),
};

static const char* BTN_TEXTS[] = {
    "CLASSIC",
    "ICE FLOOR",
    "HURRY UP",
    "DARKNESS",
};

static const char* BTN_ICONS[] = {
    "ui/icon_maze.png",
    "ui/icon_ice.png",
    "ui/icon_clock.png",
    "ui/icon_dark.png",
};

Scene* CategoryLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = CategoryLayer::create();
    
    if(scene != nullptr && layer != nullptr) {
        scene->addChild(layer);
        return scene;
    }
    
    return nullptr;
}

bool CategoryLayer::init()
{
    Layer::init();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    auto bg = LayerColor::create(Color4B::BLACK, visibleSize.width, visibleSize.height);
    if(bg != nullptr) {
        bg->setPosition(center);
        this->addChild(bg, 0);
    }
    
    auto labTitle = Label::createWithTTF("CATEGORIES", FONT_PATH, FONT_SIZE_TEXT);
    if(labTitle) {
        labTitle->setPosition(center.x, TOP_Y);
        this->addChild(labTitle, 1);
    }
    
    auto btnTitle = MenuItemImage::create("ui/icon_home.png",
                                          "ui/icon_home.png",
                                          "ui/icon_home.png");
    if(btnTitle != nullptr) {
        btnTitle->setCallback(CC_CALLBACK_1(CategoryLayer::callbackTitle, this));
        btnTitle->setPosition(70, TOP_Y);
    }
    
    Menu* menu = Menu::create(btnTitle, NULL);
    if(menu) {
        menu->setPosition(0, 0);
        this->addChild(menu, 1);
    }
    
    auto sprStar = Sprite::create("ui/icon_star.png");
    if(sprStar != nullptr) {
        sprStar->setPosition(650, TOP_Y);
        this->addChild(sprStar, 1);
        
        char buf[30];
        sprintf(buf, "%d", ACCOUNT->level);
        auto labLevel = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_LEVEL);
        if(labLevel) {
            labLevel->setPosition(sprStar->getContentSize() * 0.5f);
            sprStar->addChild(labLevel);
        }
    }

    auto btnClassic = Button::create("ui/board_item.png");
    if(btnClassic != nullptr) {
        btnClassic->setScale9Enabled(true);
        btnClassic->setColor(BTN_COLORS[0]);
        btnClassic->setContentSize(BTN_SIZE);
        btnClassic->addClickEventListener(CC_CALLBACK_1(CategoryLayer::callbackClassic, this));
        btnClassic->setPosition(Vec2(center.x, BTN_Y));
        this->addChild(btnClassic, 0);
        
        auto labTitle = Label::createWithTTF(BTN_TEXTS[0], FONT_PATH, FONT_SIZE_TEXT);
        if(labTitle) {
            labTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labTitle->setPosition(BTN_TEXT_X, BTN_SIZE.height * 0.5f);
            btnClassic->addChild(labTitle);
        }
        
        auto sprIcon = Sprite::create(BTN_ICONS[0]);
        if(sprIcon) {
            sprIcon->setPosition(BTN_ICON_X, BTN_SIZE.height * 0.5f);
            btnClassic->addChild(sprIcon);
        }
        
        char buf[30];
        sprintf(buf, "%d%%", (int)ACCOUNT->percentStage[0]);
        auto labPercent = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_TEXT);
        if(labPercent) {
            labPercent->setPosition(BTN_PERCENT_X, BTN_SIZE.height * 0.5f);
            btnClassic->addChild(labPercent);
        }
    }
    
    auto btnIce = Button::create("ui/board_item.png");
    if(btnIce != nullptr) {
        btnIce->setScale9Enabled(true);
        btnIce->setColor(BTN_COLORS[1]);
        btnIce->setContentSize(BTN_SIZE);
        btnIce->addClickEventListener(CC_CALLBACK_1(CategoryLayer::callbackIce, this));
        btnIce->setPosition(Vec2(center.x, BTN_Y + BTN_Y_OFFSET * 1));
        this->addChild(btnIce, 0);
        
        auto labTitle = Label::createWithTTF(BTN_TEXTS[1], FONT_PATH, FONT_SIZE_TEXT);
        if(labTitle) {
            labTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labTitle->setPosition(BTN_TEXT_X, BTN_SIZE.height * 0.5f);
            btnIce->addChild(labTitle);
        }
        
        auto sprIcon = Sprite::create(BTN_ICONS[1]);
        if(sprIcon) {
            sprIcon->setPosition(BTN_ICON_X, BTN_SIZE.height * 0.5f);
            btnIce->addChild(sprIcon);
        }
        
        if(ACCOUNT->stateCategory[1] == Account::OPEN) {
            char buf[30];
            sprintf(buf, "%d%%", (int)ACCOUNT->percentStage[1]);
            auto labPercent = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_TEXT);
            if(labPercent) {
                labPercent->setPosition(BTN_PERCENT_X, BTN_SIZE.height * 0.5f);
                btnIce->addChild(labPercent);
            }
        }
        else {
            btnIce->setTouchEnabled(false);
            
            auto sprLock = Sprite::create("ui/icon_lock.png");
            if(sprLock) {
                sprLock->setPosition(BTN_PERCENT_X, BTN_SIZE.height * 0.5f);
                btnIce->addChild(sprLock);
            }
            
            auto sprCover = Scale9Sprite::create("ui/board_item.png");
            if(sprCover) {
                sprCover->setColor(Color3B(59, 59, 59));
                sprCover->setOpacity(150);
                sprCover->setContentSize(BTN_SIZE);
                sprCover->setPosition(Vec2(center.x, BTN_Y + BTN_Y_OFFSET * 1));
                this->addChild(sprCover, 0);
            }
        }
    }
    
    auto btnHurry = Button::create("ui/board_item.png");
    if(btnHurry != nullptr) {
        btnHurry->setScale9Enabled(true);
        btnHurry->setColor(BTN_COLORS[2]);
        btnHurry->setContentSize(BTN_SIZE);
        btnHurry->addClickEventListener(CC_CALLBACK_1(CategoryLayer::callbackHurry, this));
        btnHurry->setPosition(Vec2(center.x, BTN_Y + BTN_Y_OFFSET * 2));
        this->addChild(btnHurry, 0);
        
        auto labTitle = Label::createWithTTF(BTN_TEXTS[2], FONT_PATH, FONT_SIZE_TEXT);
        if(labTitle) {
            labTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labTitle->setPosition(BTN_TEXT_X, BTN_SIZE.height * 0.5f);
            btnHurry->addChild(labTitle);
        }
        
        auto sprIcon = Sprite::create(BTN_ICONS[2]);
        if(sprIcon) {
            sprIcon->setPosition(BTN_ICON_X, BTN_SIZE.height * 0.5f);
            btnHurry->addChild(sprIcon);
        }
        
        if(ACCOUNT->stateCategory[2] == Account::OPEN) {
            char buf[30];
            sprintf(buf, "%d%%", (int)ACCOUNT->percentStage[2]);
            auto labPercent = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_TEXT);
            if(labPercent) {
                labPercent->setPosition(BTN_PERCENT_X, BTN_SIZE.height * 0.5f);
                btnHurry->addChild(labPercent);
            }
        }
        else {
            btnHurry->setTouchEnabled(false);
            
            auto sprLock = Sprite::create("ui/icon_lock.png");
            if(sprLock) {
                sprLock->setPosition(BTN_PERCENT_X, BTN_SIZE.height * 0.5f);
                btnHurry->addChild(sprLock);
            }
            
            auto sprCover = Scale9Sprite::create("ui/board_item.png");
            if(sprCover) {
                sprCover->setColor(Color3B(59, 59, 59));
                sprCover->setOpacity(150);
                sprCover->setContentSize(BTN_SIZE);
                sprCover->setPosition(Vec2(center.x, BTN_Y + BTN_Y_OFFSET * 2));
                this->addChild(sprCover, 0);
            }
        }
    }
    
    auto btnDark = Button::create("ui/board_item.png");
    if(btnDark != nullptr) {
        btnDark->setScale9Enabled(true);
        btnDark->setColor(BTN_COLORS[3]);
        btnDark->setContentSize(BTN_SIZE);
        btnDark->addClickEventListener(CC_CALLBACK_1(CategoryLayer::callbackDark, this));
        btnDark->setPosition(Vec2(center.x, BTN_Y + BTN_Y_OFFSET * 3));
        this->addChild(btnDark, 0);
        
        auto labTitle = Label::createWithTTF(BTN_TEXTS[3], FONT_PATH, FONT_SIZE_TEXT);
        if(labTitle) {
            labTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labTitle->setPosition(BTN_TEXT_X, BTN_SIZE.height * 0.5f);
            btnDark->addChild(labTitle);
        }
        
        auto sprIcon = Sprite::create(BTN_ICONS[3]);
        if(sprIcon) {
            sprIcon->setPosition(BTN_ICON_X, BTN_SIZE.height * 0.5f);
            btnDark->addChild(sprIcon);
        }
        
        if(ACCOUNT->stateCategory[3] == Account::OPEN) {
            char buf[30];
            sprintf(buf, "%d%%", (int)ACCOUNT->percentStage[3]);
            auto labPercent = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_TEXT);
            if(labPercent) {
                labPercent->setPosition(BTN_PERCENT_X, BTN_SIZE.height * 0.5f);
                btnDark->addChild(labPercent);
            }
        }
        else {
            btnDark->setTouchEnabled(false);
            
            auto sprLock = Sprite::create("ui/icon_lock.png");
            if(sprLock) {
                sprLock->setPosition(BTN_PERCENT_X, BTN_SIZE.height * 0.5f);
                btnDark->addChild(sprLock);
            }
            
            auto sprCover = Scale9Sprite::create("ui/board_item.png");
            if(sprCover) {
                sprCover->setColor(Color3B(59, 59, 59));
                sprCover->setOpacity(150);
                sprCover->setContentSize(BTN_SIZE);
                sprCover->setPosition(Vec2(center.x, BTN_Y + BTN_Y_OFFSET * 3));
                this->addChild(sprCover, 0);
            }
        }
    }
    
    return true;
}

void CategoryLayer::onEnter() {
    Layer::onEnter();
    
    keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = CC_CALLBACK_2(CategoryLayer::onKeyReleased, this);
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(keyListener, this);
    
    if(sdkbox::PluginAdMob::isAvailable("home"))
        sdkbox::PluginAdMob::show("home");
    else
        sdkbox::PluginAdMob::cache("home");
}

void CategoryLayer::onExit() {
    EVENT_DISPATCHER->removeEventListener(keyListener);
    
    Layer::onExit();
}

void CategoryLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event) {
    if(keycode == EventKeyboard::KeyCode::KEY_BACK) {
        auto scene = TitleLayer::createScene();
        auto director = Director::getInstance();
        director->replaceScene(scene);
        
        AUDIO->playEffect("sfx/click.mp3");
    }
}

void CategoryLayer::callbackTitle(Ref* pSender) {
    auto scene = TitleLayer::createScene();
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/click.mp3");
}

void CategoryLayer::callbackClassic(Ref* pSender) {
    auto scene = StageSelectLayer::createScene(StageManager::CATEGORY_CLASSIC);
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/mode.mp3");
}

void CategoryLayer::callbackIce(Ref* pSender) {
    auto scene = StageSelectLayer::createScene(StageManager::CATEGORY_ICE);
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/mode.mp3");
}

void CategoryLayer::callbackHurry(Ref* pSender) {
    auto scene = StageSelectLayer::createScene(StageManager::CATEGORY_HURRYUP);
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/mode.mp3");
}

void CategoryLayer::callbackDark(Ref* pSender) {
    auto scene = StageSelectLayer::createScene(StageManager::CATEGORY_DARKNESS);
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/mode.mp3");
}
