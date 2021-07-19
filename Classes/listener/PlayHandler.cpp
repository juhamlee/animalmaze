//
//  PlayHandler.cpp
//  alphabetmaze-mobile
//
//  Created by 이주함 on 11/06/2019.
//

#include "PlayHandler.h"
#include "Common.h"

PlayHandler* PlayHandler::sharedInstance = nullptr;

PlayHandler* PlayHandler::getInstance() {
    if(sharedInstance == nullptr) {
        sharedInstance = new PlayHandler;
    }
    
    return sharedInstance;
}

void PlayHandler::onConnectionStatusChanged(int status) {
     INFO("[PLAY] Status : %d", status);
}
void PlayHandler::onScoreSubmitted(const std::string& leaderboard_name, long score, bool maxScoreAllTime, bool maxScoreWeek, bool maxScoreToday) {}
void PlayHandler::onIncrementalAchievementUnlocked(const std::string& achievement_name) {}
void PlayHandler::onIncrementalAchievementStep(const std::string& achievement_name, double step) {}
void PlayHandler::onAchievementUnlocked(const std::string& achievement_name, bool newlyUnlocked) {}
void PlayHandler::onSaveGameData(bool success, const std::string& error) {}
void PlayHandler::onLoadGameData(const sdkbox::SavedGameData* savedData, const std::string& error) {}
void PlayHandler::onGameDataNames(const std::vector<std::string>& names, const std::string& error) {}
