//
//  ShopPopup.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 24/02/2019.
//

#include "ShopPopup.h"
#include "WaitPopup.h"
#include "PopupManager.h"

static const int FONT_SIZE_TEXT = 45;
static const int FONT_SIZE_PRICE = 30;
static const Size POPUP_SIZE = Size(470, 750);

bool ShopPopup::init()
{
    Layer::init();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    vector<ShopInfo> vecInfo;
    vecInfo.push_back(ShopInfo(false, 5, "1,200"));
    vecInfo.push_back(ShopInfo(false, 25, "5,500"));
    vecInfo.push_back(ShopInfo(false, 100, "25,000"));
    vecInfo.push_back(ShopInfo(false, 250, "49,000"));
    vecInfo.push_back(ShopInfo(true, 0, "2,500"));
    
    auto layerBack = LayerColor::create(Color4B(0, 0, 0, 133), 720, 1280);
    this->addChild(layerBack, -1);
    
    sprPopup = Scale9Sprite::create("ui/board_popup.png");
    if(sprPopup != nullptr) {
        sprPopup->setColor(Color3B::WHITE);
        sprPopup->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        sprPopup->setContentSize(POPUP_SIZE);
        sprPopup->setPosition(center - POPUP_SIZE * 0.5f);
        this->addChild(sprPopup, 0);
        
        
        for(int i = 0; i < vecInfo.size(); i++) {
            auto btn = Button::create("ui/board_item.png");
            if(btn != nullptr) {
                btn->setScale9Enabled(true);
                btn->setColor(Color3B(240, 184, 56));
                btn->setContentSize(Size(379, 90));
                btn->setTag(i);
                btn->addClickEventListener(CC_CALLBACK_1(ShopPopup::callbackBuy, this));
                btn->setPosition(Vec2(235, 567 - 99 * i));
                sprPopup->addChild(btn, Z_ITEM);
                
                auto sprPriceBack = Scale9Sprite::create("ui/board_price.png");
                if(sprPriceBack != nullptr) {
                    sprPriceBack->setColor(Color3B(102, 102, 102));
                    sprPriceBack->setContentSize(Size(143, 48));
                    sprPriceBack->setPosition(Vec2(290, 45));
                    btn->addChild(sprPriceBack);
                    
                    auto labelPrice = Label::createWithTTF(vecInfo[i].price, FONT_PATH, FONT_SIZE_PRICE);
                    if(labelPrice != nullptr) {
                        labelPrice->setPosition(sprPriceBack->getContentSize() * 0.5f);
                        sprPriceBack->addChild(labelPrice);
                    }
                }
                
                if(vecInfo[i].isNoAds == true) {
                    auto labNoAds = Label::createWithTTF("AD FREE", FONT_PATH, FONT_SIZE_TEXT);
                    if(labNoAds != nullptr) {
                        labNoAds->setTextColor(Color4B::BLACK);
                        labNoAds->setPosition(107, 45);
                        btn->addChild(labNoAds);
                    }
                }
                
                if(0 < vecInfo[i].hint) {
                    auto sprHint = Sprite::create("ui/icon_hint.png");
                    if(sprHint != nullptr) {
                        sprHint->setColor(Color3B::BLACK);
                        sprHint->setScale(0.55f);
                        sprHint->setPosition(58, 45);
                        btn->addChild(sprHint);
                    }
                    
                    char buf[16];
                    sprintf(buf, "%d", vecInfo[i].hint);
                    auto labHint = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_TEXT);
                    if(labHint != nullptr) {
                        labHint->setTextColor(Color4B::BLACK);
                        labHint->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                        labHint->setPosition(92, 45);
                        btn->addChild(labHint);
                    }
                }
            }
        }
        
        auto sprShop = Sprite::create("ui/icon_shop.png");
        if(sprShop != nullptr) {
            sprShop->setColor(Color3B::BLACK);
            sprShop->setPosition(POPUP_SIZE.width * 0.5f, 702);
            sprPopup->addChild(sprShop, Z_ICON);
        }
        
        auto labText = Label::createWithTTF("MORE HINTS", FONT_PATH, FONT_SIZE_TEXT);
        if(labText != nullptr) {
            labText->setTextColor(Color4B::BLACK);
            labText->setPosition(POPUP_SIZE.width * 0.5f, 652);
            sprPopup->addChild(labText, Z_TEXT);
        }
        
        //button
        auto btnOk = MenuItemImage::create("ui/btn_popup.png",
                                           "ui/btn_popup.png");
        if(btnOk != nullptr) {
            btnOk->setCallback(CC_CALLBACK_1(ShopPopup::callbackOk, this));
            btnOk->setPosition(POPUP_SIZE.width * 0.5f, 62);
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

void ShopPopup::onEnter() {
    Layer::onEnter();
    
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(ShopPopup::onTouchBegan, this);
    
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(listener, this);
    
    scheduleUpdate();
}

void ShopPopup::onExit() {
    EVENT_DISPATCHER->removeEventListener(listener);
    
    Layer::onExit();
}

bool ShopPopup::onTouchBegan(Touch* touch, Event* event) {
    return true;
}

void ShopPopup::update(float dt) {
    
}

void ShopPopup::callbackBuy(Ref* pSender) {
    auto popup = WaitPopup::create();
    if(popup != nullptr) {
        PopupManager::getInstance()->addPopup(popup);
        
        AUDIO->playEffect("sfx/click.mp3");
    }
}

void ShopPopup::callbackOk(Ref* pSender) {
    PopupManager::getInstance()->closePopup();
    
    AUDIO->playEffect("sfx/click.mp3");
}
