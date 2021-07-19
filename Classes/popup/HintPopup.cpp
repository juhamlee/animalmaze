//
//  HintPopup.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 24/02/2019.
//

#include "HintPopup.h"
#include "ShopPopup.h"
#include "PopupManager.h"

static const int FONT_SIZE_TEXT = 50;
static const int FONT_SIZE_HINT = 30;
static const Size POPUP_SIZE = Size(560, 355);

bool HintPopup::init()
{
    Layer::init();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    auto layerBack = LayerColor::create(Color4B(0, 0, 0, 133), 720, 1280);
    this->addChild(layerBack, -1);
    
    sprPopup = Scale9Sprite::create("ui/board_popup.png");
    if(sprPopup != nullptr) {
        sprPopup->setColor(Color3B::WHITE);
        sprPopup->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        sprPopup->setContentSize(POPUP_SIZE);
        sprPopup->setPosition(center - POPUP_SIZE * 0.5f);
        this->addChild(sprPopup, 0);
        
        //text
        auto labText = Label::createWithTTF("USE A HINT?", FONT_PATH, FONT_SIZE_TEXT);
        if(labText != nullptr) {
            labText->setTextColor(Color4B::BLACK);
            labText->setPosition(280, 312);
            sprPopup->addChild(labText, Z_TEXT);
        }
        
        //hint
        auto sprHint = Sprite::create("ui/icon_hint.png");
        if(sprHint != nullptr) {
            sprHint->setScale(0.81f);
            sprHint->setPosition(280, 229);
            sprPopup->addChild(sprHint, Z_HINT);
        }
        
        char buf[16];
        sprintf(buf, "%d", ACCOUNT->hint);
        auto labHint = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_HINT);
        if(labHint != nullptr) {
            labHint->setTextColor(Color4B::BLACK);
            labHint->setPosition(280, 163);
            sprPopup->addChild(labHint, Z_TEXT);
        }
        
        //button
        auto btnNo = MenuItemImage::create("ui/btn_popup.png",
                                           "ui/btn_popup.png");
        if(btnNo != nullptr) {
            btnNo->setCallback(CC_CALLBACK_1(HintPopup::callbackNo, this));
            btnNo->setPosition(137, 73);
            btnNo->setColor(Color3B(141, 24, 46));
            
            auto sprIcon = Sprite::create("ui/icon_x.png");
            if(sprIcon != nullptr) {
                sprIcon->setPosition(btnNo->getPosition());
                sprPopup->addChild(sprIcon, Z_BUTTON_ICON);
            }
        }
        
        auto btnShop = MenuItemImage::create("ui/btn_popup.png",
                                             "ui/btn_popup.png");
        if(btnShop != nullptr) {
            btnShop->setCallback(CC_CALLBACK_1(HintPopup::callbackShop, this));
            btnShop->setPosition(137 + 144, 73);
            btnShop->setColor(Color3B(226, 178, 47));
            
            auto sprIcon = Sprite::create("ui/icon_shop.png");
            if(sprIcon != nullptr) {
                sprIcon->setPosition(btnShop->getPosition());
                sprPopup->addChild(sprIcon, Z_BUTTON_ICON);
            }
        }
        
        auto btnYes = MenuItemImage::create("ui/btn_popup.png",
                                            "ui/btn_popup.png");
        if(btnYes != nullptr) {
            btnYes->setCallback(CC_CALLBACK_1(HintPopup::callbackYes, this));
            btnYes->setPosition(137 + 144 * 2, 73);
            btnYes->setColor(Color3B(15, 120, 61));
            
            auto sprIcon = Sprite::create("ui/icon_o.png");
            if(sprIcon != nullptr) {
                sprIcon->setPosition(btnYes->getPosition());
                sprPopup->addChild(sprIcon, Z_BUTTON_ICON);
            }
        }
        
        Menu* menu = Menu::create(btnNo, btnShop, btnYes, NULL);
        if(menu != nullptr) {
            menu->setPosition(0, 0);
            sprPopup->addChild(menu, Z_BUTTON);
        }
    }
    
    return true;
}

void HintPopup::onEnter() {
    Layer::onEnter();
    
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HintPopup::onTouchBegan, this);
    
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(listener, this);
    
    scheduleUpdate();
}

void HintPopup::onExit() {
    EVENT_DISPATCHER->removeEventListener(listener);
    
    Layer::onExit();
}

bool HintPopup::onTouchBegan(Touch* touch, Event* event) {
    return true;
}

void HintPopup::update(float dt) {
    
}

void HintPopup::callbackNo(Ref* pSender) {
    POPUP_MANAGER->closePopup();
    
    SimpleAudioEngine::getInstance()->playEffect("sfx/click.mp3");
}

void HintPopup::callbackShop(Ref* pSender) {
    auto popup = ShopPopup::create();
    if(popup != nullptr) {
        POPUP_MANAGER->addPopup(popup);
    }
    
    SimpleAudioEngine::getInstance()->playEffect("sfx/click.mp3");
}

void HintPopup::callbackYes(Ref* pSender) {
    POPUP_MANAGER->closePopup();
    
    EVENT_DISPATCHER->dispatchCustomEvent(E_USE_HINT);
    
    SimpleAudioEngine::getInstance()->playEffect("sfx/click.mp3");
}
