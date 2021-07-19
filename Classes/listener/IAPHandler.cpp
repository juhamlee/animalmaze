//
//  IAPs.cpp
//  alphabetmaze-mobile
//
//  Created by 이주함 on 11/06/2019.
//

#include "IAPHandler.h"
#include "Common.h"
#include "PopupManager.h"

IAPHandler* IAPHandler::sharedInstance = nullptr;

IAPHandler* IAPHandler::getInstance() {
    if(sharedInstance == nullptr) {
        sharedInstance = new IAPHandler;
    }
    
    return sharedInstance;
}

void IAPHandler::onInitialized(bool ok) {
    INFO("onInitialized: %s", ok ? "true" : "false");
}
void IAPHandler::onSuccess(sdkbox::Product const& p) {
    INFO("onSuccess: %s", p.name.c_str());
    
    if(p.name == SKU_UNLOCK1) {
        ACCOUNT->unlockAll(0);
        EVENT_DISPATCHER->dispatchCustomEvent(E_RELOAD_SCENE);
    }
    else if(p.name == SKU_UNLOCK2) {
        ACCOUNT->unlockAll(1);
        EVENT_DISPATCHER->dispatchCustomEvent(E_RELOAD_SCENE);
    }
    else if(p.name == SKU_UNLOCK3) {
        ACCOUNT->unlockAll(2);
        EVENT_DISPATCHER->dispatchCustomEvent(E_RELOAD_SCENE);
    }
    else if(p.name == SKU_UNLOCK4) {
        ACCOUNT->unlockAll(3);
        EVENT_DISPATCHER->dispatchCustomEvent(E_RELOAD_SCENE);
    }
    else if(p.name == SKU_NO_ADS) {
        ACCOUNT->isNoAds = true;
        ACCOUNT->sync();
        EVENT_DISPATCHER->dispatchCustomEvent(E_RELOAD_SCENE);
    }
    else if(p.name == SKU_HINT_5) {
        ACCOUNT->hint += 5;
        ACCOUNT->sync();
        EVENT_DISPATCHER->dispatchCustomEvent(E_REFRESH_HINT);
    }
    else if(p.name == SKU_HINT_25) {
        ACCOUNT->hint += 25;
        ACCOUNT->sync();
        EVENT_DISPATCHER->dispatchCustomEvent(E_REFRESH_HINT);
    }
    else if(p.name == SKU_HINT_100) {
        bool prevNoAds = ACCOUNT->isNoAds;
        
        ACCOUNT->hint += 100;
        ACCOUNT->isNoAds = true;
        ACCOUNT->sync();
        EVENT_DISPATCHER->dispatchCustomEvent(E_REFRESH_HINT);
        
        if(prevNoAds == false)
            EVENT_DISPATCHER->dispatchCustomEvent(E_RELOAD_SCENE);
    }
    else if(p.name == SKU_HINT_250) {
        bool prevNoAds = ACCOUNT->isNoAds;
        
        ACCOUNT->hint += 250;
        ACCOUNT->isNoAds = true;
        ACCOUNT->sync();
        EVENT_DISPATCHER->dispatchCustomEvent(E_REFRESH_HINT);
        
        if(prevNoAds == false)
            EVENT_DISPATCHER->dispatchCustomEvent(E_RELOAD_SCENE);
    }
    
    POPUP_MANAGER->closeWaitPopup();
}
void IAPHandler::onFailure(sdkbox::Product const& p, const std::string &msg) {
    INFO("onFailure: %s", msg.c_str());
    INFO("onFailure: %s", p.name.c_str());
    
    POPUP_MANAGER->closeWaitPopup();
}
void IAPHandler::onCanceled(sdkbox::Product const& p) {
    INFO("onCanceled: %s", p.name.c_str());
    
    POPUP_MANAGER->closeWaitPopup();
}
void IAPHandler::onRestored(sdkbox::Product const& p) {
    INFO("onRestored: %s", p.name.c_str());
    
    if(p.name == SKU_UNLOCK1) {
        ACCOUNT->unlockAll(0);
    }
    else if(p.name == SKU_UNLOCK2) {
        ACCOUNT->unlockAll(1);
    }
    else if(p.name == SKU_UNLOCK3) {
        ACCOUNT->unlockAll(2);
    }
    else if(p.name == SKU_UNLOCK4) {
        ACCOUNT->unlockAll(3);
    }
    else if(p.name == SKU_NO_ADS) {
        bool prevNoAds = ACCOUNT->isNoAds;
        
        ACCOUNT->isNoAds = true;
        ACCOUNT->sync();
        
        if(prevNoAds == false)
            EVENT_DISPATCHER->dispatchCustomEvent(E_RELOAD_SCENE);
    }
    else if(p.name == SKU_HINT_100) {
        bool prevNoAds = ACCOUNT->isNoAds;
        
        ACCOUNT->isNoAds = true;
        ACCOUNT->sync();
        
        if(prevNoAds == false)
            EVENT_DISPATCHER->dispatchCustomEvent(E_RELOAD_SCENE);
    }
    else if(p.name == SKU_HINT_250) {
        bool prevNoAds = ACCOUNT->isNoAds;
        
        ACCOUNT->isNoAds = true;
        ACCOUNT->sync();
        
        if(prevNoAds == false)
            EVENT_DISPATCHER->dispatchCustomEvent(E_RELOAD_SCENE);
    }
    
    POPUP_MANAGER->closeWaitPopup();
}
void IAPHandler::onProductRequestSuccess(std::vector<sdkbox::Product> const &products) {
    int size = (unsigned int)products.size();
    for(int i = 0 ;i < size; i++) {
        INFO("products %d: %s", i, products[i].name.c_str());
    }
}
void IAPHandler::onProductRequestFailure(const std::string &msg) {
    INFO("onProductRequestFailure: %s", msg.c_str());
}
void IAPHandler::onRestoreComplete(bool ok, const std::string &msg) {
    INFO("onRestoreComplete: %s", msg.c_str());
}
