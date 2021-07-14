//
//  PopupManager.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 23/02/2019.
//

#include "PopupManager.h"

PopupManager* PopupManager::sharedInstance = nullptr;

PopupManager::PopupManager() {
    nodeBase = nullptr;
    listPopup.clear();
}

PopupManager::~PopupManager() {
    listPopup.clear();
}

PopupManager* PopupManager::getInstance() {
    if(sharedInstance == nullptr) {
        sharedInstance = new PopupManager;
    }
    
    return sharedInstance;
}

void PopupManager::initWithBaseNode(Node* base) {
    if(base != nullptr) {
        nodeBase = base;
    }
    listPopup.clear();
}

void PopupManager::addPopup(Node* p) {
    if(nodeBase != nullptr && p != nullptr) {
        list<Node*>::iterator itr = listPopup.begin();
        for(;itr != listPopup.end(); itr++) {
            (*itr)->setVisible(false);
        }
        listPopup.push_back(p);
        nodeBase->addChild(p, 999);
    }
}

void PopupManager::closePopup() {
    int size = (int)listPopup.size();
    if(0 < size) {
        Node* p = listPopup.back();
        listPopup.pop_back();
        if(p != nullptr) {
            p->stopAllActions();
            p->removeFromParent();
        }
    }
    
    if(0 < size) {
        Node* p = listPopup.back();
        if(p != nullptr) {
            p->setVisible(true);
        }
    }
}

bool PopupManager::isEmpty() {
    return listPopup.empty();
}
