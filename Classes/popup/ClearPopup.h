//
//  ClearPopup.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 23/02/2019.
//

#ifndef ClearPopup_hpp
#define ClearPopup_hpp

#include "Common.h"

class ClearPopup : public Layer
{
public:
    enum {
        Z_EXP = 0,
        Z_BUTTON,
        Z_BUTTON_ICON,
        Z_TEXT,
    };
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void update(float dt) override;
    
    void callbackHome(Ref* pSender);
    void callbackNext(Ref* pSender);
    void callbackLike(Ref* pSender);
    void callbackView(Ref* pSender);
    void callbackRetry(Ref* pSender);

public:
    EventListenerTouchOneByOne* listener;
    
    Sprite* sprPopup;
    ProgressTimer* progressExp;
    Label* labLevel;
    
    ParticleSystemQuad* emitter[4];
    float delay;
    
    // implement the "static create()" method manually
    CREATE_FUNC(ClearPopup);
};

#endif /* ClearPopup_hpp */
