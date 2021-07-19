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
    
    recentWait = false;
}

PopupManager::~PopupManager() {
    listPopup.clear();
    
    recentWait = false;
}

PopupManager* PopupManager::getInstance() {
    //첫 호출 시 인스턴스가 생성되어있지 않다면 인스턴스를 할당해준다
    if(sharedInstance == nullptr) {
        sharedInstance = new PopupManager;
    }
    
    return sharedInstance;
}

void PopupManager::initWithBaseNode(Node* base) {
    //팝업이 붙을 베이스노드를 할당하고 팝업 목록을 초기화
    if(base != nullptr) {
        nodeBase = base;
    }
    
    //활성화 된 팝업이 있다면 전부 삭제 후 비워준다
    for(int i = 0; i < (unsigned int)listPopup.size(); i++)
        closePopup();
    listPopup.clear();
}

void PopupManager::addPopup(Node* p, bool isWait) {
    if(nodeBase != nullptr && p != nullptr) {
        //기존의 팝업은 전부 숨김처리 후 새 팝업만 노출시킴
        list<Node*>::iterator itr = listPopup.begin();
        for(;itr != listPopup.end(); itr++) {
            (*itr)->setVisible(false);
        }
        listPopup.push_back(p);
        nodeBase->addChild(p, 999);
        
        //네트워크 대기 팝업인지 체크
        recentWait = isWait;
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
    
    //팝업을 종료 후 리스트에 팝업이 남아있으면 제일 최근의 팝업을 다시 노출시킴
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

//네트워크 처리가 끝났을 경우 호출 됨
void PopupManager::closeWaitPopup() {
    if(recentWait) {
        closePopup();
    }
}
