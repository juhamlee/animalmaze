//
//  TitleLayer.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 05/01/2019.
//

#include "TitleLayer.h"
#include "CategoryLayer.h"
#include "ShopLayer.h"
#include "MoreLayer.h"
#include "PopupManager.h"
#include "ExitPopup.h"
#include "DailyPopup.h"

#include "PluginAdMob/PluginAdMob.h"

static const int FONT_SIZE_TITLE = 100;
static const int FONT_SIZE_LEVEL = 30;

static const Color3B BTN_COLORS[5] = {
    Color3B(76, 136, 183),
    Color3B(100, 137, 27),
    Color3B(114, 21, 48),
    Color3B(93, 165, 171),
    Color3B(226, 178, 47)
};

Scene* TitleLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = TitleLayer::create();
    
    if(scene != nullptr && layer != nullptr) {
        scene->addChild(layer);
        
        PopupManager::getInstance()->initWithBaseNode(layer);
        
        return scene;
    }
    
    return nullptr;
}

bool TitleLayer::init()
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
    
    auto sprStar = Sprite::create("ui/icon_star.png");
    if(sprStar != nullptr) {
        sprStar->setPosition(650, 1210);
        this->addChild(sprStar, 1);
        
        char buf[30];
        sprintf(buf, "%d", ACCOUNT->level);
        auto labLevel = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_LEVEL);
        if(labLevel) {
            labLevel->setPosition(sprStar->getContentSize() * 0.5f);
            sprStar->addChild(labLevel);
        }
    }
    
    auto labTitle = Label::createWithTTF("ANIMAL\nMAZE", FONT_PATH, FONT_SIZE_TITLE);
    if(labTitle != nullptr) {
        labTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labTitle->setPosition(center.x, 1071);
        this->addChild(labTitle, 1);
    }
    
    auto sprTitle = Sprite::create("ui/title.png");
    if(sprTitle != nullptr) {
        sprTitle->setPosition(center.x, 467);
        this->addChild(sprTitle, 1);
    }
    
    auto sprMaze = Sprite::create("ui/title_maze.png");
    if(sprMaze != nullptr) {
        sprMaze->setPosition(center.x, 749);
        this->addChild(sprMaze, 1);
    }
    
    auto btnPlay = MenuItemImage::create("ui/icon_play.png",
                                         "ui/icon_play.png");
    if(btnPlay != nullptr) {
        btnPlay->setCallback(CC_CALLBACK_1(TitleLayer::callbackPlay, this));
        btnPlay->setScale(0.71f);
        btnPlay->setPosition(center.x + 15, 749);
    }
    
    auto btnVolume = MenuItemImage::create("ui/btn_title.png",
                                      "ui/btn_title.png");
    if(btnVolume != nullptr) {
        btnVolume->setCallback(CC_CALLBACK_1(TitleLayer::callbackVolume, this));
        btnVolume->setPosition(82, 163);
        btnVolume->setColor(BTN_COLORS[0]);
        
        if(ACCOUNT->isMute == false) {
            sprVolume = Sprite::create("ui/icon_volume.png");
            if(sprVolume != nullptr) {
                sprVolume->setPosition(btnVolume->getPosition());
                this->addChild(sprVolume, 2);
            }
        }
        else {
            sprVolume = Sprite::create("ui/icon_volume_off.png");
            if(sprVolume != nullptr) {
                sprVolume->setPosition(btnVolume->getPosition());
                this->addChild(sprVolume, 2);
            }
        }
    }
    
    auto btnRank = MenuItemImage::create("ui/btn_title.png",
                                         "ui/btn_title.png");
    if(btnRank != nullptr) {
        btnRank->setCallback(CC_CALLBACK_1(TitleLayer::callbackRank, this));
        btnRank->setPosition(82 + 138, 163);
        btnRank->setColor(BTN_COLORS[1]);
        
        auto sprIcon = Sprite::create("ui/icon_crown.png");
        if(sprIcon != nullptr) {
            sprIcon->setPosition(btnRank->getPosition());
            this->addChild(sprIcon, 2);
        }
    }
    
    auto btnLike = MenuItemImage::create("ui/btn_title.png",
                                         "ui/btn_title.png");
    if(btnLike != nullptr) {
        btnLike->setCallback(CC_CALLBACK_1(TitleLayer::callbackLike, this));
        btnLike->setPosition(82 + 138 * 2, 163);
        btnLike->setColor(BTN_COLORS[2]);
        
        auto sprIcon = Sprite::create("ui/icon_like.png");
        if(sprIcon != nullptr) {
            sprIcon->setPosition(btnLike->getPosition());
            this->addChild(sprIcon, 2);
        }
    }
    
    auto btnMore = MenuItemImage::create("ui/btn_title.png",
                                         "ui/btn_title.png");
    if(btnMore != nullptr) {
        btnMore->setCallback(CC_CALLBACK_1(TitleLayer::callbackMore, this));
        btnMore->setPosition(82 + 138 * 3, 163);
        btnMore->setColor(BTN_COLORS[3]);
        
        auto sprIcon = Sprite::create("ui/icon_more.png");
        if(sprIcon != nullptr) {
            sprIcon->setPosition(btnMore->getPosition());
            this->addChild(sprIcon, 2);
        }
    }
    
    auto btnShop = MenuItemImage::create("ui/btn_title.png",
                                         "ui/btn_title.png");
    if(btnShop != nullptr) {
        btnShop->setCallback(CC_CALLBACK_1(TitleLayer::callbackShop, this));
        btnShop->setPosition(82 + 138 * 4, 163);
        btnShop->setColor(BTN_COLORS[4]);
        
        auto sprIcon = Sprite::create("ui/icon_shop.png");
        if(sprIcon != nullptr) {
            sprIcon->setPosition(btnShop->getPosition());
            this->addChild(sprIcon, 2);
        }
    }
    
    Menu* menu = Menu::create(btnPlay, btnVolume, btnRank, btnLike, btnMore, btnShop, NULL);
    if(menu) {
        menu->setPosition(0, 0);
        this->addChild(menu, 1);
    }
    
    return true;
}

