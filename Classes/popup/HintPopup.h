//
//  HintPopup.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 24/02/2019.
//

#ifndef HintPopup_hpp
#define HintPopup_hpp

#include "Common.h"

class HintPopup : public Layer
{
public:
    enum {
        Z_TEXT = 0,
        Z_HINT,
        Z_BUTTON,
        Z_BUTTON_ICON,
    };
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void update(float dt) override;
    
    void callbackNo(Ref* pSender);
    void callbackShop(Ref* pSender);
    void callbackYes(Ref* pSender);
    
public:
    EventListenerTouchOneByOne* listener;
    Sprite* sprPopup;
    
    // implement the "static create()" method manually
    CREATE_FUNC(HintPopup);
};

#endif /* HintPopup_hpp */
