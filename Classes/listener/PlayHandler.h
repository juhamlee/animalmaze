//
//  PlayHandler.hpp
//  alphabetmaze-mobile
//
//  Created by 이주함 on 11/06/2019.
//

#ifndef PlayHandler_hpp
#define PlayHandler_hpp

#include <stdio.h>
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"

class PlayHandler : public sdkbox::SdkboxPlayListener
{
private:
    virtual void onConnectionStatusChanged(int status) override;
    virtual void onScoreSubmitted(const std::string& leaderboard_name, long score, bool maxScoreAllTime, bool maxScoreWeek, bool maxScoreToday) override;
    virtual void onIncrementalAchievementUnlocked(const std::string& achievement_name) override;
    virtual void onIncrementalAchievementStep(const std::string& achievement_name, double step) override;
    virtual void onAchievementUnlocked(const std::string& achievement_name, bool newlyUnlocked) override;
    virtual void onSaveGameData(bool success, const std::string& error) override;
    virtual void onLoadGameData(const sdkbox::SavedGameData* savedData, const std::string& error) override;
    virtual void onGameDataNames(const std::vector<std::string>& names, const std::string& error) override;
    
public:
    static PlayHandler* getInstance();
    static PlayHandler* sharedInstance;
};

#endif /* PlayHandler_hpp */