void TitleLayer::onEnter() {
    Layer::onEnter();
    
    keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = CC_CALLBACK_2(TitleLayer::onKeyReleased, this);
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(keyListener, this);
    
    if(sdkbox::PluginAdMob::isAvailable("home"))
        sdkbox::PluginAdMob::show("home");
    else
        sdkbox::PluginAdMob::cache("home");
    
    scheduleOnce(schedule_selector(TitleLayer::checkDailyBonus), 0.1f);
}

void TitleLayer::onExit() {
    EVENT_DISPATCHER->removeEventListener(keyListener);
    
    Layer::onExit();
}

void TitleLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event) {
    if(keycode == EventKeyboard::KeyCode::KEY_BACK) {
        auto popup = ExitPopup::create();
        if(popup != nullptr) {
            PopupManager::getInstance()->addPopup(popup);
        }
        
        AUDIO->playEffect("sfx/click.mp3");
    }
}

void TitleLayer::callbackPlay(Ref* pSender) {
    auto scene = CategoryLayer::createScene();
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/play.mp3");
}

void TitleLayer::callbackVolume(Ref* pSender) {
    ACCOUNT->isMute = ACCOUNT->isMute ? false : true;
    ACCOUNT->sync();
    
    if(ACCOUNT->isMute == false) {
        sprVolume->setTexture("ui/icon_volume.png");
        AUDIO->setBackgroundMusicVolume(1.f);
        AUDIO->setEffectsVolume(1.f);
    }
    else {
        sprVolume->setTexture("ui/icon_volume_off.png");
        AUDIO->setBackgroundMusicVolume(FLT_MIN);
        AUDIO->setEffectsVolume(FLT_MIN);
    }
    
    AUDIO->playEffect("sfx/click.mp3");
}

void TitleLayer::callbackRank(Ref* pSender) {
    AUDIO->playEffect("sfx/click.mp3");
}

void TitleLayer::callbackLike(Ref* pSender) {
}

void TitleLayer::callbackMore(Ref* pSender) {
    auto scene = MoreLayer::createScene();
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/click.mp3");
}


void TitleLayer::callbackShop(Ref* pSender) {
    auto scene = ShopLayer::createScene();
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/click.mp3");
}

void TitleLayer::checkDailyBonus(float dt) {
    time_t t = time(NULL);
    struct tm* t2 = localtime(&t);
    int today = t2->tm_mday;
    int last = ACCOUNT->lastlogin;
    
    if(last != today) {
        ACCOUNT->lastlogin = today;
        ACCOUNT->hint += 1;
        ACCOUNT->sync();
        
        auto popup = DailyPopup::create();
        if(popup != nullptr) {
            PopupManager::getInstance()->addPopup(popup);
        }
    }
}
