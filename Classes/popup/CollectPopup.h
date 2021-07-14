//
//  CollectPopup.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 28/05/2019.
//

#ifndef CollectPopup_hpp
#define CollectPopup_hpp

#include "Common.h"

class CollectPopup : public Layer
{
public:
    enum {
        Z_BACK = 0,
        Z_TEXT,
        Z_ANIMAL,
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
    
    void funcShow(float);
    void funcButton(float);
public:
    EventListenerTouchOneByOne* listener;
    Sprite* sprPopup;
    
    MenuItemImage* btnOk;
    Sprite* sprBtn;
    
    static string strCollect;
    static int idxAnimal;
    
    // implement the "static create()" method manually
    CREATE_FUNC(CollectPopup);
};

#endif /* CollectPopup_hpp */
