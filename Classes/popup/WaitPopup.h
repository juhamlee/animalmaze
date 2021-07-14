//
//  WaitPopup.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 23/02/2019.
//

#ifndef WaitPopup_hpp
#define WaitPopup_hpp

#include "Common.h"

class WaitPopup : public Layer
{
public:
    enum {
        Z_TEXT = 0,
    };
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void update(float dt) override;
    
    void close(float);
public:
    EventListenerTouchOneByOne* listener;
    Sprite* sprPopup;
    
    // implement the "static create()" method manually
    CREATE_FUNC(WaitPopup);
};

#endif /* WaitPopup_hpp */
