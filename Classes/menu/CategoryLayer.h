//
//  CategoryLayer.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 05/01/2019.
//

#ifndef CategoryLayer_hpp
#define CategoryLayer_hpp

#include "Common.h"

class CategoryLayer : public Layer
{
public:
    static Scene* createScene();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event) override;
    
    void callbackTitle(Ref* pSender);
    void callbackClassic(Ref* pSender);
    void callbackIce(Ref* pSender);
    void callbackHurry(Ref* pSender);
    void callbackDark(Ref* pSender);
    
public:
    EventListenerKeyboard* keyListener;
    
    // implement the "static create()" method manually
    CREATE_FUNC(CategoryLayer);
};

#endif /* CategoryLayer_hpp */
