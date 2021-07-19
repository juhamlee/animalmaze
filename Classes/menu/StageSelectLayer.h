//
//  StageSelectLayer.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 24/02/2019.
//

#ifndef StageSelectLayer_hpp
#define StageSelectLayer_hpp

#include "Common.h"

class StageSelectLayer : public Layer
{
public:
    static Scene* createScene(int category);

    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event) override;
    
    void callbackTitle(Ref* pSender);
    void callbackUnlock(Ref* pSender);
    void callbackSelect(Ref* pSender);
    
    void onReload(EventCustom* event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(StageSelectLayer);
    
public:
    EventListenerKeyboard* keyListener;
    EventListenerCustom* customListener;
    
    int category;
};

#endif /* StageSelectLayer_hpp */
