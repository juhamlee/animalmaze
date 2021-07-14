//
//  MazeAlphaLayer.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 09/01/2019.
//

#ifndef MazeAlphaLayer_hpp
#define MazeAlphaLayer_hpp

#include "Common.h"
#include "Animal.h"
#include "PathLayer.h"


class MazeNode {
public:
    MazeNode() {
        index = 0;
        pos = Vec2(0,0);
        vecpAdjs.clear();
    }
    
    int index;
    Vec2 pos;
    vector<MazeNode*> vecpAdjs;
};

class MazeAlphaLayer : public Layer
{
public:
    enum {
        Z_BACK = 0,
        Z_PATH_HINT,
        Z_PATH_TRACE,
        Z_MAZE,
        Z_UI,
        Z_BUTTON,
        Z_BUTTON_OVER,
    };
    
    enum {
        READY = 0,
        IDLE,
        MOVE,
        FINISH,
    };
    
public:
    static Scene* createScene();
    
    MazeAlphaLayer();
    ~MazeAlphaLayer();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void onTouchMoved(Touch* touch, Event* event) override;
    virtual void update(float dt) override;
    
    void onUseHint(EventCustom* event);
    void onFinish(EventCustom* event);
    void callbackPause(Ref* pSender);
    void callbackRetry(Ref* pSender);
    void callbackHint(Ref* pSender);
    
    void start();
    void moveTo(int index);
    void check();
    void makeArrow(bool isFist = false);
    
    void clear();
    void finish(float dt = 0.f);
    
    Vec2 getPositionFromIndex(int idx);
    
public:
    EventListenerTouchOneByOne* listener;
    EventListenerCustom* customListener;
    EventListenerCustom* finishListener;
    
    int state;
    bool isTouch;
    
    Animal* player;
    Animal* follower;
    
    Vec2 touchOrigin;
    
    vector<MazeNode> vecMazeNode;
    
    vector<int> vecTrace;
    vector<Vec2> vecTracePosition;
    vector<Vec2> vecHint;
    vector<Vec2> vecFollow;
    PathLayer* pathTrace;
    PathLayer* pathHint;
    
    int startIndex;
    int endIndex;
    int currentIndex;
    int lastIndex;
    
    bool isFollow;
    bool isHintUsed;
    int coloridx;
    
    float fScale;
    
    Label* labHint;
    
    int moveSoundId;
    
    // implement the "static create()" method manually
    CREATE_FUNC(MazeAlphaLayer);
};

#endif /* MazeAlphaLayer_hpp */
