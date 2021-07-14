//
//  ExitPopup.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 23/02/2019.
//

#include "ExitPopup.h"
#include "PopupManager.h"

static const int FONT_SIZE_TEXT = 50;
static const Size POPUP_SIZE = Size(560, 240);

bool ExitPopup::init()
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
        auto labText = Label::createWithTTF("WANT TO EXIT?", FONT_PATH, FONT_SIZE_TEXT);
        if(labText != nullptr) {
            labText->setTextColor(Color4B::BLACK);
            labText->setPosition(POPUP_SIZE.width * 0.5f, 175);
            sprPopup->addChild(labText, Z_TEXT);
        }
        
        //button
        auto btnYes = MenuItemImage::create("ui/btn_popup.png",
                                            "ui/btn_popup.png");
        if(btnYes != nullptr) {
            btnYes->setCallback(CC_CALLBACK_1(ExitPopup::callbackYes, this));
            btnYes->setPosition(163, 73);
            btnYes->setColor(Color3B(15, 120, 61));
            
            auto sprIcon = Sprite::create("ui/icon_o.png");
            if(sprIcon != nullptr) {
                sprIcon->setPosition(btnYes->getPosition());
                sprPopup->addChild(sprIcon, Z_BUTTON_ICON);
            }
        }
        
        auto btnNo = MenuItemImage::create("ui/btn_popup.png",
                                           "ui/btn_popup.png");
        if(btnNo != nullptr) {
            btnNo->setCallback(CC_CALLBACK_1(ExitPopup::callbackNo, this));
            btnNo->setPosition(163 + 247, 73);
            btnNo->setColor(Color3B(141, 24, 46));
            
            auto sprIcon = Sprite::create("ui/icon_x.png");
            if(sprIcon != nullptr) {
                sprIcon->setPosition(btnNo->getPosition());
                sprPopup->addChild(sprIcon, Z_BUTTON_ICON);
            }
        }
        
        Menu* menu = Menu::create(btnYes, btnNo, NULL);
        if(menu != nullptr) {
            menu->setPosition(0, 0);
            sprPopup->addChild(menu, Z_BUTTON);
        }
    }
    
    return true;
}

void ExitPopup::onEnter() {
    Layer::onEnter();
    
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(ExitPopup::onTouchBegan, this);
    
    keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = CC_CALLBACK_2(ExitPopup::onKeyReleased, this);
    
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(listener, this);
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(keyListener, this);
    
    scheduleUpdate();
}

void ExitPopup::onExit() {
    EVENT_DISPATCHER->removeEventListener(listener);
    EVENT_DISPATCHER->removeEventListener(keyListener);
    
    Layer::onExit();
}

bool ExitPopup::onTouchBegan(Touch* touch, Event* event) {
    return true;
}

void ExitPopup::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event) {
    if(keycode == EventKeyboard::KeyCode::KEY_BACK) {
        PopupManager::getInstance()->closePopup();
        
        AUDIO->playEffect("sfx/click.mp3");
    }
}

void ExitPopup::callbackYes(Ref* pSender) {
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    AUDIO->playEffect("sfx/click.mp3");
}

void ExitPopup::callbackNo(Ref* pSender) {
    PopupManager::getInstance()->closePopup();
    
    AUDIO->playEffect("sfx/click.mp3");
}
