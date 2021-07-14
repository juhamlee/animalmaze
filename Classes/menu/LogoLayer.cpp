//
//  LogoScene.cpp
//  animalmaze-mobile
//

#include "LogoLayer.h"
#include "TitleLayer.h"

#include "PluginAppnext/PluginAppnext.h"

Scene* LogoLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = LogoLayer::create();
    
    if(scene != nullptr && layer != nullptr) {
        scene->addChild(layer);
        return scene;
    }
    
    return nullptr;
}

LogoLayer::LogoLayer() {
    AUDIO->preloadEffect("sfx/click.mp3");
    AUDIO->preloadEffect("sfx/mode.mp3");
    AUDIO->preloadEffect("sfx/play.mp3");
    
    time = 100.f;
    delay = 0.f;
}

LogoLayer::~LogoLayer() {
    
}

bool LogoLayer::init()
{
    Layer::init();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto sprite = Sprite::create("ui/dopaminit.png");
    if (sprite != nullptr) {
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        this->addChild(sprite, 0);
    }
    
    return true;
}

void LogoLayer::onEnter() {
    Layer::onEnter();
    
    auto seq = Sequence::create(DelayTime::create(1.f), CallFunc::create(CC_CALLBACK_0(LogoLayer::finish, this)), nullptr);
    this->runAction(seq);
    
    scheduleUpdate();
}

void LogoLayer::finish() {
    if(ACCOUNT->isMute) {
        AUDIO->setBackgroundMusicVolume(FLT_MIN);
        AUDIO->setEffectsVolume(FLT_MIN);
    }
    time = 10.f;
}

void LogoLayer::update(float dt) {
    if(0.f < time) {
        time -= dt;
        if(time <= 10.f) {
            delay += dt;
            if(0.5f < delay) {
                delay = 0.f;
                if(sdkbox::PluginAppnext::isAdReady()) {
                    sdkbox::PluginAppnext::showAd();
                    delay = 0.f;
                    time = 0.f;
                }
                else
                    sdkbox::PluginAppnext::cacheAd("default");
            }
        }
        
    }
    else {
        auto scene = TitleLayer::createScene();
        auto director = Director::getInstance();
        director->replaceScene(scene);
    }
}
