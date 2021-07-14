//
//  DailyPopup.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 25/05/2019.
//

#ifndef DailyPopup_hpp
#define DailyPopup_hpp

#include "Common.h"

class DailyPopup : public Layer
{
public:
    enum {
        Z_BAND = 0,
        Z_TEXT,
        Z_BUTTON,
        Z_BUTTON_ICON,
    };
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void update(float dt) override;
    
    void callbackOk(Ref* pSender);
public:
    EventListenerTouchOneByOne* listener;
    Sprite* sprPopup;
    
    // implement the "static create()" method manually
    CREATE_FUNC(DailyPopup);
};

#endif /* DailyPopup_hpp */
