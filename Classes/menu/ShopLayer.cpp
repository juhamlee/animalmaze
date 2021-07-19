//
//  ShopLayer.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 05/01/2019.
//

#include "ShopLayer.h"
#include "TitleLayer.h"
#include "MoreLayer.h"
#include "PopupManager.h"
#include "WaitPopup.h"

#include "PluginAdMob/PluginAdMob.h"
#include "PluginIAP/PluginIAP.h"

static const int FONT_SIZE_LEVEL = 30;
static const int FONT_SIZE_HINT = 30;
static const int FONT_SIZE_PRICE = 30;
static const int FONT_SIZE_TEXT = 50;

static const int TOP_Y = 1210;
static const int BTN_Y = 1000;
static const int BTN_Y_OFFSET = -168;
static const int BTN_BADGE_X = 70;
static const int BTN_ICON_X = 170;
static const int BTN_PLUS_X = 234;
static const int BTN_ICON2_X = 312;
static const int BTN_PRICE_X = 457;
static const int RESTORE_Y = 198;

static const Size BTN_SIZE = Size(569, 150);
static const Size RESTORE_SIZE = Size(569, 79);
static const Size PRICE_SIZE = Size(143, 48);

static const Color3B BTN_COLOR = Color3B(154, 189, 252);
static const Color3B RESTORE_COLOR_ON = Color3B(53, 102, 192);
static const Color3B RESTORE_COLOR_OFF = Color3B(78, 78, 78);
static const Color3B PRICE_COLOR = Color3B(102, 102, 102);

static const Color3B BTN_COLORS[4] = {
    Color3B(114, 21, 48),
    Color3B(93, 165, 171),
    Color3B(76, 136, 183),
    Color3B(226, 178, 47)
};

Scene* ShopLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = ShopLayer::create();
    
    if(scene != nullptr && layer != nullptr) {
        scene->addChild(layer);
        
        POPUP_MANAGER->initWithBaseNode(layer);
        ScreenLog::getInstance()->attachToScene( scene );
        
        return scene;
    }
    
    return nullptr;
}

