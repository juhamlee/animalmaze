//
//  PausePopup.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 24/02/2019.
//

#ifndef PausePopup_hpp
#define PausePopup_hpp

#include "Common.h"

//게임 일시정지 시 노출되는 팝업
class PausePopup : public Layer
{
public:
    enum {
        Z_BUTTON = 0,
        Z_BUTTON_ICON,
    };
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    //팝업 뒷 레이어의 터치를 막기위한 터치 이벤트
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void update(float dt) override;
    
    void callbackHome(Ref* pSender); //로비로 돌아가는 버튼 콜백
    void callbackResume(Ref* pSender); //게임을 재개하는 버튼 콜백
    void callbackShop(Ref* pSender); //상점을 여는 버튼 콜백
    
public:
    EventListenerTouchOneByOne* listener;
    Sprite* sprPopup;
    
    // implement the "static create()" method manually
    CREATE_FUNC(PausePopup);
};

#endif /* PausePopup_hpp */
