//
//  TitleLayer.h
//  animalmaze-mobile
//
//  Created by 이주함 on 05/01/2019.
//

#ifndef TitleLayer_h
#define TitleLayer_h

#include "Common.h"

//타이틀 화면의 씬과 레이어
class TitleLayer : public Layer
{
public:
    //씬을 따로 구현 안하고 빈 씬에 레이어를 붙이는 식으로 구현
    static Scene* createScene();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event) override;
    
    void callbackPlay(Ref* pSender);
    void callbackVolume(Ref* pSender);
    void callbackRank(Ref* pSender);
    void callbackLike(Ref* pSender);
    void callbackMore(Ref* pSender);
    void callbackShop(Ref* pSender);
    
    void checkDailyBonus(float);
    
public:
    EventListenerKeyboard* keyListener;
    Sprite* sprVolume;
    
    // implement the "static create()" method manually
    CREATE_FUNC(TitleLayer);
};

#endif /* TitleLayer_h */
