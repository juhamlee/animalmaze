//
//  ClearPopup.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 23/02/2019.
//

#include "ClearPopup.h"
#include "TitleLayer.h"
#include "MazeLayer.h"
#include "MazeAlphaLayer.h"
#include "ViewPopup.h"
#include "PopupManager.h"

#include "PluginAdMob/PluginAdMob.h"

static const int FONT_SIZE_LEVEL = 30;
static const int FONT_SIZE_TEXT = 70;
static const Size POPUP_SIZE = Size(569, 325);

bool ClearPopup::init()
{
    Layer::init();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    auto layerBack = LayerColor::create(Color4B(0, 0, 0, 200), 720, 1280);
    this->addChild(layerBack, -1);
    
    auto nodePopup = Node::create();
    if(nodePopup != nullptr) {
        nodePopup->setPosition(0, 720);
        nodePopup->runAction(EaseBackOut::create(MoveTo::create(0.3f, Vec2::ZERO)));
        this->addChild(nodePopup);
        
        auto sprCrown = Sprite::create("ui/icon_crown.png");
        if(sprCrown != nullptr) {
            sprCrown->setPosition(360, 912);
            nodePopup->addChild(sprCrown, Z_TEXT);
        }
        
        auto labText = Label::createWithTTF("STAGE CLEAR!", FONT_PATH, FONT_SIZE_TEXT);
        if(labText != nullptr) {
            labText->setTextColor(Color4B::WHITE);
            labText->setPosition(360, 822);
            labText->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.3f, 1.2f), ScaleTo::create(0.3f, 1.f), NULL)));
            nodePopup->addChild(labText, Z_TEXT);
        }
        
        //button
        auto btnHome = MenuItemImage::create("ui/btn_clear.png",
                                             "ui/btn_clear.png");
        if(btnHome != nullptr) {
            btnHome->setCallback(CC_CALLBACK_1(ClearPopup::callbackHome, this));
            btnHome->setPosition(188, 605);
            btnHome->setColor(Color3B(105, 105, 105));
            
            auto sprIcon = Sprite::create("ui/icon_home.png");
            if(sprIcon != nullptr) {
                sprIcon->setPosition(btnHome->getPosition());
                nodePopup->addChild(sprIcon, Z_BUTTON_ICON);
            }
        }
        
        auto btnRetry = MenuItemImage::create("ui/btn_clear.png",
                                             "ui/btn_clear.png");
        if(btnRetry != nullptr) {
            btnRetry->setCallback(CC_CALLBACK_1(ClearPopup::callbackRetry, this));
            btnRetry->setPosition(188 + 168, 605);
            btnRetry->setColor(Color3B(105, 105, 105));
            
            auto sprIcon = Sprite::create("ui/icon_retry.png");
            if(sprIcon != nullptr) {
                sprIcon->setPosition(btnRetry->getPosition());
                nodePopup->addChild(sprIcon, Z_BUTTON_ICON);
            }
        }
        
        auto btnNext = MenuItemImage::create("ui/btn_clear.png",
                                             "ui/btn_clear.png");
        if(btnNext != nullptr) {
            btnNext->setCallback(CC_CALLBACK_1(ClearPopup::callbackNext, this));
            btnNext->setPosition(188 + 168 * 2, 605);
            btnNext->setColor(Color3B(89, 140, 184));
            
            auto sprIcon = Sprite::create("ui/icon_next.png");
            if(sprIcon != nullptr) {
                sprIcon->setPosition(btnNext->getPosition());
                nodePopup->addChild(sprIcon, Z_BUTTON_ICON);
            }
        }
        
        Menu* menu = Menu::create(btnHome, btnRetry, btnNext, NULL);
        if(menu != nullptr) {
            menu->setPosition(0, 0);
            nodePopup->addChild(menu, Z_BUTTON);
        }
    }
    
    /*
    //exp
    Node* nodeExp = Node::create();
    if(nodeExp != nullptr) {
        nodeExp->setPosition(0, 179);
        sprPopup->addChild(nodeExp, Z_EXP);
        
        auto sprExpBack = Scale9Sprite::create("ui/board_gauge.png");
        if(sprExpBack != nullptr) {
            sprExpBack->setColor(Color3B(133, 133, 133));
            sprExpBack->setContentSize(Size(507, 42));
            sprExpBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            sprExpBack->setPosition(30.5f, 0);
            nodeExp->addChild(sprExpBack);
        }
        
        auto sprExpFront = Scale9Sprite::create("ui/board_gauge.png");
        if(sprExpFront != nullptr) {
            sprExpFront->setColor(Color3B(253, 234, 70));
            sprExpFront->setContentSize(Size(507 * ACCOUNT->exp, 42));
            sprExpFront->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            sprExpFront->setPosition(30.5f, 0);
            nodeExp->addChild(sprExpFront);
        }
        
        auto sprStar = Sprite::create("ui/icon_star.png");
        if(sprStar != nullptr) {
            sprStar->setPosition(60, 5);
            nodeExp->addChild(sprStar);
            
            char buf[8];
            sprintf(buf, "%d", ACCOUNT->level);
            labLevel = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_LEVEL);
            if(labLevel != nullptr) {
                labLevel->setPosition(sprStar->getContentSize() * 0.5f);
                sprStar->addChild(labLevel);
            }
        }
    }
    
    //button
    auto btnHome = MenuItemImage::create("ui/btn_popup.png",
                                         "ui/btn_popup.png");
    if(btnHome != nullptr) {
        btnHome->setCallback(CC_CALLBACK_1(ClearPopup::callbackHome, this));
        btnHome->setPosition(144, 56);
        btnHome->setColor(Color3B(144, 197, 69));
        
        auto sprIcon = Sprite::create("ui/icon_home.png");
        if(sprIcon != nullptr) {
            sprIcon->setPosition(btnHome->getPosition());
            sprPopup->addChild(sprIcon, Z_BUTTON_ICON);
        }
    }
    
    auto btnNext = MenuItemImage::create("ui/icon_play_small.png",
                                         "ui/icon_play_small.png");
    if(btnNext != nullptr) {
        btnNext->setCallback(CC_CALLBACK_1(ClearPopup::callbackNext, this));
        btnNext->setPosition(144 + 134, 72);
        btnNext->setScale(82.f/46.f);
        btnNext->setColor(Color3B(104, 162, 216));
    }
    
    auto btnLike = MenuItemImage::create("ui/btn_popup.png",
                                         "ui/btn_popup.png");
    if(btnLike != nullptr) {
        btnLike->setCallback(CC_CALLBACK_1(ClearPopup::callbackLike, this));
        btnLike->setPosition(144 + 134 * 2, 56);
        btnLike->setColor(Color3B(131, 23, 41));
        
        auto sprIcon = Sprite::create("ui/icon_like.png");
        if(sprIcon != nullptr) {
            sprIcon->setPosition(btnLike->getPosition());
            sprPopup->addChild(sprIcon, Z_BUTTON_ICON);
        }
    }
    
    auto btnView = MenuItemImage::create("ui/icon_eye.png",
                                         "ui/icon_eye.png");
    if(btnView != nullptr) {
        btnView->setCallback(CC_CALLBACK_1(ClearPopup::callbackView, this));
        btnView->setPosition(510, 268);
    }
    
    Menu* menu = Menu::create(btnHome, btnNext, btnLike, btnView, NULL);
    if(menu != nullptr) {
        menu->setPosition(0, 0);
        sprPopup->addChild(menu, Z_BUTTON);
    }
    
    auto labText = Label::createWithTTF("INSPIRING!", FONT_PATH, FONT_SIZE_TEXT);
    if(labText != nullptr) {
        labText->setTextColor(Color4B::BLACK);
        labText->setPosition(284, 268);
        sprPopup->addChild(labText, Z_TEXT);
    }
     */
    
    for(int i = 0; i < 4; i++)
        emitter[i] = nullptr;
    
    return true;
}

