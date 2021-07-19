//
//  ViewPopup.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 23/02/2019.
//

#include "ViewPopup.h"
#include "PopupManager.h"

static const int FONT_SIZE_TEXT = 50;
static const Size POPUP_SIZE = Size(690, 110);

bool ViewPopup::init()
{
    Layer::init();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    sprPopup = Scale9Sprite::create("ui/board_popup.png");
    if(sprPopup != nullptr) {
        sprPopup->setColor(Color3B::WHITE);
        sprPopup->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        sprPopup->setContentSize(POPUP_SIZE);
        sprPopup->setPosition(center.x - POPUP_SIZE.width * 0.5f, 1157);
        this->addChild(sprPopup, 0);
        
        //text
        auto labText = Label::createWithTTF("OUTSTANDING!", FONT_PATH, FONT_SIZE_TEXT);
        if(labText != nullptr) {
            labText->setTextColor(Color4B::BLACK);
            labText->setPosition(POPUP_SIZE * 0.5f);
            sprPopup->addChild(labText, Z_TEXT);
        }
        
        //button
        auto btnBack = MenuItemImage::create("ui/icon_arrow.png",
                                             "ui/icon_arrow.png");
        if(btnBack != nullptr) {
            btnBack->setCallback(CC_CALLBACK_1(ViewPopup::callbackBack, this));
            btnBack->setPosition(633, 55);
        }
        
        Menu* menu = Menu::create(btnBack, NULL);
        if(menu != nullptr) {
            menu->setPosition(0, 0);
            sprPopup->addChild(menu, Z_BUTTON);
        }
    }
    
    return true;
}

void ViewPopup::onEnter() {
    Layer::onEnter();
    
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(ViewPopup::onTouchBegan, this);
    
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(listener, this);
    
    scheduleUpdate();
}

void ViewPopup::onExit() {
    EVENT_DISPATCHER->removeEventListener(listener);
    
    Layer::onExit();
}

bool ViewPopup::onTouchBegan(Touch* touch, Event* event) {
    return true;
}

void ViewPopup::update(float dt) {
    
}

void ViewPopup::callbackBack(Ref* pSender) {
    POPUP_MANAGER->closePopup();
    
    AUDIO->playEffect("sfx/click.mp3");
}
