//
//  StageSelectLayer.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 24/02/2019.
//

#include "StageSelectLayer.h"
#include "TitleLayer.h"
#include "MazeLayer.h"
#include "MazeAlphaLayer.h"
#include "WaitPopup.h"
#include "PopupManager.h"

#include "PluginAdMob/PluginAdMob.h"
#include "PluginIAP/PluginIAP.h"

static const int FONT_SIZE_LEVEL = 30;
static const int FONT_SIZE_TEXT = 50;
static const int FONT_SIZE_STAGE = 65;

static const int TOP_Y = 1210;
static const Size SCROLL_SIZE = Size(674, 1003);
static const Size BTN_SIZE = Size(115, 115);

static const char* TITLE_TEXTS[] = {
    "CLASSIC",
    "ICE FLOOR",
    "HURRY UP",
    "DARKNESS",
};

static const Color3B BTN_COLORS[] = {
    Color3B(132, 252, 215),
    Color3B(146, 192, 254),
    Color3B(255, 166, 195),
    Color3B(179, 53, 249),
    Color3B(76, 76, 76)
};

static const Color3B LOCK_COLOR = Color3B(76, 76, 76);


Scene* StageSelectLayer::createScene(int category)
{
    auto scene = Scene::create();
    auto layer = StageSelectLayer::create();
    
    if(scene != nullptr && layer != nullptr) {
        layer->category = category;
        
        POPUP_MANAGER->initWithBaseNode(layer);
        ScreenLog::getInstance()->attachToScene( scene );
        
        scene->addChild(layer);
        return scene;
    }
    
    return nullptr;
}

bool StageSelectLayer::init()
{
    Layer::init();
    
    return true;
}