void ClearPopup::onEnter() {
    Layer::onEnter();
    
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(ClearPopup::onTouchBegan, this);
    
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(listener, this);
    
    scheduleUpdate();
}

void ClearPopup::onExit() {
    EVENT_DISPATCHER->removeEventListener(listener);
    
    Layer::onExit();
}

bool ClearPopup::onTouchBegan(Touch* touch, Event* event) {
    return true;
}

void ClearPopup::update(float dt) {
    static const char* particlefiles[4] = {
        "game/particle_r.plist",
        "game/particle_g.plist",
        "game/particle_y.plist",
        "game/particle_b.plist"
    };
    
    delay += dt;
    for(int i = 0; i < 4; i++) {
        if(0.3f + i * 0.15f < delay && emitter[i] == nullptr) {
            emitter[i] = ParticleSystemQuad::create(particlefiles[i]);
            emitter[i]->setPosition(360, 1280);
            addChild(emitter[i], 999);
        }
    }
}

void ClearPopup::callbackHome(Ref* pSender) {
    auto scene = TitleLayer::createScene();
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/click.mp3");
}

void ClearPopup::callbackNext(Ref* pSender) {
    if(STAGE_MANAGER->isLastStage)
        return;
    
    int category = STAGE_MANAGER->nextStage.category;
    int idx = STAGE_MANAGER->nextStage.index;
    STAGE_MANAGER->setCurrentStage(category, idx);
    
    if(category == StageManager::CATEGORY_CLASSIC) {
        if(STAGE_MANAGER->currentStage.type == StageManager::TYPE_ALPHA) {
            auto scene = MazeAlphaLayer::createScene();
            auto director = Director::getInstance();
            director->replaceScene(scene);
        }
        else {
            auto scene = MazeLayer::createScene();
            auto director = Director::getInstance();
            director->replaceScene(scene);
        }
    }
    else {
        auto scene = MazeLayer::createScene();
        auto director = Director::getInstance();
        director->replaceScene(scene);
    }
    
    AUDIO->playEffect("sfx/click.mp3");
}

void ClearPopup::callbackLike(Ref* pSender) {
    AUDIO->playEffect("sfx/click.mp3");
}

void ClearPopup::callbackView(Ref* pSender) {
    auto popup = ViewPopup::create();
    if(popup != nullptr) {
        PopupManager::getInstance()->addPopup(popup);
    }
    
    AUDIO->playEffect("sfx/click.mp3");
}

void ClearPopup::callbackRetry(Ref* pSender) {
    if(STAGE_MANAGER->currentStage.type == StageManager::TYPE_ALPHA) {
        auto scene = MazeAlphaLayer::createScene();
        auto director = Director::getInstance();
        director->replaceScene(scene);
    }
    else {
        auto scene = MazeLayer::createScene();
        auto director = Director::getInstance();
        director->replaceScene(scene);
    }
    
    AUDIO->playEffect("sfx/click.mp3");
}
