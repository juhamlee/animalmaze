//
//  CollectPopup.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 28/05/2019.
//

#include "CollectPopup.h"
#include "PopupManager.h"
#include "ClearPopup.h"

static const int FONT_SIZE_TEXT = 90;

string CollectPopup::strCollect = "";
int CollectPopup::idxAnimal = 0;

bool CollectPopup::init()
{
    Layer::init();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    sprPopup = Sprite::create("game/collect_back.png");
    if(sprPopup != nullptr) {
        sprPopup->setPosition(center);
        sprPopup->setOpacity(0);
        this->addChild(sprPopup, 0);
    
        //button
        btnOk = MenuItemImage::create("ui/btn_popup.png",
                                           "ui/btn_popup.png");
        if(btnOk != nullptr) {
            btnOk->setVisible(false);
            btnOk->setCallback(CC_CALLBACK_1(CollectPopup::callbackOk, this));
            btnOk->setPosition(360, 230);
            btnOk->setColor(Color3B(15, 120, 61));
            
            sprBtn = Sprite::create("ui/icon_o.png");
            if(sprBtn != nullptr) {
                sprBtn->setVisible(false);
                sprBtn->setPosition(btnOk->getPosition());
                sprPopup->addChild(sprBtn, Z_BUTTON_ICON);
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

void CollectPopup::onEnter() {
    Layer::onEnter();
    
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(CollectPopup::onTouchBegan, this);
    
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto sprLight = Sprite::create("game/collect_light.png");
    if(sprLight != nullptr) {
        sprLight->setPosition(360, 640);
        sprLight->setOpacity(0);
        sprLight->setScale(0.3f);
        this->addChild(sprLight, -1);
        
        sprLight->runAction(FadeIn::create(0.7f));
        sprLight->runAction(ScaleTo::create(0.7f, 3.f));
    }
    
    sprPopup->runAction(EaseIn::create(FadeIn::create(0.7f), 3.f));
    
    float delay = strCollect.size() * 0.2f + 2.2f;
    scheduleOnce(schedule_selector(CollectPopup::funcShow), 1.f);
    scheduleOnce(schedule_selector(CollectPopup::funcButton), delay);
    
    scheduleUpdate();
}

void CollectPopup::onExit() {
    EVENT_DISPATCHER->removeEventListener(listener);
    
    Layer::onExit();
}

bool CollectPopup::onTouchBegan(Touch* touch, Event* event) {
    return true;
}

void CollectPopup::update(float dt) {
    
}

void CollectPopup::callbackOk(Ref* pSender) {
    PopupManager::getInstance()->closePopup();
    
    AUDIO->playEffect("sfx/click.mp3");
    
    auto popup = ClearPopup::create();
    if(popup != nullptr) {
        PopupManager::getInstance()->addPopup(popup);
    }
}

void CollectPopup::funcShow(float dt) {
    int length = strCollect.size();
    float unit = 80.f;
    float begin = (length - 1) * unit * -0.5f;
    
    for(int i = 0; i < length; i++) {
        char buf[8];
        sprintf(buf, "%c", strCollect[i]);
        auto label = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_TEXT);
        if(label != nullptr) {
            label->setTextColor(Color4B::BLACK);
            label->setPosition(360 + begin + unit * i, 1000.f);
            label->setOpacity(0);
            sprPopup->addChild(label, Z_TEXT);
            
            auto seq = Sequence::create(DelayTime::create(0.2f * i), FadeIn::create(0.f), NULL);
            label->runAction(seq);
        }
    }
    
    char buf[64];
    sprintf(buf, "animal/big/animals-%d.png", idxAnimal);
    auto sprAnimal = Sprite::create(buf);
    if(sprAnimal != nullptr) {
        sprAnimal->setPosition(360, 640);
        sprAnimal->setOpacity(0);
        sprPopup->addChild(sprAnimal, Z_ANIMAL);
        
        auto spwn = Spawn::create(FadeIn::create(0.2f), ScaleTo::create(0.2f, 1.5f), NULL);
        auto seqScale = Sequence::create(ScaleTo::create(0.2f, 1.05f), ScaleTo::create(0.2f, 1.f), NULL);
        auto seq = Sequence::create(DelayTime::create(0.2f * length), spwn, ScaleTo::create(0.3f, 1.f), Repeat::create(seqScale, 9999), NULL);
        auto seq2 = Sequence::create(DelayTime::create(0.2f * length + 1.f), FlipX::create(true), DelayTime::create(0.2f), FlipX::create(false), NULL);
        
        sprAnimal->runAction(seq);
        sprAnimal->runAction(seq2);
    }
}

void CollectPopup::funcButton(float dt) {
    btnOk->setVisible(true);
    sprBtn->setVisible(true);
}
