//
//  PathLayer.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 03/03/2019.
//

#ifndef PathLayer_hpp
#define PathLayer_hpp

#include "Common.h"

class PathLayer : public Node {
public:
    PathLayer();
    ~PathLayer();
    
public:
    vector<Vec2> vecPoints;
    Vec2 ptExtra;
    
    Color3B color;
    float fScale;
    
    vector<Sprite*> vecSprLines;
    vector<Sprite*> vecSprJoints;
    Sprite* sprExtra;
    
    CREATE_FUNC(PathLayer);
    
public:
    void pushPoint(Vec2 v);
    void popPoint();
    void setExtraPoint(Vec2 v);
    void setPoints(vector<Vec2> vv, float limit = 0.f);
    
    void setColor(Color3B _color);
    virtual void setScale(float scale) override;
    
    void updateExtra();
};

#endif /* PathLayer_hpp */
