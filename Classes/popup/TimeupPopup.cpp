//
//  TimeupPopup.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 24/02/2019.
//

#include "TimeupPopup.h"
#include "TitleLayer.h"
#include "PopupManager.h"

static const int FONT_SIZE_TEXT = 50;
static const Size POPUP_SIZE = Size(500, 171);

bool TimeupPopup::init()
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
        auto labText = Label::createWithTTF("TIME IS UP!", FONT_PATH, FONT_SIZE_TEXT);
        if(labText != nullptr) {
            labText->setTextColor(Color4B::BLACK);
            labText->setPosition(POPUP_SIZE.width * 0.5f, 127);
            sprPopup->addChild(labText, Z_TEXT);
        }
        
        auto sprIcon = Sprite::create("ui/icon_clock.png");
        if(sprIcon != nullptr) {
            sprIcon->setColor(Color3B::BLACK);
            sprIcon->setPosition(POPUP_SIZE.width * 0.5f, 58);
            sprPopup->addChild(sprIcon, Z_ICON);
        }
    }
    
    return true;
}

void TimeupPopup::onEnter() {
    Layer::onEnter();
    
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(TimeupPopup::onTouchBegan, this);
    
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(listener, this);
    
    scheduleUpdate();
    
    scheduleOnce(schedule_selector(TimeupPopup::close), 2.f);
}

void TimeupPopup::onExit() {
    EVENT_DISPATCHER->removeEventListener(listener);
    
    Layer::onExit();
}

bool TimeupPopup::onTouchBegan(Touch* touch, Event* event) {
    return true;
}

void TimeupPopup::update(float dt) {
    
}

void TimeupPopup::close(float dt) {
    auto scene = TitleLayer::createScene();
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/click.mp3");
}
