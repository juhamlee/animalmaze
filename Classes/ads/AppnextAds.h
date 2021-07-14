//
//  AppnextAds.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 06/03/2019.
//

#ifndef AppnextAds_hpp
#define AppnextAds_hpp

#include <stdio.h>
#include "PluginAppnext/PluginAppnext.h"

class AppnextAds : public sdkbox::AppnextListener
{
private:
    void onAdError(const std::string& msg);
    void onAdLoaded();
    void onAdOpened(); // not support on android
    void onAdClosed();
    void onAdClicked();
    
    void onVideoLoaded(const std::string& name);
    void onVideoClicked(const std::string& name);
    void onVideoClosed(const std::string& name);
    void onVideoEnded(const std::string& name); // not support on ios
    void onVideoError(const std::string& name, const std::string& msg);
    
public:
    static AppnextAds* getInstance();
    static AppnextAds* sharedInstance;
};

#endif /* AppnextAds_hpp */
