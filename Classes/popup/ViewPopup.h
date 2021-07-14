//
//  ViewPopup.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 23/02/2019.
//

#ifndef ViewPopup_hpp
#define ViewPopup_hpp

#include "Common.h"

class ViewPopup : public Layer
{
public:
    enum {
        Z_BUTTON = 0,
        Z_TEXT,
    };
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void update(float dt) override;
    
    void callbackBack(Ref* pSender);
    
public:
    EventListenerTouchOneByOne* listener;
    Sprite* sprPopup;
    
    // implement the "static create()" method manually
    CREATE_FUNC(ViewPopup);
};

#endif /* ViewPopup_hpp */
