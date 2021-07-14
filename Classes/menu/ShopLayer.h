//
//  ShopLayer.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 05/01/2019.
//

#ifndef ShopLayer_hpp
#define ShopLayer_hpp

#include "Common.h"

class ShopLayer : public Layer
{
public:
    enum {
        BESTSELLER = 0,
        BESTVALUE,
        NORMAL,
    };
    
    struct ShopInfo {
    public:
        int type;
        bool isNoAds;
        int hints;
        string price;
        
        ShopInfo(int _type, bool _isNoAds, int _hints, string _price) {
            type = _type;
            isNoAds = _isNoAds;
            hints = _hints;
            price = _price;
        }
    };
    
public:
    static Scene* createScene();

    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event) override;
    
    
    void callbackTitle(Ref* pSender);
    void callbackMore(Ref* pSender);
    void callbackVolume(Ref* pSender);
    
    void callbackBuy(Ref* pSender);
    void callbackRestore(Ref* pSender);
    
public:
    EventListenerKeyboard* keyListener;
    Sprite* sprVolume;
    
    // implement the "static create()" method manually
    CREATE_FUNC(ShopLayer);
};

#endif /* ShopLayer_hpp */
