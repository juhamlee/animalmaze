//
//  Animal.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 09/01/2019.
//

#include "Animal.h"

bool Animal::init()
{
    Layer::init();
    
    sprCharacter = nullptr;
    nodeDarkness = nullptr;
    layerBlack = nullptr;
    
    for(int i = 0; i < NUMBER_OF_DIRECTION; i++) {
        sprArrow[i][0] = nullptr;
        sprArrow[i][1] = nullptr;
        sprArrow[i][2] = nullptr;
    }
    vecArrows.clear();
    
    nodeBase = Node::create();
    if(nodeBase != nullptr) {
        nodeBase->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        nodeBase->setPosition(0, 0);
        this->addChild(nodeBase);
    }
    
    static const char* FILES[] = {
        "game/arrow_v.png",
        "game/arrow_v.png",
        "game/arrow_h.png",
        "game/arrow_h.png"
    };
    
    for(int i = 0; i < NUMBER_OF_DIRECTION; i++) {
        for(int j = 0; j < 3; j++) {
            sprArrow[i][j] = Sprite::create(FILES[i]);
            if(sprArrow[i][j] != nullptr) {
                if(i == DIR_UP)
                    sprArrow[i][j]->setFlippedY(true);
                else if(i == DIR_LEFT)
                    sprArrow[i][j]->setFlippedX(true);
                
                sprArrow[i][j]->setVisible(false);
                nodeBase->addChild(sprArrow[i][j], Z_ARROW);
            }
        }
    }
    
    fScale = 1.0f;
    
    return true;
}

void Animal::onEnter() {
    Layer::onEnter();
    
    scheduleUpdate();
}

void Animal::update(float dt) {
    if(nodeDarkness && layerBlack) {
        layerBlack->setPosition(-((this->getPosition() + Vec2(10, 190)) / fScale));
    }
}

void Animal::setScale(float scale) {
    nodeBase->setScale(scale);
    fScale = scale;
    
    if(layerBlack) {
        layerBlack->setContentSize(Size(720, 1280) / fScale);
    }
}

void Animal::setAnimalByNumber(int number) {
    if(sprCharacter != nullptr) {
        sprCharacter->removeFromParent();
        sprCharacter = nullptr;
    }
    
    if(number == 0) {
        sprCharacter = Sprite::create("animal/soul.png");
        sprCharacter->setScale(1.2f);
        nodeBase->addChild(sprCharacter, Z_CHARACTER);
    }
    else {
        char buf[32];
        sprintf(buf, "animal/animals-%d.png", number);
        
        sprCharacter = Sprite::create(buf);
        if(sprCharacter != nullptr) {
            sprCharacter->setScale(1.0f);
            nodeBase->addChild(sprCharacter, Z_CHARACTER);
        }
    }
}

void Animal::setFlip(bool isFlip) {
    if(sprCharacter != nullptr)
        sprCharacter->setFlippedX(isFlip);
}

void Animal::setDarkness(bool isDarkness) {
    if(nodeDarkness != nullptr) {
        nodeDarkness->setVisible(isDarkness);
    }
    else {
        if(isDarkness == true) {
            nodeDarkness = Node::create();
            nodeBase->addChild(nodeDarkness, Z_DARKNESS);
            
            layerBlack = LayerColor::create(Color4B::BLACK, 720 * 1.3f / fScale, 1280 * 1.3f / fScale);
            layerBlack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            layerBlack->setPosition(-((this->getPosition() + Vec2(10, 190)) / fScale));
            
            auto clip = ClippingNode::create();
            sprBlack = Sprite::create("game/black.png");
            sprBlack->setPosition(0, 0);
            clip->setStencil(sprBlack);
            clip->setInverted(true);
            clip->addChild(layerBlack);
            nodeDarkness->addChild(clip);
            
            sprLight = Sprite::create("game/light.png");
            nodeDarkness->addChild(sprLight);
        }
    }
}

void Animal::setLightScale(float scale) {
    if(sprBlack != nullptr)
        sprBlack->setScale(scale);
    if(sprLight != nullptr)
        sprLight->setScale(scale);
}

void Animal::hideArrowAll() {
    for(int i = 0; i < NUMBER_OF_DIRECTION; i++) {
        for(int j = 0; j < 3; j++) {
            if(sprArrow[i][j] != nullptr) {
                sprArrow[i][j]->setVisible(false);
                sprArrow[i][j]->stopAllActions();
            }
        }
    }
    
    for(int i = 0; i < (unsigned int)vecArrows.size(); i++) {
        vecArrows[i]->removeFromParent();
    }
    vecArrows.clear();
}

