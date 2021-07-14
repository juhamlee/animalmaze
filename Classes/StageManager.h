//
//  StageManger.hpp
//  animalmaze-mobile
//
//  Created by 이주함 on 24/02/2019.
//

#ifndef StageManager_hpp
#define StageManager_hpp

#include <vector>
#include <string>

using namespace std;

class StageManager {
public:
    enum {
        CATEGORY_CLASSIC = 0,
        CATEGORY_ICE,
        CATEGORY_HURRYUP,
        CATEGORY_DARKNESS,
        
        NUMBER_OF_CATEGORY = 4,
    };
    
    enum {
        TYPE_TILE = 0,
        TYPE_TILE_WORD,
        TYPE_ALPHA
    };
    
    enum {
        DIFF_EASY = 0,
        DIFF_NORM,
        DIFF_HARD,
    };
    
    struct Stage {
        int category;
        int index;
        int type;
        int animal;
        string filepath;
        string imagepath;
        
        int diff;
        int order;
        
        int length;
        string collect;
        string goal;
        
        bool complete;
        int nextAnimal;
        string nameAnimal;
        
        Stage() {}
        Stage(int _category, int _idx, int _type, int _animal, string _file, string _image, int _diff, int _order, int _len, string _col, string _goal, bool _comp, int _next, string _name) {
            category = _category;
            index = _idx;
            type = _type;
            animal = _animal;
            filepath = _file;
            imagepath = _image;
            
            diff = _diff;
            order = _order;
            
            length = _len;
            collect = _col;
            goal = _goal;
            
            complete = _comp;
            nextAnimal = _next;
            nameAnimal = _name;
        }
    };
    
public:
    StageManager();
    ~StageManager();
    
    void init();
    void setCurrentStage(int category, int idx);
public:
    vector<Stage> vecStages[NUMBER_OF_CATEGORY];
    int stageNumbers[NUMBER_OF_CATEGORY];
    
    int totalStageNumber;
    
    Stage currentStage;
    Stage nextStage;
    bool isLastStage;
    
public:
    static StageManager* getInstance();
    static StageManager* sharedInstance;
};

#endif /* StageManager_hpp */
