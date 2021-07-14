//
//  MoreLayer.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 04/03/2019.
//

#ifndef MoreLayer_hpp
#define MoreLayer_hpp

#include "Common.h"

class MoreLayer : public Layer
{
public:
    static Scene* createScene();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event) override;
    
    
    void callbackTitle(Ref* pSender);
    void callbackVolume(Ref* pSender);
    void callbackShop(Ref* pSender);
    
    void callbackLink(Ref* pSender);
    
public:
    EventListenerKeyboard* keyListener;
    Sprite* sprVolume;
    
    // implement the "static create()" method manually
    CREATE_FUNC(MoreLayer);
};

#endif /* MoreLayer_hpp */
