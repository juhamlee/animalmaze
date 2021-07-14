//
//  TimeupPopup.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 24/02/2019.
//

#ifndef TimeupPopup_hpp
#define TimeupPopup_hpp

#include "Common.h"

class TimeupPopup : public Layer
{
public:
    enum {
        Z_TEXT = 0,
        Z_ICON
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
    CREATE_FUNC(TimeupPopup);
};

#endif /* TimeupPopup_hpp */
