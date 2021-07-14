//
//  TitleLayer.h
//  animalmaze-mobile
//
//  Created by 이주함 on 05/01/2019.
//

#ifndef TitleLayer_h
#define TitleLayer_h

#include "Common.h"

class TitleLayer : public Layer
{
public:
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
