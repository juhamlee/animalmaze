//
//  AppnextAds.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 06/03/2019.
//

#include "AppnextAds.h"
#include "Common.h"

AppnextAds* AppnextAds::sharedInstance = nullptr;

AppnextAds* AppnextAds::getInstance() {
    if(sharedInstance == nullptr) {
        sharedInstance = new AppnextAds;
    }
    
    return sharedInstance;
}

void AppnextAds::onAdError(const std::string& msg) {}
void AppnextAds::onAdLoaded() {
    EVENT_DISPATCHER->dispatchCustomEvent(E_AD_MAIN_READY);
}
void AppnextAds::onAdOpened() {}
void AppnextAds::onAdClosed() {
    EVENT_DISPATCHER->dispatchCustomEvent(E_AD_MAIN_CLOSE);
}
void AppnextAds::onAdClicked() {}

void AppnextAds::onVideoLoaded(const std::string& name) {}
void AppnextAds::onVideoClicked(const std::string& name) {}
void AppnextAds::onVideoClosed(const std::string& name) {
    if(name == "fullscreen") {
        EVENT_DISPATCHER->dispatchCustomEvent(E_AD_VIDEO_CLOSE);
    }
}
void AppnextAds::onVideoEnded(const std::string& name) {}
void AppnextAds::onVideoError(const std::string& name, const std::string& msg) {}
