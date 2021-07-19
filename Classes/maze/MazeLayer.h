//
//  MazeLayer.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 05/01/2019.
//

#ifndef MazeLayer_hpp
#define MazeLayer_hpp

#include "Common.h"
#include "Animal.h"
#include "PathLayer.h"

class MazeLayer : public Layer
{
public:
    enum {
        Z_BACK = 0,
        Z_MAZE,
        Z_PATH_HINT,
        Z_PATH_TRACE,
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
    
    MazeLayer();
    ~MazeLayer();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void onTouchMoved(Touch* touch, Event* event) override;
    virtual void update(float dt) override;
    
    void onUseHint(EventCustom* event);
    void onRefreshHint(EventCustom* event);
    void onFinish(EventCustom* event);
    void callbackPause(Ref* pSender);
    void callbackRetry(Ref* pSender);
    void callbackHint(Ref* pSender);
    
    void start();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void check();
    void makeArrow(bool isFirst = false);
    
    void clear();
    void finish(float dt = 0.f);
    
    Vec2 getPositionFromPoint(Point pt);
    int getMapFromPoint(Point pt);
    
public:
    EventListenerTouchOneByOne* listener;
    EventListenerCustom* customListener;
    EventListenerCustom* refreshListener;
    EventListenerCustom* finishListener;
    
    int state;
    bool isTouch;
    
    Size mapSize;
    vector<vector<int>> map;
    
    vector<Vec2> vecTrace;
    vector<Vec2> vecTracePosition;
    vector<Vec2> vecHint;
    PathLayer* pathTrace;
    PathLayer* pathHint;
    
    Animal* player;
    Animal* follower;
    
    Point ptStart;
    Point ptEnd;
    Point ptCurrent;
    Point ptLast;
    
    Vec2 touchOrigin;
    
    bool isFollow;
    int  hintLimit;
    int  hintUsed;
    
    float fScale;
    Vec2 vScale;
    
    Label* labHint;
    Label* labTime;
    float fTime;
    
    int lastDirection;
    
    int moveSoundId;
    
    // implement the "static create()" method manually
    CREATE_FUNC(MazeLayer);
};

#endif /* MazeLayer_hpp */
