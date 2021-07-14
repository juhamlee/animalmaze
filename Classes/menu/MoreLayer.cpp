//
//  MoreLayer.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 04/03/2019.
//

#include "MoreLayer.h"
#include "ShopLayer.h"
#include "TitleLayer.h"
#include "PopupManager.h"
#include "WaitPopup.h"

#include "PluginAdMob/PluginAdMob.h"

static const int FONT_SIZE_LEVEL = 30;
static const int FONT_SIZE_TEXT = 50;

static const int TOP_Y = 1210;
static const int BTN_Y = 932;
static const int BTN_Y_OFFSET = -197;

static const Size BTN_SIZE = Size(560, 150);
static const Color3B BTN_COLOR = Color3B(154, 189, 252);

static const Color3B BTN_COLORS[4] = {
    Color3B(114, 21, 48),
    Color3B(93, 165, 171),
    Color3B(76, 136, 183),
    Color3B(226, 178, 47)
};

static const char* BTN_TEXTS[4] = {
    "PRIVACY POLICY",
    "TERMS OF SERVICE",
    "SUPPORT",
    "MORE GAMES"
};

Scene* MoreLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = MoreLayer::create();
    
    if(scene != nullptr && layer != nullptr) {
        scene->addChild(layer);
        
        PopupManager::getInstance()->initWithBaseNode(layer);
        
        return scene;
    }
    
    return nullptr;
}

bool MoreLayer::init()
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
        sprStar->setPosition(70, TOP_Y);
        this->addChild(sprStar, 1);
        
        char buf[30];
        sprintf(buf, "%d", ACCOUNT->level);
        auto labLevel = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_LEVEL);
        if(labLevel) {
            labLevel->setPosition(sprStar->getContentSize() * 0.5f);
            sprStar->addChild(labLevel);
        }
    }
    
    auto btnTitle = MenuItemImage::create("ui/btn_title.png",
                                          "ui/btn_title.png");
    if(btnTitle != nullptr) {
        btnTitle->setScaleY(-1.f);
        btnTitle->setColor(BTN_COLORS[0]);
        btnTitle->setCallback(CC_CALLBACK_1(MoreLayer::callbackTitle, this));
        btnTitle->setPosition(212, 1225);
        
        auto sprIcon = Sprite::create("ui/icon_home.png");
        if(sprIcon != nullptr) {
            sprIcon->setPosition(btnTitle->getPosition());
            this->addChild(sprIcon, 2);
        }
    }
    
    auto sprButton = Sprite::create("ui/btn_title_up.png");
    if(sprButton != nullptr) {
        sprButton->setScaleY(-1.f);
        sprButton->setColor(BTN_COLORS[1]);
        sprButton->setPosition(212 + 138 * 1, 1205);
        this->addChild(sprButton, 1);
        
        auto sprIcon = Sprite::create("ui/icon_more.png");
        if(sprIcon != nullptr) {
            sprIcon->setPosition(sprButton->getPosition() + Vec2(0, -25));
            this->addChild(sprIcon, 2);
        }
    }
    
    auto btnVolume = MenuItemImage::create("ui/btn_title.png",
                                           "ui/btn_title.png");
    if(btnVolume != nullptr) {
        btnVolume->setScaleY(-1.f);
        btnVolume->setColor(BTN_COLORS[2]);
        btnVolume->setCallback(CC_CALLBACK_1(MoreLayer::callbackVolume, this));
        btnVolume->setPosition(212 + 138 * 2, 1225);
        
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
    
    auto btnShop = MenuItemImage::create("ui/btn_title.png",
                                         "ui/btn_title.png");
    if(btnShop != nullptr) {
        btnShop->setScaleY(-1.f);
        btnShop->setColor(BTN_COLORS[3]);
        btnShop->setCallback(CC_CALLBACK_1(MoreLayer::callbackShop, this));
        btnShop->setPosition(212 + 138 * 3, 1225);
        
        auto sprIcon = Sprite::create("ui/icon_shop.png");
        if(sprIcon != nullptr) {
            sprIcon->setPosition(btnShop->getPosition());
            this->addChild(sprIcon, 2);
        }
    }
    
    Menu* menu = Menu::create(btnTitle, btnShop, btnVolume, NULL);
    if(menu) {
        menu->setPosition(0, 0);
        this->addChild(menu, 1);
    }
    
    for(int i = 0; i < 4; i++) {
        auto btn = Button::create("ui/board_item.png");
        if(btn != nullptr) {
            btn->setScale9Enabled(true);
            btn->setColor(BTN_COLOR);
            btn->setContentSize(BTN_SIZE);
            btn->setTag(i);
            btn->addClickEventListener(CC_CALLBACK_1(MoreLayer::callbackLink, this));
            btn->setPosition(Vec2(center.x, BTN_Y + BTN_Y_OFFSET * i));
            this->addChild(btn, 0);
            
            auto labText = Label::createWithTTF(BTN_TEXTS[i], FONT_PATH, FONT_SIZE_TEXT);
            if(labText) {
                labText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                labText->setPosition(btn->getContentSize() * 0.5f);
                btn->addChild(labText);
            }
        }
    }
    
    return true;
}

void MoreLayer::onEnter() {
    Layer::onEnter();
    
    keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = CC_CALLBACK_2(MoreLayer::onKeyReleased, this);
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(keyListener, this);
    
    if(sdkbox::PluginAdMob::isAvailable("home"))
        sdkbox::PluginAdMob::show("home");
    else
        sdkbox::PluginAdMob::cache("home");
}

void MoreLayer::onExit() {
    EVENT_DISPATCHER->removeEventListener(keyListener);
    
    Layer::onExit();
}

void MoreLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event) {
    if(keycode == EventKeyboard::KeyCode::KEY_BACK) {
        auto scene = TitleLayer::createScene();
        auto director = Director::getInstance();
        director->replaceScene(scene);
        
        AUDIO->playEffect("sfx/click.mp3");
    }
}

void MoreLayer::callbackTitle(Ref* pSender) {
    auto scene = TitleLayer::createScene();
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/click.mp3");
}

void MoreLayer::callbackVolume(Ref* pSender) {
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

void MoreLayer::callbackShop(Ref* pSender) {
    auto scene = ShopLayer::createScene();
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/click.mp3");
}

void MoreLayer::callbackLink(Ref* pSender) {
    AUDIO->playEffect("sfx/click.mp3");
}
