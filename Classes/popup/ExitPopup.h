//
//  ExitPopup.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 23/02/2019.
//

#ifndef ExitPopup_hpp
#define ExitPopup_hpp

#include "Common.h"

class ExitPopup : public Layer
{
public:
    enum {
        Z_BUTTON = 0,
        Z_BUTTON_ICON,
        Z_TEXT,
    };
    
public:    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event) override;
    
    void callbackYes(Ref* pSender);
    void callbackNo(Ref* pSender);
    
public:
    EventListenerTouchOneByOne* listener;
    EventListenerKeyboard* keyListener;
    
    Sprite* sprPopup;
    
    // implement the "static create()" method manually
    CREATE_FUNC(ExitPopup);
};

#endif /* ExitPopup_hpp */