void Animal::showArrow(int dir, bool isFirst) {
    if(sprArrow[dir] != nullptr) {
        sprArrow[dir][0]->setVisible(true);
        sprArrow[dir][1]->setVisible(true);
        sprArrow[dir][2]->setVisible(true);
        
        Vec2 pos[3];
        if(dir == DIR_UP) {
            pos[0] = Vec2(0, 60);
            pos[1] = Vec2(0, 68);
            pos[2] = Vec2(0, 76);
        }
        else if(dir == DIR_DOWN) {
            pos[0] = Vec2(0, -60);
            pos[1] = Vec2(0, -68);
            pos[2] = Vec2(0, -76);
        }
        else if(dir == DIR_LEFT) {
            pos[0] = Vec2(-60, 0);
            pos[1] = Vec2(-68, 0);
            pos[2] = Vec2(-76, 0);
        }
        else if(dir == DIR_RIGHT) {
            pos[0] = Vec2(60, 0);
            pos[1] = Vec2(68, 0);
            pos[2] = Vec2(76, 0);
        }
        
        for(int i = 0; i < 3; i++)
            sprArrow[dir][i]->setPosition(pos[i]);
        
        /*Spawn* spawn[3] = {NULL, };
        for(int i = 0; i < 3; i++)
            spawn[i] = Spawn::create(FadeOut::create(0.f), MoveTo::create(0.f, pos[i]), NULL);
        auto spawnFade = Spawn::create(FadeIn::create(0.1f), DelayTime::create(0.3f), NULL);
        */
        
        Repeat* rep[3] = {NULL, };
        rep[0] = Repeat::create(Sequence::create(FadeOut::create(0.f),
                                                 FadeIn::create(0.5f),
                                                 DelayTime::create(0.0f),
                                                 FadeOut::create(0.5f),
                                                 DelayTime::create(1.0f), NULL), 9999);
        rep[1] = Repeat::create(Sequence::create(FadeOut::create(0.f),
                                                 DelayTime::create(0.5f),
                                                 FadeIn::create(0.5f),
                                                 DelayTime::create(0.0f),
                                                 FadeOut::create(0.5f),
                                                 DelayTime::create(0.5f), NULL), 9999);
        rep[2] = Repeat::create(Sequence::create(FadeOut::create(0.f),
                                                 DelayTime::create(1.0f),
                                                 FadeIn::create(0.5f),
                                                 DelayTime::create(0.0f),
                                                 FadeOut::create(0.5f), NULL), 9999);
        
        if(isFirst) {
            for(int i = 0; i < 3; i++)
                sprArrow[dir][i]->runAction(rep[i]);
        }
        else {
            for(int i = 0; i < 3; i++)
                sprArrow[dir][i]->runAction(Sequence::create(FadeOut::create(0.f), DelayTime::create(5.0f), rep[i], NULL));
        }
    }
}

void Animal::addArrow(float angle, bool isFirst) {
    Vec2 unit(1, 0);
    unit.rotate(Vec2(0, 0), angle);
    
    Vec2 pos[3];
    pos[0] = unit * 60.f;
    pos[1] = unit * 68.f;
    pos[2] = unit * 76.f;
    
    Repeat* rep[3] = {NULL, };
    rep[0] = Repeat::create(Sequence::create(FadeOut::create(0.f),
                                             FadeIn::create(0.5f),
                                             DelayTime::create(0.0f),
                                             FadeOut::create(0.5f),
                                             DelayTime::create(1.0f), NULL), 9999);
    rep[1] = Repeat::create(Sequence::create(FadeOut::create(0.f),
                                             DelayTime::create(0.5f),
                                             FadeIn::create(0.5f),
                                             DelayTime::create(0.0f),
                                             FadeOut::create(0.5f),
                                             DelayTime::create(0.5f), NULL), 9999);
    rep[2] = Repeat::create(Sequence::create(FadeOut::create(0.f),
                                             DelayTime::create(1.0f),
                                             FadeIn::create(0.5f),
                                             DelayTime::create(0.0f),
                                             FadeOut::create(0.5f), NULL), 9999);
    
    for(int i = 0; i < 3; i++) {
        auto arrow = Sprite::create("game/arrow_h.png");
        if(arrow != nullptr) {
            arrow->setRotation(-CC_RADIANS_TO_DEGREES(angle));
            arrow->setPosition(pos[i]);
            vecArrows.push_back(arrow);
            nodeBase->addChild(arrow, Z_ARROW);
            
            if(isFirst)
                arrow->runAction(rep[i]);
            else
                arrow->runAction(Sequence::create(FadeOut::create(0.f), DelayTime::create(5.0f), rep[i], NULL));
        }
    }
}