bool ShopLayer::init()
{
    Layer::init();
 
    vector<ShopInfo> vecInfo;
    vecInfo.push_back(ShopInfo(NORMAL, true, 0, "2,500"));
    vecInfo.push_back(ShopInfo(BESTSELLER, false, 5, "1,200"));
    vecInfo.push_back(ShopInfo(NORMAL, false, 25, "5,500"));
    vecInfo.push_back(ShopInfo(BESTVALUE, true, 100, "25,000"));
    vecInfo.push_back(ShopInfo(NORMAL, true, 250, "49,000"));
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    auto bg = LayerColor::create(Color4B::BLACK, visibleSize.width, visibleSize.height);
    if(bg != nullptr) {
        bg->setPosition(center);
        this->addChild(bg, 0);
    }
    
    auto sprStar = Sprite::create("ui/icon_star.png");
    if(sprStar != nullptr) {
        sprStar->setPosition(70, TOP_Y);
        this->addChild(sprStar, 1);
        
        char buf[30];
        sprintf(buf, "%d", ACCOUNT->level);
        auto labLevel = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_LEVEL);
        if(labLevel) {
            labLevel->setPosition(sprStar->getContentSize() * 0.5f);
            sprStar->addChild(labLevel);
        }
    }
    
    auto btnTitle = MenuItemImage::create("ui/btn_title.png",
                                          "ui/btn_title.png");
    if(btnTitle != nullptr) {
        btnTitle->setScaleY(-1.f);
        btnTitle->setColor(BTN_COLORS[0]);
        btnTitle->setCallback(CC_CALLBACK_1(ShopLayer::callbackTitle, this));
        btnTitle->setPosition(212, 1225);
        
        auto sprIcon = Sprite::create("ui/icon_home.png");
        if(sprIcon != nullptr) {
            sprIcon->setPosition(btnTitle->getPosition());
            this->addChild(sprIcon, 2);
        }
    }
    
    auto btnMore = MenuItemImage::create("ui/btn_title.png",
                                         "ui/btn_title.png");
    if(btnMore != nullptr) {
        btnMore->setScaleY(-1.f);
        btnMore->setColor(BTN_COLORS[1]);
        btnMore->setCallback(CC_CALLBACK_1(ShopLayer::callbackMore, this));
        btnMore->setPosition(212 + 138, 1225);
        
        auto sprIcon = Sprite::create("ui/icon_more.png");
        if(sprIcon != nullptr) {
            sprIcon->setPosition(btnMore->getPosition());
            this->addChild(sprIcon, 2);
        }
    }
    
    auto btnVolume = MenuItemImage::create("ui/btn_title.png",
                                           "ui/btn_title.png");
    if(btnVolume != nullptr) {
        btnVolume->setScaleY(-1.f);
        btnVolume->setColor(BTN_COLORS[2]);
        btnVolume->setCallback(CC_CALLBACK_1(ShopLayer::callbackVolume, this));
        btnVolume->setPosition(212 + 138 * 2, 1225);
        
        if(ACCOUNT->isMute == false) {
            sprVolume = Sprite::create("ui/icon_volume.png");
            if(sprVolume != nullptr) {
                sprVolume->setPosition(btnVolume->getPosition());
                this->addChild(sprVolume, 2);
            }
        }
        else {
            sprVolume = Sprite::create("ui/icon_volume_off.png");
            if(sprVolume != nullptr) {
                sprVolume->setPosition(btnVolume->getPosition());
                this->addChild(sprVolume, 2);
            }
        }
    }
    
    auto sprButton = Sprite::create("ui/btn_title_up.png");
    if(sprButton != nullptr) {
        sprButton->setScaleY(-1.f);
        sprButton->setColor(BTN_COLORS[3]);
        sprButton->setPosition(212 + 138 * 3, 1205);
        this->addChild(sprButton, 1);
        
        auto sprIcon = Sprite::create("ui/icon_shop.png");
        if(sprIcon != nullptr) {
            sprIcon->setPosition(sprButton->getPosition() + Vec2(0, -25));
            this->addChild(sprIcon, 2);
        }
    }
    
    Menu* menu = Menu::create(btnTitle, btnMore, btnVolume, NULL);
    if(menu) {
        menu->setPosition(0, 0);
        this->addChild(menu, 1);
    }
    
    for(int i = 0; i < vecInfo.size(); i++) {
        auto btn = Button::create("ui/board_item.png");
        if(btn != nullptr) {
            btn->setScale9Enabled(true);
            btn->setColor(BTN_COLOR);
            btn->setContentSize(BTN_SIZE);
            btn->setTag(i);
            btn->addClickEventListener(CC_CALLBACK_1(ShopLayer::callbackBuy, this));
            btn->setPosition(Vec2(center.x, BTN_Y + BTN_Y_OFFSET * i));
            this->addChild(btn, 0);
            
            if(i == 0 && ACCOUNT->isNoAds == true)
                btn->setEnabled(false);
            
            int offset_y = BTN_SIZE.height * 0.5f;
            
            //badge
            if(vecInfo[i].type == BESTSELLER) {
                auto sprBadge = Sprite::create("ui/icon_bestseller.png");
                if(sprBadge != nullptr) {
                    sprBadge->setPosition(BTN_BADGE_X, offset_y);
                    btn->addChild(sprBadge);
                }
            }
            else if(vecInfo[i].type == BESTVALUE) {
                auto sprBadge = Sprite::create("ui/icon_bestvalue.png");
                if(sprBadge != nullptr) {
                    sprBadge->setPosition(BTN_BADGE_X, offset_y + 15);
                    btn->addChild(sprBadge);
                }
            }
            
            //icon
            if(vecInfo[i].isNoAds && vecInfo[i].hints == 0) {
                auto sprIcon = Sprite::create("ui/icon_ban.png");
                if(sprIcon != nullptr) {
                    sprIcon->setPosition(BTN_ICON_X, offset_y);
                    btn->addChild(sprIcon);
                }
            }
            else {
                auto sprIcon = Sprite::create("ui/icon_hint.png");
                if(sprIcon != nullptr) {
                    sprIcon->setPosition(BTN_ICON_X, offset_y);
                    btn->addChild(sprIcon);
                    
                    char buf[30];
                    sprintf(buf, "%d", vecInfo[i].hints);
                    auto labCount = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_HINT);
                    if(labCount) {
                        labCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                        labCount->setPosition(sprIcon->getContentSize() * 0.5f);
                        sprIcon->addChild(labCount);
                    }
                }
            }
            
            //plus icons
            if(vecInfo[i].isNoAds && 0 < vecInfo[i].hints) {
                auto sprPlus = Sprite::create("ui/icon_plus.png");
                if(sprPlus != nullptr) {
                    sprPlus->setPosition(BTN_PLUS_X, offset_y);
                    btn->addChild(sprPlus);
                }
                
                auto sprIcon = Sprite::create("ui/icon_ban.png");
                if(sprIcon != nullptr) {
                    sprIcon->setPosition(BTN_ICON2_X, offset_y);
                    btn->addChild(sprIcon);
                }
            }
            
            auto sprPriceBack = Scale9Sprite::create("ui/board_price.png");
            if(sprPriceBack != nullptr) {
                sprPriceBack->setColor(PRICE_COLOR);
                sprPriceBack->setContentSize(PRICE_SIZE);
                sprPriceBack->setPosition(Vec2(BTN_PRICE_X, offset_y));
                btn->addChild(sprPriceBack);
                
                auto labelPrice = Label::createWithTTF(vecInfo[i].price, FONT_PATH, FONT_SIZE_PRICE);
                if(labelPrice != nullptr) {
                    labelPrice->setPosition(sprPriceBack->getContentSize() * 0.5f);
                    sprPriceBack->addChild(labelPrice);
                }
            }
        }
    }
    
    auto btnRestore = Button::create("ui/board_item.png");
    if(btnRestore != nullptr) {
        btnRestore->setScale9Enabled(true);
        btnRestore->setColor(RESTORE_COLOR_ON);
        btnRestore->setContentSize(RESTORE_SIZE);
        btnRestore->addClickEventListener(CC_CALLBACK_1(ShopLayer::callbackRestore, this));
        btnRestore->setPosition(Vec2(center.x, RESTORE_Y));
        btnRestore->setEnabled(true);
        this->addChild(btnRestore, 0);
        
        auto labText = Label::createWithTTF("RESTORE PURCHASE", FONT_PATH, FONT_SIZE_TEXT);
        if(labText != nullptr) {
            labText->setPosition(btnRestore->getContentSize() * 0.5f);
            btnRestore->addChild(labText);
        }
    }
    
    return true;
}

