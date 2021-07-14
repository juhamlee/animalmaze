//
//  Account.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 05/01/2019.
//

#include "Account.h"
#include "StageManager.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

Account* Account::sharedInstance = nullptr;

USING_NS_CC;

static const char* KEY_OLDUSER = "isOldUser";
static const char* KEY_LEVEL = "level";
static const char* KEY_HINT = "hint";
static const char* KEY_MUTE = "mute";
static const char* KEY_LOGIN = "login";

static const int    MAX_LEVEL = 200;

Account::Account() {
    for(int i = 0; i < 4; i++) {
        vecStageState[i].clear();
        percentStage[i] = 0.f;
        stateCategory[i] = LOCK;
    }
    
    clearStageNumber = 0;
    ads_time = time(NULL);
    ads_count = 0;
    exp = 0.f;
}

Account::~Account() {
    for(int i = 0; i < 4; i++) {
        vecStageState[i].clear();
        percentStage[i] = 0.f;
        stateCategory[i] = LOCK;
    }
}

Account* Account::getInstance() {
    if(sharedInstance == nullptr) {
        sharedInstance = new Account;
        sharedInstance->init();
    }
    
    return sharedInstance;
}

void Account::init() {
    bool isOldUser = UserDefault::getInstance()->getBoolForKey(KEY_OLDUSER);
    if(isOldUser == false) {
        level = 1;
        hint = 2;
        isMute = false;
        
        time_t t = time(NULL);
        struct tm* t2 = localtime(&t);
        lastlogin = t2->tm_mday;
        
        char buf[32];
        auto stageMgr = StageManager::getInstance();
        for(int i = 0; i < 4; i++) {
            int size = stageMgr->stageNumbers[i];
            for(int j = 0; j < size; j++) {
                sprintf(buf, "category_%d_state_%d", i + 1, j + 1);
                
                if(j == 0)
                    vecStageState[i].push_back(OPEN);
                else
                    vecStageState[i].push_back(LOCK);
            }
            percentStage[i] = 0.f;
            if(i == 0) stateCategory[i] = OPEN;
            else stateCategory[i] = LOCK;
        }
        
        sync();
    }
    else {
        time_t t = time(NULL);
        struct tm* t2 = localtime(&t);
        int today = t2->tm_mday;
        
        level = UserDefault::getInstance()->getIntegerForKey(KEY_LEVEL, 1);
        hint = UserDefault::getInstance()->getIntegerForKey(KEY_HINT, 2);
        isMute = UserDefault::getInstance()->getBoolForKey(KEY_MUTE, false);
        lastlogin = UserDefault::getInstance()->getIntegerForKey(KEY_LOGIN, today);
        
        char buf[32];
        auto stageMgr = StageManager::getInstance();
        for(int i = 0; i < 4; i++) {
            int size = stageMgr->stageNumbers[i];
            for(int j = 0; j < size; j++) {
                sprintf(buf, "category_%d_state_%d", i + 1, j + 1);
                
                int state = UserDefault::getInstance()->getIntegerForKey(buf, LOCK);
                if(j == 0) {
                    if(state == CLEAR)
                        vecStageState[i].push_back(CLEAR);
                    else
                        vecStageState[i].push_back(OPEN);
                }
                else
                    vecStageState[i].push_back(state);
            }
        }
        
        updateStates();
    }
}

void Account::sync() {
    UserDefault::getInstance()->setBoolForKey(KEY_OLDUSER, true);
    UserDefault::getInstance()->setIntegerForKey(KEY_LEVEL, level);
    UserDefault::getInstance()->setIntegerForKey(KEY_HINT, hint);
    UserDefault::getInstance()->setBoolForKey(KEY_MUTE, isMute);
    UserDefault::getInstance()->setIntegerForKey(KEY_LOGIN, lastlogin);
    
    char buf[32];
    auto stageMgr = StageManager::getInstance();
    for(int i = 0; i < 4; i++) {
        int size = stageMgr->stageNumbers[i];
        for(int j = 0; j < size; j++) {
            sprintf(buf, "category_%d_state_%d", i + 1, j + 1);
            int state = vecStageState[i][j];
            UserDefault::getInstance()->setIntegerForKey(buf, state);
        }
    }
    
    updateStates();
}

void Account::updateStates() {
    int classicCount = 0;
    clearStageNumber = 0;
    for(int i = 0; i < 4; i++) {
        int count = 0;
        int size = (int)vecStageState[i].size();
        for(int j = 0; j < size; j++) {
            int state = vecStageState[i][j];
            if(state == CLEAR) count++;
        }
        percentStage[i] = (float)count / (float)size * 100.f;
        if(i == 0) {
            classicCount = count;
            stateCategory[i] = OPEN;
        }
        else if(i == 1) stateCategory[i] = 13 <= classicCount ? OPEN : LOCK;
        else if(i == 2) stateCategory[i] = 195 <= classicCount ? OPEN : LOCK;
        else if(i == 3) stateCategory[i] = 290 <= classicCount ? OPEN : LOCK;
        
        clearStageNumber += count;
    }
    
    float clear = clearStageNumber;
    float total = StageManager::getInstance()->totalStageNumber;
    exp = clear / total * (float)MAX_LEVEL;
    level = (int)exp;
    exp = exp - level;
}

void Account::setStageState(int category, int idx, int state) {
    if(vecStageState[category][idx] < state)
        return;
    
    vecStageState[category][idx] = state;
}

void Account::unlockAll(int category) {
    if(4 <= category)
        return;
    
    int size = (unsigned int)vecStageState[category].size();
    for(int i = 0; i < size; i++) {
        vecStageState[category][i] = CLEAR;
    }
    
    sync();
}
