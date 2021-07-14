//
//  LogoLayer.h
//  animalmaze-mobile
//

#ifndef LogoLayer_h
#define LogoLayer_h

#include "Common.h"

class LogoLayer : public Layer
{
public:
    static Scene* createScene();
    
    LogoLayer();
    ~LogoLayer();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void update(float dt) override;
    
    void finish(void);
    
    // implement the "static create()" method manually
    CREATE_FUNC(LogoLayer);
public:
    float time;
    float delay;
};

#endif /* LogoLayer_h */
