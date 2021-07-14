//
//  StageManger.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 24/02/2019.
//

#include "StageManager.h"
#include "cocos-ext.h"

StageManager* StageManager::sharedInstance = nullptr;

USING_NS_CC;

StageManager::StageManager() {
    for(int i = 0; i < NUMBER_OF_CATEGORY; i++) {
        vecStages[i].clear();
        stageNumbers[i] = 0;
    }
    totalStageNumber = 0;
}

StageManager::~StageManager() {
}

StageManager* StageManager::getInstance() {
    if(sharedInstance == nullptr) {
        sharedInstance = new StageManager;
        sharedInstance->init();
    }
    
    return sharedInstance;
}

void StageManager::init() {
    static const char* categoryfiles[NUMBER_OF_CATEGORY] = {
        "maze/classic_animal.json",
        "maze/ice.json",
        "maze/hurryup.json",
        "maze/darkness.json"
    };
    
    totalStageNumber = 0;
    for(int i = 0; i < NUMBER_OF_CATEGORY; i++) {
        vecStages[i].clear();
        
        if(i == CATEGORY_CLASSIC) {
            string data = FileUtils::getInstance()->getStringFromFile(categoryfiles[i]);
            rapidjson::Document doc;
            doc.Parse(data.c_str());
            
            if(!doc.HasParseError()) {
                int animal = 0;
                string folder = doc["folder"].GetString();
                string alphabet_folder = doc.HasMember("afolder") ? doc["afolder"].GetString() : "";
                int diff_norm_index = doc.HasMember("normal") ? doc["normal"].GetInt() : 999;
                int diff_hard_index = doc.HasMember("hard") ? doc["hard"].GetInt() : 999;
                int size = doc["stages"].Size();
                for(int j = 0; j < size; j++) {
                    const rapidjson::Value& value = doc["stages"][j];
                    int idx = value["index"].GetInt();
                    int type = value.HasMember("type") ? value["type"].GetInt() : 0;
                    animal = value.HasMember("animal") ? value["animal"].GetInt() : animal;
                    int diff = DIFF_EASY;
                    if(diff_hard_index <= idx)
                        diff = DIFF_HARD;
                    else if(diff_norm_index <= idx)
                        diff = DIFF_NORM;
                    int order = value.HasMember("order") ? value["order"].GetInt() : 0;
                    
                    bool isWordTile = (type == 1) ? true : false;
                    int length = 0;
                    string collect = "";
                    string goal = "";
                    if(isWordTile) {
                        type = TYPE_TILE_WORD;
                        length = value["length"].GetInt();
                        collect = value["collect"].GetString();
                        goal = value["goal"].GetString();
                    }
                    
                    int nextIdx = 0;
                    string name = "";
                    bool isComplete = value.HasMember("complete") ? value["complete"].GetBool() : false;
                    if(isComplete) {
                        nextIdx = value.HasMember("nextAnimal") ? value["nextAnimal"].GetInt() : 0;
                        name = value.HasMember("nameAnimal") ? value["nameAnimal"].GetString() : "";
                    }
                    
                    string alphabet = value.HasMember("alphabet") ? value["alphabet"].GetString() : "";
                    string file = value.HasMember("file") ? value["file"].GetString() : "";
                    string filepath = "";
                    string imagepath = "";
                    if(type == TYPE_ALPHA) {
                        char buf[128];
                        char buf2[128];
                        string diffcode = "easy";
                        if(diff == DIFF_NORM)
                            diffcode = "normal";
                        else if(diff == DIFF_HARD)
                            diffcode = "hard";
                        
                        sprintf(buf, "%s%s/%s_%s_%d.json", alphabet_folder.c_str(), diffcode.c_str(), diffcode.c_str(), alphabet.c_str(), order);
                        sprintf(buf2, "%s%s/%s_%s_%d.png", alphabet_folder.c_str(), diffcode.c_str(), diffcode.c_str(), alphabet.c_str(), order);
                        
                        filepath = buf;
                        imagepath = buf2;
                    }
                    else
                        filepath = folder + file;
                    
                    Stage newStage(i, idx, type, animal, filepath, imagepath, diff, order, length, collect, goal, isComplete, nextIdx, name);
                    vecStages[i].push_back(newStage);
                }
                stageNumbers[i] = size;
                totalStageNumber += size;
            }
        }
        else {
            string data = FileUtils::getInstance()->getStringFromFile(categoryfiles[i]);
            rapidjson::Document doc;
            doc.Parse(data.c_str());
            
            if(!doc.HasParseError()) {
                string folder = doc["folder"].GetString();
                int size = doc["stageMax"].GetInt();
                int animal = doc["animal"].GetInt();
                for(int j = 0; j < size; j++) {
                    int idx = j + 1;
                    int type = TYPE_TILE;
                    
                    char buf[16];
                    sprintf(buf, "%d.json", idx);
                    string filepath = folder + buf;
                    
                    Stage newStage(i, idx, type, animal, filepath, "", DIFF_EASY, 0, 0, "", "", false, 0, "");
                    vecStages[i].push_back(newStage);
                }
                stageNumbers[i] = size;
                totalStageNumber += size;
            }
        }
    }
}

void StageManager::setCurrentStage(int category, int idx) {
    isLastStage = true;
    
    int size = (int)vecStages[category].size();
    for(int i = 0; i < size; i++) {
        if(vecStages[category][i].index == idx) {
            currentStage.index = idx;
            currentStage.category = vecStages[category][i].category;
            currentStage.type = vecStages[category][i].type;
            currentStage.animal = vecStages[category][i].animal;
            currentStage.filepath = vecStages[category][i].filepath;
            currentStage.imagepath = vecStages[category][i].imagepath;
            currentStage.diff = vecStages[category][i].diff;
            currentStage.order = vecStages[category][i].order;
            
            currentStage.length = vecStages[category][i].length;
            currentStage.collect = vecStages[category][i].collect;
            currentStage.goal = vecStages[category][i].goal;
            
            currentStage.complete = vecStages[category][i].complete;
            currentStage.nextAnimal = vecStages[category][i].nextAnimal;
            currentStage.nameAnimal = vecStages[category][i].nameAnimal;
        }
        
        if(vecStages[category][i].index == idx + 1) {
            nextStage.index = idx + 1;
            nextStage.category = vecStages[category][i].category;
            nextStage.type = vecStages[category][i].type;
            nextStage.animal = vecStages[category][i].animal;
            nextStage.filepath = vecStages[category][i].filepath;
            nextStage.imagepath = vecStages[category][i].imagepath;
            nextStage.diff = vecStages[category][i].diff;
            nextStage.order = vecStages[category][i].order;
            
            nextStage.length = vecStages[category][i].length;
            nextStage.collect = vecStages[category][i].collect;
            nextStage.goal = vecStages[category][i].goal;
            
            nextStage.complete = vecStages[category][i].complete;
            nextStage.nextAnimal = vecStages[category][i].nextAnimal;
            nextStage.nameAnimal = vecStages[category][i].nameAnimal;
            
            isLastStage = false;
            
            break;
        }
    }
}


