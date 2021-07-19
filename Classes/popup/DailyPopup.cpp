//
//  DailyPopup.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 25/05/2019.
//

#include "DailyPopup.h"
#include "PopupManager.h"

static const int FONT_SIZE_TEXT = 50;
static const int FONT_SIZE_TEXT2 = 36;
static const Size POPUP_SIZE = Size(560, 407);

bool DailyPopup::init()
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
        sprPopup->setPosition(center.x - POPUP_SIZE.width * 0.5f, 514);
        this->addChild(sprPopup, 0);
        
        //text
        auto labText = Label::createWithTTF("Daily Bonus!", FONT_PATH, FONT_SIZE_TEXT);
        if(labText != nullptr) {
            labText->setTextColor(Color4B::BLACK);
            labText->setPosition(POPUP_SIZE.width * 0.5f, 355.f);
            sprPopup->addChild(labText, Z_TEXT);
        }
        
        //hint
        auto sprHint = Sprite::create("ui/icon_hint.png");
        if(sprHint != nullptr) {
            //sprHint->setScale(0.81f);
            sprHint->setPosition(240, 175);
            sprPopup->addChild(sprHint, Z_TEXT);
        }
        
        //hint text
        auto labHint = Label::createWithTTF("x1", FONT_PATH, FONT_SIZE_TEXT);
        if(labHint != nullptr) {
            labHint->setTextColor(Color4B::BLACK);
            labHint->setPosition(310, 175);
            sprPopup->addChild(labHint, Z_TEXT);
        }
        
        //gray
        auto drawBand = DrawNode::create();
        if(drawBand != nullptr) {
            drawBand->drawSolidRect(Vec2(0,0), Vec2(560, 61), Color4F(Color3B(213, 213, 213)));
            drawBand->setPosition(0, 241);
            sprPopup->addChild(drawBand, Z_BAND);
        }
        
        auto labBand = Label::createWithTTF("Get Free Hint!", FONT_PATH, FONT_SIZE_TEXT2);
        if(labBand != nullptr) {
            labBand->setTextColor(Color4B::WHITE);
            labBand->setPosition(POPUP_SIZE.width * 0.5f, 271);
            sprPopup->addChild(labBand, Z_BAND);
        }
        
        //button
        auto btnOk = MenuItemImage::create("ui/btn_popup.png",
                                           "ui/btn_popup.png");
        if(btnOk != nullptr) {
            btnOk->setCallback(CC_CALLBACK_1(DailyPopup::callbackOk, this));
            btnOk->setPosition(POPUP_SIZE.width * 0.5f, 63);
            btnOk->setColor(Color3B(15, 120, 61));
            
            auto sprIcon = Sprite::create("ui/icon_o.png");
            if(sprIcon != nullptr) {
                sprIcon->setPosition(btnOk->getPosition());
                sprPopup->addChild(sprIcon, Z_BUTTON_ICON);
            }
        }
        
        Menu* menu = Menu::create(btnOk, NULL);
        if(menu != nullptr) {
            menu->setPosition(0, 0);
            sprPopup->addChild(menu, Z_BUTTON);
        }
    }
    
    return true;
}

void DailyPopup::onEnter() {
    Layer::onEnter();
    
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(DailyPopup::onTouchBegan, this);
    
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(listener, this);
    
    scheduleUpdate();
}

void DailyPopup::onExit() {
    EVENT_DISPATCHER->removeEventListener(listener);
    
    Layer::onExit();
}

bool DailyPopup::onTouchBegan(Touch* touch, Event* event) {
    return true;
}

void DailyPopup::update(float dt) {
    
}

void DailyPopup::callbackOk(Ref* pSender) {
    POPUP_MANAGER->closePopup();
    
    AUDIO->playEffect("sfx/click.mp3");
}
