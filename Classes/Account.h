//
//  Account.h
//  animalmaze-mobile
//
//  Created by 이주함 on 05/01/2019.
//

#ifndef Account_h
#define Account_h

#include <vector>
#include <string>

using namespace std;

//레벨, 스테이지 진척상황 등 플레이 전반적인 정보를 저장하는 클래스
class Account {
public:
    //각 스테이지의 상태를 정의하기 위한 enum
    //CLEAR - 이미 클리어 함
    //OPEN - 이 전 스테이지는 클리어 했기에 열려있지만 클리어하진 않음
    //LOCK - 잠겨있어서 접근이 안되는 스테이지
    enum {
        CLEAR = 0,
        OPEN,
        LOCK
    };
    
public:
    Account();
    ~Account();
    
    void init(); //Account 클래스로 저장되어있는 플레이 데이터를 가져온다
    void sync(); //Account 클래스의 현재 값으로 플레이 데이터를 저장한다
    void updateStates(); //스테이지의 개방, 클리어 상태를 갱신한다
    
    void setStageState(int category, int idx, int state); //updateStates() 에서 쓰임
    
    bool isUnlockAll(int category); //해당 타입의 스테이지 전체가 잠겨있지 않은지 체크
    void unlockAll(int category); //해당 타입의 스테이지 전체를 잠금 해제함
    
public:
    int level;
    float exp;
    int hint;
    bool isMute;
    int lastlogin; //지난 접속 날짜
    bool isNoAds; //광고 없음을 구매했는지
    
    //스테이지의 클리어 상태에 대한 변수
    //vecStageState에 CLEAR, OPEN, LOCK의 데이터가
    //각 스테이지의 타입별로 vector로 들어간다.
    //총 스테이지는 4개의 타입으로 나뉨
    vector<int> vecStageState[4];
    float percentStage[4];
    int stateCategory[4];
    
    int clearStageNumber;
    
    //광고 출연 타이밍을 위한 변수들
    long ads_time;
    int ads_count;
    
public:
    static Account* getInstance();
    static Account* sharedInstance;
};

#endif /* Account_h */
