//
//  Animal.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 09/01/2019.
//

#ifndef Animal_hpp
#define Animal_hpp

#include "Common.h"

class Animal : public Layer
{
public:
    enum {
        DIR_UP = 0,
        DIR_DOWN,
        DIR_LEFT,
        DIR_RIGHT,
        
        NUMBER_OF_DIRECTION = 4
    };
    
public:
    enum {
        Z_CHARACTER = 0,
        Z_ARROW,
        Z_DARKNESS,
    };
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void update(float dt) override;
    virtual void setScale(float scale) override;
    
    void setAnimalByNumber(int number);
    void setFlip(bool isFlip);
    void setDarkness(bool isDarkness);
    void setLightScale(float scale);
    
    void hideArrowAll();
    void showArrow(int dir, bool isFirst =  false);
    void addArrow(float angle, bool isFirst = false);
    
public:
    Node* nodeBase;
    Sprite* sprCharacter;
    Node* nodeDarkness;
    LayerColor* layerBlack;
    Sprite* sprBlack;
    Sprite* sprLight;
    
    Sprite* sprArrow[NUMBER_OF_DIRECTION][3];
    vector<Sprite*> vecArrows;
    
    float fScale;
    
    // implement the "static create()" method manually
    CREATE_FUNC(Animal);
};

#endif /* Animal_hpp */
