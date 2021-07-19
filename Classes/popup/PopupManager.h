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
    
    //팝업이 addChild 될 씬의 노드와 함께 초기화를 한다
    //씬이 트랜지션 될 때 마다 호출되어야 함
    void initWithBaseNode(Node* base);
    //네트워크 신호를 기다리는 팝업의 경우 isWait = ture로 호출
    void addPopup(Node* p, bool isWait = false);
    void closePopup();
    bool isEmpty();
    
    //네트워크 신호의 처리가 끝났을 때 호출 됨
    void closeWaitPopup();
    
public:
    //팝업이 addChild 될 씬의 베이스노드
    Node* nodeBase;
    //중첩된 팝업의 목록을 list로 관리한다
    list<Node*> listPopup;
    bool recentWait;
    
public:
    //싱글턴 구현과 호출을 위한 부분
    static PopupManager* getInstance();
    static PopupManager* sharedInstance;
};

#endif /* PopupManager_hpp */
