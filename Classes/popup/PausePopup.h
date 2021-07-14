//
//  PausePopup.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 24/02/2019.
//

#ifndef PausePopup_hpp
#define PausePopup_hpp

#include "Common.h"

class PausePopup : public Layer
{
public:
    enum {
        Z_BUTTON = 0,
        Z_BUTTON_ICON,
    };
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void update(float dt) override;
    
    void callbackHome(Ref* pSender);
    void callbackResume(Ref* pSender);
    void callbackShop(Ref* pSender);
    
public:
    EventListenerTouchOneByOne* listener;
    Sprite* sprPopup;
    
    // implement the "static create()" method manually
    CREATE_FUNC(PausePopup);
};

#endif /* PausePopup_hpp */
