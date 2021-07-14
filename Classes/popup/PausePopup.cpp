//
//  PausePopup.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 24/02/2019.
//

#include "PausePopup.h"
#include "ShopPopup.h"
#include "TitleLayer.h"
#include "PopupManager.h"

static const Size POPUP_SIZE = Size(560, 150);

bool PausePopup::init()
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
        
        //button
        auto btnHome = MenuItemImage::create("ui/btn_popup.png",
                                             "ui/btn_popup.png");
        if(btnHome != nullptr) {
            btnHome->setCallback(CC_CALLBACK_1(PausePopup::callbackHome, this));
            btnHome->setPosition(140, 73);
            btnHome->setColor(Color3B(155, 204, 78));
            
            auto sprIcon = Sprite::create("ui/icon_home.png");
            if(sprIcon != nullptr) {
                sprIcon->setPosition(btnHome->getPosition());
                sprPopup->addChild(sprIcon, Z_BUTTON_ICON);
            }
        }
        
        auto btnResume = MenuItemImage::create("ui/btn_popup.png",
                                               "ui/btn_popup.png");
        if(btnResume != nullptr) {
            btnResume->setCallback(CC_CALLBACK_1(PausePopup::callbackResume, this));
            btnResume->setPosition(140 + 140, 73);
            btnResume->setColor(Color3B(104, 162, 216));
            
            auto sprIcon = Sprite::create("ui/icon_play_small.png");
            if(sprIcon != nullptr) {
                sprIcon->setPosition(btnResume->getPosition());
                sprPopup->addChild(sprIcon, Z_BUTTON_ICON);
            }
        }
        
        auto btnShop = MenuItemImage::create("ui/btn_popup.png",
                                             "ui/btn_popup.png");
        if(btnShop != nullptr) {
            btnShop->setCallback(CC_CALLBACK_1(PausePopup::callbackShop, this));
            btnShop->setPosition(140 + 140 * 2, 73);
            btnShop->setColor(Color3B(226, 178, 47));
            
            auto sprIcon = Sprite::create("ui/icon_shop.png");
            if(sprIcon != nullptr) {
                sprIcon->setPosition(btnShop->getPosition());
                sprPopup->addChild(sprIcon, Z_BUTTON_ICON);
            }
        }
        
        Menu* menu = Menu::create(btnHome, btnResume, btnShop, NULL);
        if(menu != nullptr) {
            menu->setPosition(0, 0);
            sprPopup->addChild(menu, Z_BUTTON);
        }
    }
    
    return true;
}

void PausePopup::onEnter() {
    Layer::onEnter();
    
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(PausePopup::onTouchBegan, this);
    
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(listener, this);
    
    scheduleUpdate();
}

void PausePopup::onExit() {
    EVENT_DISPATCHER->removeEventListener(listener);
    
    Layer::onExit();
}

bool PausePopup::onTouchBegan(Touch* touch, Event* event) {
    return true;
}

void PausePopup::update(float dt) {
    
}

void PausePopup::callbackHome(Ref* pSender) {
    auto scene = TitleLayer::createScene();
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/click.mp3");
}

void PausePopup::callbackResume(Ref* pSender) {
    PopupManager::getInstance()->closePopup();
    
    AUDIO->playEffect("sfx/click.mp3");
}

void PausePopup::callbackShop(Ref* pSender) {
    auto popup = ShopPopup::create();
    if(popup != nullptr) {
        PopupManager::getInstance()->addPopup(popup);
    }
    
    AUDIO->playEffect("sfx/click.mp3");
}