void ShopLayer::onEnter() {
    Layer::onEnter();
    
    keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = CC_CALLBACK_2(ShopLayer::onKeyReleased, this);
    
    customListener = EventListenerCustom::create(E_RELOAD_SCENE, CC_CALLBACK_1(ShopLayer::onReload, this));
    
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(keyListener, this);
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(customListener, this);
    
    if(ACCOUNT->isNoAds == false) {
        if(sdkbox::PluginAdMob::isAvailable("home"))
            sdkbox::PluginAdMob::show("home");
        else
            sdkbox::PluginAdMob::cache("home");
    }
    else
        sdkbox::PluginAdMob::hide("home");
}

void ShopLayer::onExit() {
    EVENT_DISPATCHER->removeEventListener(keyListener);
    EVENT_DISPATCHER->removeEventListener(customListener);
    
    Layer::onExit();
}

void ShopLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event) {
    if(keycode == EventKeyboard::KeyCode::KEY_BACK) {
        auto scene = TitleLayer::createScene();
        auto director = Director::getInstance();
        director->replaceScene(scene);

        AUDIO->playEffect("sfx/click.mp3");
    }
}

void ShopLayer::callbackTitle(Ref* pSender) {
    auto scene = TitleLayer::createScene();
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/click.mp3");
}

void ShopLayer::callbackMore(Ref* pSender) {
    auto scene = MoreLayer::createScene();
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/click.mp3");
}

void ShopLayer::callbackVolume(Ref* pSender) {
    ACCOUNT->isMute = ACCOUNT->isMute ? false : true;
    ACCOUNT->sync();
    
    if(ACCOUNT->isMute == false) {
        sprVolume->setTexture("ui/icon_volume.png");
        AUDIO->setBackgroundMusicVolume(1.f);
        AUDIO->setEffectsVolume(1.f);
    }
    else {
        sprVolume->setTexture("ui/icon_volume_off.png");
        AUDIO->setBackgroundMusicVolume(FLT_MIN);
        AUDIO->setEffectsVolume(FLT_MIN);
    }
    
    AUDIO->playEffect("sfx/click.mp3");
}

void ShopLayer::callbackBuy(Ref* pSender) {
    auto popup = WaitPopup::create();
    if(popup != nullptr) {
        POPUP_MANAGER->addPopup(popup, true);
    }
    
    AUDIO->playEffect("sfx/click.mp3");
    
    Node* p = (Node*)pSender;
    if(p) {
        int tag = p->getTag();
        
        if(tag == 0)
            sdkbox::IAP::purchase(SKU_NO_ADS);
        else if(tag == 1)
            sdkbox::IAP::purchase(SKU_HINT_5);
        else if(tag == 2)
            sdkbox::IAP::purchase(SKU_HINT_25);
        else if(tag == 3)
            sdkbox::IAP::purchase(SKU_HINT_100);
        else if(tag == 4)
            sdkbox::IAP::purchase(SKU_HINT_250);
    }
}

void ShopLayer::callbackRestore(Ref* pSender) {
    AUDIO->playEffect("sfx/click.mp3");
    
    sdkbox::IAP::restore();
}

void ShopLayer::onReload(EventCustom *event) {
    auto scene = ShopLayer::createScene();
    auto director = Director::getInstance();
    director->replaceScene(scene);
}
