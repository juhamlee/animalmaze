//
//  WaitPopup.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 23/02/2019.
//

#include "WaitPopup.h"
#include "PopupManager.h"

static const int FONT_SIZE_TEXT = 50;
static const Size POPUP_SIZE = Size(500, 171);

bool WaitPopup::init()
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
        auto labText = Label::createWithTTF("PLEASE WAIT...", FONT_PATH, FONT_SIZE_TEXT);
        if(labText != nullptr) {
            labText->setTextColor(Color4B::BLACK);
            labText->setPosition(POPUP_SIZE * 0.5f);
            sprPopup->addChild(labText, Z_TEXT);
        }
    }
    
    return true;
}

void WaitPopup::onEnter() {
    Layer::onEnter();
    
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(WaitPopup::onTouchBegan, this);
    
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(listener, this);
    
    scheduleUpdate();
    
    scheduleOnce(schedule_selector(WaitPopup::close), 0.5f);
}

void WaitPopup::onExit() {
    EVENT_DISPATCHER->removeEventListener(listener);
    
    Layer::onExit();
}

bool WaitPopup::onTouchBegan(Touch* touch, Event* event) {
    return true;
}

void WaitPopup::update(float dt) {
    
}

void WaitPopup::close(float dt) {
    PopupManager::getInstance()->closePopup();
    
    AUDIO->playEffect("sfx/click.mp3");
}
