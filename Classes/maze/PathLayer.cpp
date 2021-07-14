//
//  PathLayer.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 03/03/2019.
//

#include "PathLayer.h"

PathLayer::PathLayer() {
    
    vecPoints.clear();
    ptExtra = Vec2(0, 0);
    
    color = Color3B::BLACK;
    
    fScale = 1.f;
    
    vecSprLines.clear();
    vecSprJoints.clear();
    sprExtra = nullptr;
}

PathLayer::~PathLayer() {
    vecPoints.clear();
    vecSprLines.clear();
    vecSprJoints.clear();
    
    ptExtra = Vec2(0, 0);
}

void PathLayer::pushPoint(Vec2 v) {
    vecPoints.push_back(v);
    
    int size = (unsigned int)vecPoints.size();
    if(1 < size) {
        Vec2 prev = vecPoints[size - 2];
        auto sprLine = Sprite::create("game/path_line.png");
        if(sprLine != nullptr) {
            sprLine->getTexture()->setAliasTexParameters();
            sprLine->setColor(Color3B(color));
            
            Vec2 dir = v - prev;
            Vec2 axis = Vec2(1, 0);
            Vec2 pos = prev.getMidpoint(v);
            float scale_x = prev.distance(v) / sprLine->getContentSize().width;
            float rotate = -MATH_RAD_TO_DEG(axis.getAngle(dir));
            
            sprLine->setPosition(pos);
            sprLine->setScale(scale_x, fScale);
            sprLine->setRotation(rotate);
            
            this->addChild(sprLine);
            
            vecSprLines.push_back(sprLine);
        }
    }
    
    auto sprJoint = Sprite::create("game/path_joint.png");
    if(sprJoint != nullptr) {
        sprJoint->getTexture()->setAliasTexParameters();
        sprJoint->setColor(Color3B(color));
        sprJoint->setPosition(v);
        sprJoint->setScale(fScale);
        this->addChild(sprJoint);
        
        vecSprJoints.push_back(sprJoint);
    }
}

void PathLayer::popPoint() {
    if(vecPoints.empty() == false)
        vecPoints.pop_back();
    
    if(vecSprLines.empty() == false) {
        Sprite* p_last = vecSprLines.back();
        this->removeChild(p_last);
        vecSprLines.pop_back();
    }
    
    if(vecSprJoints.empty() == false) {
        Sprite* p_last = vecSprJoints.back();
        this->removeChild(p_last);
        vecSprJoints.pop_back();
    }
}


void PathLayer::setExtraPoint(Vec2 v) {
    if(vecPoints.empty())
        return;
    
    Vec2 prev = vecPoints.back();
    if(sprExtra == nullptr) {
        sprExtra = Sprite::create("game/path_line.png");
        if(sprExtra != nullptr) {
            sprExtra->getTexture()->setAliasTexParameters();
            sprExtra->setColor(Color3B(color));
            this->addChild(sprExtra);
        }
    }
    
    if(sprExtra != nullptr) {
        Vec2 dir = v - prev;
        Vec2 axis = Vec2(1, 0);
        Vec2 pos = prev.getMidpoint(v);
        float scale_x = prev.distance(v) / sprExtra->getContentSize().width;
        float rotate = -MATH_RAD_TO_DEG(axis.getAngle(dir));
        
        sprExtra->setPosition(pos);
        sprExtra->setScale(scale_x, fScale);
        sprExtra->setRotation(rotate);
    }
}

void PathLayer::setPoints(vector<Vec2> vv, float limit) {
    if(vv.empty())
        return;
    
    float sum_dist = 0.f;
    Vec2 prev = vv[0];
    for(int i = 0; i < (unsigned int)vv.size(); i++) {
        pushPoint(vv[i]);
        
        if(0.f < limit) {
            Vec2 curr = vv[i];
            float dist = curr.distance(prev);
            sum_dist += dist;
            prev = curr;
            
            if(limit <= sum_dist)
                break;
        }
    }
}

void PathLayer::setColor(Color3B _color) {
    this->color = _color;
    
    for(int i = 0; i < (unsigned int)vecSprLines.size(); i++)
        vecSprLines[i]->setColor(color);
    
    for(int i = 0; i < (unsigned int)vecSprJoints.size(); i++)
        vecSprJoints[i]->setColor(color);
    
    if(sprExtra != nullptr)
        sprExtra->setColor(color);
}

void PathLayer::setScale(float _scale) {
    fScale = _scale;
    
    for(int i = 0; i < (unsigned int)vecSprLines.size(); i++)
        vecSprLines[i]->setScaleY(fScale);
    
    for(int i = 0; i < (unsigned int)vecSprJoints.size(); i++)
        vecSprJoints[i]->setScale(fScale);
    
    if(sprExtra != nullptr)
        sprExtra->setScaleY(fScale);
}

