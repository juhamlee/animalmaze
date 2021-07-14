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

class Account {
public:
    enum {
        CLEAR = 0,
        OPEN,
        LOCK
    };
    
public:
    Account();
    ~Account();
    
    void init();
    void sync();
    void updateStates();
    
    void setStageState(int category, int idx, int state);
    
    void unlockAll(int category);
    
public:
    int level;
    float exp;
    int hint;
    bool isMute;
    int lastlogin;
    
    vector<int> vecStageState[4];
    float percentStage[4];
    int stateCategory[4];
    
    int clearStageNumber;
    
    long ads_time;
    int ads_count;
    
public:
    static Account* getInstance();
    static Account* sharedInstance;
};

#endif /* Account_h */
