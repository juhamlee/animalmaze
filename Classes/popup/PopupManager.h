//
//  PopupManager.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 23/02/2019.
//

#ifndef PopupManager_hpp
#define PopupManager_hpp

#include "Common.h"

class PopupManager {
public:
    PopupManager();
    ~PopupManager();
    
    void initWithBaseNode(Node* base);
    void addPopup(Node* p);
    void closePopup();
    bool isEmpty();
    
public:
    Node* nodeBase;
    list<Node*> listPopup;
    
public:
    static PopupManager* getInstance();
    static PopupManager* sharedInstance;
};

#endif /* PopupManager_hpp */