void StageSelectLayer::onEnter() {
    Layer::onEnter();
    
    keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = CC_CALLBACK_2(StageSelectLayer::onKeyReleased, this);
    
    customListener = EventListenerCustom::create(E_RELOAD_SCENE, CC_CALLBACK_1(StageSelectLayer::onReload, this));
    
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(keyListener, this);
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(customListener, this);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    auto bg = LayerColor::create(Color4B::BLACK, visibleSize.width, visibleSize.height);
    if(bg != nullptr) {
        bg->setPosition(center);
        this->addChild(bg, 0);
    }
    
    auto labTitle = Label::createWithTTF(TITLE_TEXTS[category], FONT_PATH, FONT_SIZE_TEXT);
    if(labTitle) {
        labTitle->setPosition(center.x, TOP_Y);
        this->addChild(labTitle, 1);
    }
    
    auto btnTitle = MenuItemImage::create("ui/icon_home.png",
                                          "ui/icon_home.png",
                                          "ui/icon_home.png");
    if(btnTitle != nullptr) {
        btnTitle->setCallback(CC_CALLBACK_1(StageSelectLayer::callbackTitle, this));
        btnTitle->setPosition(70, TOP_Y);
    }
    
    auto btnUnlock = MenuItemImage::create("ui/btn_small.png",
                                           "ui/btn_small.png");
    if(btnUnlock != nullptr) {
        btnUnlock->setScaleY(-1.f);
        btnUnlock->setOpacity(128);
        btnUnlock->setCallback(CC_CALLBACK_1(StageSelectLayer::callbackUnlock, this));
        btnUnlock->setPosition(549, 1220);
        
        auto sprIcon = Sprite::create("ui/icon_lock.png");
        if(sprIcon != nullptr) {
            sprIcon->setPosition(btnUnlock->getPosition() + Vec2(0, -8));
            this->addChild(sprIcon, 3);
        }
        
        if(ACCOUNT->isUnlockAll(category)) {
            btnUnlock->setVisible(false);
            if(sprIcon != nullptr)
                sprIcon->setVisible(false);
        }
    }
    
    Menu* menu = Menu::create(btnTitle, btnUnlock, NULL);
    if(menu) {
        menu->setPosition(0, 0);
        this->addChild(menu, 1);
    }
    
    auto sprStar = Sprite::create("ui/icon_star.png");
    if(sprStar != nullptr) {
        sprStar->setPosition(650, TOP_Y);
        this->addChild(sprStar, 1);
        
        char buf[30];
        sprintf(buf, "%d", ACCOUNT->level);
        auto labLevel = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_LEVEL);
        if(labLevel) {
            labLevel->setPosition(sprStar->getContentSize() * 0.5f);
            sprStar->addChild(labLevel);
        }
    }
    
    int stagenum = STAGE_MANAGER->stageNumbers[category];
    Size sizeInner(674, MAX(SCROLL_SIZE.height, ((stagenum - 1)/5 + 1) * 135));
    
    auto scrollview = ui::ScrollView::create();
    scrollview->setClippingEnabled(true);
    scrollview->setContentSize(SCROLL_SIZE);
    scrollview->setDirection(ui::ScrollView::Direction::VERTICAL);
    scrollview->getInnerContainer()->setLayoutType(ui::Layout::Type::ABSOLUTE);
    scrollview->setInnerContainerSize(sizeInner);
    scrollview->setPosition(Vec2(21, 108));
    this->addChild(scrollview);
    
    
    for(int i = 0; i < stagenum; i++) {
        if(ACCOUNT->vecStageState[category][i] == Account::LOCK) {
            auto button = ui::Button::create("ui/board_item.png");
            button->setScale9Enabled(true);
            button->setContentSize(BTN_SIZE);
            button->setColor(LOCK_COLOR);
            button->setTag(i + 1);
            button->setPosition(Vec2(i%5 * 135, sizeInner.height - (1 + i/5) * 135) + (BTN_SIZE + Size(10, 10))  * 0.5f);
            button->addClickEventListener(CC_CALLBACK_1(StageSelectLayer::callbackSelect, this));
            button->setEnabled(false);
            scrollview->addChild(button);
            
            auto sprLock = Sprite::create("ui/icon_lock.png");
            if(sprLock != nullptr) {
                sprLock->setPosition(BTN_SIZE * 0.5f);
                button->addChild(sprLock);
            }
        }
        else if(ACCOUNT->vecStageState[category][i] == Account::OPEN) {
            auto button = ui::Button::create("ui/board_item.png");
            button->setScale9Enabled(true);
            button->setContentSize(BTN_SIZE);
            button->setColor(BTN_COLORS[category]);
            button->setTag(i + 1);
            button->setPosition(Vec2(i%5 * 135, sizeInner.height - (1 + i/5) * 135) + (BTN_SIZE + Size(10, 10))  * 0.5f);
            button->addClickEventListener(CC_CALLBACK_1(StageSelectLayer::callbackSelect, this));
            scrollview->addChild(button);
            
            char buf[16];
            sprintf(buf, "%d", i + 1);
            auto labStage = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_STAGE);
            if(labStage != nullptr) {
                labStage->setPosition(BTN_SIZE * 0.5f);
                button->addChild(labStage);
            }
            
            auto sprCover = Scale9Sprite::create("ui/board_item.png");
            if(sprCover) {
                sprCover->setColor(Color3B(59, 59, 59));
                sprCover->setOpacity(150);
                sprCover->setContentSize(BTN_SIZE);
                sprCover->setPosition(BTN_SIZE * 0.5f);
                button->addChild(sprCover);
            }
        }
        else {
            auto button = ui::Button::create("ui/board_item.png");
            button->setScale9Enabled(true);
            button->setContentSize(BTN_SIZE);
            button->setColor(BTN_COLORS[category]);
            button->setTag(i + 1);
            button->setPosition(Vec2(i%5 * 135, sizeInner.height - (1 + i/5) * 135) + (BTN_SIZE + Size(10, 10))  * 0.5f);
            button->addClickEventListener(CC_CALLBACK_1(StageSelectLayer::callbackSelect, this));
            scrollview->addChild(button);
            
            char buf[16];
            sprintf(buf, "%d", i + 1);
            auto labStage = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_STAGE);
            if(labStage != nullptr) {
                labStage->setPosition(BTN_SIZE * 0.5f);
                button->addChild(labStage);
            }
        }
    }
    
    if(ACCOUNT->isNoAds == false) {
        if(sdkbox::PluginAdMob::isAvailable("home"))
            sdkbox::PluginAdMob::show("home");
        else
            sdkbox::PluginAdMob::cache("home");
    }
    else
        sdkbox::PluginAdMob::hide("home");
}

void StageSelectLayer::callbackTitle(Ref* pSender) {
    auto scene = TitleLayer::createScene();
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/click.mp3");
}

void StageSelectLayer::callbackUnlock(Ref* pSender) {
    auto popup = WaitPopup::create();
    if(popup != nullptr) {
        POPUP_MANAGER->addPopup(popup, true);
    }
    
    AUDIO->playEffect("sfx/click.mp3");
    
    if(category == 0)
        sdkbox::IAP::purchase(SKU_UNLOCK1);
    else if(category == 1)
        sdkbox::IAP::purchase(SKU_UNLOCK2);
    else if(category == 2)
        sdkbox::IAP::purchase(SKU_UNLOCK3);
    else if(category == 3)
        sdkbox::IAP::purchase(SKU_UNLOCK4);
}

void StageSelectLayer::callbackSelect(Ref* pSender) {
    Node* node = (Node*)pSender;
    int idx = node->getTag();
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

void StageSelectLayer::onExit() {
    EVENT_DISPATCHER->removeEventListener(keyListener);
    EVENT_DISPATCHER->removeEventListener(customListener);
    
    Layer::onExit();
}

void StageSelectLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event) {
    if(keycode == EventKeyboard::KeyCode::KEY_BACK) {
        auto scene = TitleLayer::createScene();
        auto director = Director::getInstance();
        director->replaceScene(scene);
        
        AUDIO->playEffect("sfx/click.mp3");
    }
}

void StageSelectLayer::onReload(EventCustom *event) {
    auto scene = StageSelectLayer::createScene(this->category);
    auto director = Director::getInstance();
    director->replaceScene(scene);
}
