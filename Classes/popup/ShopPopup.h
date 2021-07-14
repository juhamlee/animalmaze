//
//  ShopPopup.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 24/02/2019.
//

#ifndef ShopPopup_hpp
#define ShopPopup_hpp

#include "Common.h"

class ShopPopup : public Layer
{
public:
    enum {
        Z_ITEM = 0,
        Z_BUTTON,
        Z_BUTTON_ICON,
        Z_ICON,
        Z_TEXT,
    };
    
    struct ShopInfo {
    public:
        int hint;
        bool isNoAds;
        string price;
        
        ShopInfo(bool _isNoAds, int _hint, string _price) {
            isNoAds = _isNoAds;
            hint = _hint;
            price = _price;
        }
    };
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void update(float dt) override;
    
    void callbackBuy(Ref* pSender);
    void callbackOk(Ref* pSender);
    
public:
    EventListenerTouchOneByOne* listener;
    Sprite* sprPopup;
    
    // implement the "static create()" method manually
    CREATE_FUNC(ShopPopup);
};

#endif /* ShopPopup_hpp */
