//
//  MazeLayer.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 05/01/2019.
//

#include "MazeLayer.h"
#include "MazeAlphaLayer.h"
#include "PopupManager.h"
#include "ClearPopup.h"
#include "ShopPopup.h"
#include "PausePopup.h"
#include "HintPopup.h"
#include "TimeupPopup.h"
#include "CollectPopup.h"

#include "PluginAdMob/PluginAdMob.h"
#include "PluginAppnext/PluginAppnext.h"

static const int FONT_SIZE_HINT = 28;
static const int FONT_SIZE_STAGE = 50;
static const int FONT_SIZE_TIME = 70;
static const int FONT_SIZE_WORD = 40;

static const float SPEED_FALL = 0.1f;
static const float SPEED_MOVE = 0.1f;
static const float SPEED_GHOST = 450.f;

static const int BG_SIZE = 7;
static const int BG_OFFSETS[] = {
    1280,
    1218,
    912,
    591,
    340,
    169,
    147
};

static const Color4B BG_COLORS[6][7] = {
    {
        Color4B(61, 132, 198, 255),
        Color4B(75, 148, 216, 255),
        Color4B(93, 163, 227, 255),
        Color4B(109, 173, 233, 255),
        Color4B(124, 182, 237, 255),
        Color4B(192, 207, 221, 255),
        Color4B(199, 162, 119, 255)
    },
    {
        Color4B(207, 117, 168, 255),
        Color4B(220, 128, 180, 255),
        Color4B(228, 141, 191, 255),
        Color4B(233, 152, 197, 255),
        Color4B(237, 163, 205, 255),
        Color4B(228, 209, 219, 255),
        Color4B(156, 194, 176, 255)
    },
    {
        Color4B(150, 192, 103, 255),
        Color4B(160, 204, 114, 255),
        Color4B(171, 212, 127, 255),
        Color4B(179, 218, 138, 255),
        Color4B(185, 221, 148, 255),
        Color4B(220, 232, 207, 255),
        Color4B(220, 185, 118, 255)
    },
    {
        Color4B(117, 95, 207, 255),
        Color4B(129, 106, 222, 255),
        Color4B(141, 121, 231, 255),
        Color4B(156, 135, 236, 255),
        Color4B(166, 147, 239, 255),
        Color4B(186, 177, 253, 255),
        Color4B(154, 221, 220, 255)
    },
    {
        Color4B(222, 176, 79, 255),
        Color4B(232, 191, 92, 255),
        Color4B(238, 203, 113, 255),
        Color4B(241, 211, 131, 255),
        Color4B(242, 217, 149, 255),
        Color4B(247, 239, 226, 255),
        Color4B(240, 151, 86, 255)
    },
    {
        Color4B(42, 50, 97, 255),
        Color4B(51, 60, 116, 255),
        Color4B(59, 68, 129, 255),
        Color4B(67, 76, 135, 255),
        Color4B(75, 83, 139, 255),
        Color4B(86, 94, 148, 255),
        Color4B(57, 67, 125, 255)
    }
};

static const Color3B PATH_COLOR = Color3B(180, 180, 180);
static const Color3B HINT_COLOR = Color3B(220, 70, 70);

/*
static const Color3B GOAL_COLORS[4] = {
    Color3B(35, 69, 0),
    Color3B(5, 59, 109),
    Color3B(154, 25, 98),
    Color3B(46, 25, 133)
};

static const Color4B TRACE_COLORS[4] = {
    Color4B(35, 69, 0, 255),
    Color4B(5, 59, 109, 255),
    Color4B(154, 25, 98, 255),
    Color4B(46, 25, 133, 255)
};

static const Color4B HINT_COLORS[4] = {
    Color4B(91, 116, 64, 255),
    Color4B(68, 108, 146, 255),
    Color4B(180, 83, 138, 255),
    Color4B(99, 83, 164, 255)
};
 */

static const char* CATEGORY_NAMES[] = {
    "CLASSIC",
    "ICE",
    "HURRY UP",
    "DARKNESS"
};

Scene* MazeLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = MazeLayer::create();
    
    if(scene != nullptr && layer != nullptr) {
        scene->addChild(layer);
        
        PopupManager::getInstance()->initWithBaseNode(layer);
        
        return scene;
    }
    
    return nullptr;
}

MazeLayer::MazeLayer() {
    AUDIO->preloadBackgroundMusic("sfx/bgm01.mp3");
    AUDIO->preloadBackgroundMusic("sfx/bgm02.mp3");
    
    AUDIO->preloadEffect("sfx/clear.mp3");
    AUDIO->preloadEffect("sfx/move.mp3");
    
    moveSoundId = -1;
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

MazeLayer::~MazeLayer() {
    AUDIO->stopBackgroundMusic();
    AUDIO->stopAllEffects();
    
    AUDIO->unloadEffect("sfx/clear.mp3");
    AUDIO->unloadEffect("sfx/move.mp3");
}

bool MazeLayer::init()
{
    Layer::init();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    int category = STAGE_MANAGER->currentStage.category;
    int animal = STAGE_MANAGER->currentStage.animal;
    
    static const int arrColorAnimal[6][8] = {
        {3, 6, 12, 13, 22, 29, 30, -1},
        {11, 18, 21, 25, 26, 28, -1, -1},
        {1, 8, 10, 19, 23, -1, -1, -1},
        {4, 7, 14, 16, 27, 31, -1, -1},
        {2, 5, 9, 15, 17, 20, 24, -1},
        {32, -1, -1, -1, -1, -1, -1, -1}
    };
    
    int coloridx = 0;
    for(int i = 0; i < 6; i++) {
        bool isFind = false;
        for(int j = 0; j < 8; j++) {
            if(arrColorAnimal[i][j] == -1)
                break;
            
            if(animal == arrColorAnimal[i][j]) {
                coloridx = i;
                isFind = true;
                break;
            }
        }
        if(isFind == true)
            break;
    }
    
    for(int i = 0; i < BG_SIZE; i++) {
        Color4B color = BG_COLORS[coloridx][i];
        
        auto layerBG = LayerColor::create(color, 720, BG_OFFSETS[i]);
        if(layerBG != nullptr) {
            this->addChild(layerBG, Z_BACK);
        }
    }
    
    char buf[30];
    sprintf(buf, "%s-%d", CATEGORY_NAMES[category], STAGE_MANAGER->currentStage.index);
    auto labStage = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_STAGE);
    if(labStage != nullptr) {
        labStage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labStage->setPosition(20, 1217);
        this->addChild(labStage, Z_UI);
    }
    
    if(category == StageManager::CATEGORY_HURRYUP) {
        labTime = Label::createWithTTF("", FONT_PATH, FONT_SIZE_TIME);
        if(labTime != nullptr) {
            labTime->setPosition(center.x, 1219);
            this->addChild(labTime, Z_UI);
        }
    }
    else labTime = nullptr;
    
    auto btnPause = MenuItemImage::create("ui/btn_small.png",
                                          "ui/btn_small.png");
    if(btnPause != nullptr) {
        btnPause->setScaleY(-1.f);
        btnPause->setOpacity(128);
        btnPause->setCallback(CC_CALLBACK_1(MazeLayer::callbackPause, this));
        btnPause->setPosition(490, 1220);
        
        auto sprIcon = Sprite::create("ui/icon_pause.png");
        if(sprIcon != nullptr) {
            sprIcon->setPosition(btnPause->getPosition() + Vec2(0, -8));
            this->addChild(sprIcon, Z_BUTTON_OVER);
        }
    }
    
    auto btnRetry = MenuItemImage::create("ui/btn_small.png",
                                          "ui/btn_small.png");
    if(btnRetry != nullptr) {
        btnRetry->setScaleY(-1.f);
        btnRetry->setOpacity(128);
        btnRetry->setCallback(CC_CALLBACK_1(MazeLayer::callbackRetry, this));
        btnRetry->setPosition(584, 1220);
        
        auto sprIcon = Sprite::create("ui/icon_retry.png");
        if(sprIcon != nullptr) {
            sprIcon->setPosition(btnRetry->getPosition() + Vec2(0, -8));
            this->addChild(sprIcon, Z_BUTTON_OVER);
        }
    }
    
    auto btnHint = MenuItemImage::create("ui/icon_hint.png",
                                         "ui/icon_hint.png");
    if(btnHint != nullptr) {
        btnHint->setCallback(CC_CALLBACK_1(MazeLayer::callbackHint, this));
        btnHint->setPosition(671, 1213);
        
        char buf[30];
        sprintf(buf, "%d", ACCOUNT->hint);
        labHint = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_HINT);
        if(labHint != nullptr) {
            labHint->setPosition(Vec2(btnHint->getContentSize() * 0.5f) + Vec2(0, 7));
            labHint->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            btnHint->addChild(labHint);
        }
    }
    
    Menu* menu = Menu::create(btnPause, btnRetry, btnHint, NULL);
    if(menu != nullptr) {
        menu->setPosition(0, 0);
        this->addChild(menu, Z_BUTTON);
    }
    
    if(STAGE_MANAGER->currentStage.type == StageManager::TYPE_TILE_WORD) {
        auto nodeWord = Node::create();
        if(nodeWord != nullptr) {
            nodeWord->setPosition(360, 1130);
            this->addChild(nodeWord, Z_UI);
            
            int length = STAGE_MANAGER->currentStage.length;
            string collect = STAGE_MANAGER->currentStage.collect;
            
            int offset = (length * 46 + (length - 1) * 35) * -0.5 + 23;
            for(int i = 0; i < length; i++) {
                auto nodeLetter = Node::create();
                if(nodeLetter != nullptr) {
                    nodeLetter->setPosition(offset, 0);
                    nodeWord->addChild(nodeLetter);
                    
                    auto sprBack = Sprite::create("game/word_back.png");
                    if(sprBack != nullptr) {
                        sprBack->setColor(Color3B::WHITE);
                        sprBack->setOpacity(128);
                        nodeLetter->addChild(sprBack);
                    
                        if(i < collect.size()) {
                            char buf[8];
                            sprintf(buf, "%c", collect[i]);
                            auto labLetter = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_WORD);
                            if(labLetter != nullptr) {
                                labLetter->setPosition(sprBack->getPosition() + Vec2(0, 2));
                                nodeLetter->addChild(labLetter);
                            }
                            
                            if(i == collect.size() - 1) {
                                auto blink = Blink::create(1, 1);
                                nodeLetter->runAction(RepeatForever::create(blink));
                            }
                        }
                    }
                }
                offset += 40;
                
                if(i < length - 1) {
                    auto sprLine = Sprite::create("game/dot_line.png");
                    if(sprLine != nullptr) {
                        sprLine->setPosition(offset, 0);
                        nodeWord->addChild(sprLine);
                    }
                    offset += 40;
                }
            }
        }
    }
        
    
    state = READY;
    isTouch = false;
    isFollow = false;
    isHintUsed = false;

    vecTrace.clear();
    vecTracePosition.clear();
    vecHint.clear();
    
    return true;
}

void MazeLayer::onEnter() {
    Layer::onEnter();
    
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(MazeLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MazeLayer::onTouchMoved, this);
    
    customListener = EventListenerCustom::create(E_USE_HINT, CC_CALLBACK_1(MazeLayer::onUseHint, this));
    finishListener = EventListenerCustom::create(E_AD_VIDEO_CLOSE, CC_CALLBACK_1(MazeLayer::onFinish, this));
    
    
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(listener, this);
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(customListener, this);
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(finishListener, this);
    
    string str = FileUtils::getInstance()->getStringFromFile(STAGE_MANAGER->currentStage.filepath);
    rapidjson::Document doc;
    doc.Parse(str.c_str());
    if(!doc.HasParseError()) {
        mapSize.width = doc["size"][0].GetInt();
        mapSize.height = doc["size"][1].GetInt();
        
        fScale = MAZE_WIDTH / (mapSize.width * 100.f);
        vScale = Vec2(fScale, MAZE_HEIGHT / (mapSize.height * 100.f));
        
        for(int i = 0; i < mapSize.height; i++) {
            vector<int> vec;
            map.push_back(vec);
        }
        
        for(int i = 0; i < mapSize.height; i++) {
            for(int j = 0; j < mapSize.width; j++) {
                const rapidjson::Value& value = doc["maze"];
                int data = value[i * mapSize.width + j].GetInt();
                map[i].push_back(data);
            }
        }
        
        if(doc.HasMember("time"))
            fTime = doc["time"].GetInt();
        else
            fTime = 0.f;
        
        ptStart.x = doc["start"][0].GetInt();
        ptStart.y = doc["start"][1].GetInt();
        ptEnd.x = doc["end"][0].GetInt();
        ptEnd.y = doc["end"][1].GetInt();
        
        ptCurrent = ptStart;
        ptLast = ptCurrent;
        
        if(doc.HasMember("hint")) {
            int size = (int)doc["hint"].Size();
            for(int i = 0; i < size; i++) {
                Point pt;
                pt.x = doc["hint"][i][0].GetInt();
                pt.y = doc["hint"][i][1].GetInt();
                vecHint.push_back(getPositionFromPoint(pt));
            }
        }
    }
    
    auto nodeMaze = Node::create();
    nodeMaze->setPosition(10, 190);
    this->addChild(nodeMaze, Z_MAZE);
    
    //ICE
    for(int i = 0; i < mapSize.height; i++) {
        for(int j = 0; j < mapSize.width; j++) {
            float offset_x = MAZE_WIDTH / mapSize.width;
            float offset_y = MAZE_HEIGHT / mapSize.height;
            
            auto nodeWall = Node::create();
            if(nodeWall != nullptr) {
                Vec2 pos = Vec2((float)j * offset_x, (float)i * offset_y);
                nodeWall->setPosition(j * offset_x, 1280);
                nodeWall->runAction(Sequence::create(DelayTime::create(SPEED_FALL * fScale * i), MoveTo::create(SPEED_FALL * fScale, pos), NULL));
                nodeMaze->addChild(nodeWall);
                
                if(map[i][j] & DIR_ICE){
                    auto sprIce = Sprite::create("game/ice.png");
                    if(sprIce != nullptr) {
                        sprIce->setScale(vScale.x, vScale.y);
                        sprIce->setPosition(Vec2(offset_x, offset_y) * 0.5f);
                        nodeWall->addChild(sprIce);
                    }
                }
            }
        }
    }
            
    for(int i = 0; i < mapSize.height; i++) {
        for(int j = 0; j < mapSize.width; j++) {
            float offset_x = MAZE_WIDTH / mapSize.width;
            float offset_y = MAZE_HEIGHT / mapSize.height;
            
            auto nodeWall = Node::create();
            if(nodeWall != nullptr) {
                nodeWall->setPosition(j * offset_x, 1280);
                nodeWall->runAction(Sequence::create(DelayTime::create(SPEED_FALL * fScale * i), MoveTo::create(SPEED_FALL * fScale, Vec2(j * offset_x, i * offset_y)), NULL));
                nodeMaze->addChild(nodeWall);
                
                if(j == 0 && (map[i][j] & DIR_L)) {
                    auto sprWall = Sprite::create("game/wall_l.png");
                    if(sprWall != nullptr) {
                        sprWall->setScale(vScale.x, vScale.y);
                        sprWall->setPosition(Vec2(offset_x, offset_y) * 0.5f);
                        nodeWall->addChild(sprWall);
                    }
                }
                if(i == 0 && (map[i][j] & DIR_D)) {
                    auto sprWall = Sprite::create("game/wall_d.png");
                    if(sprWall != nullptr) {
                        sprWall->setScale(vScale.x, vScale.y);
                        sprWall->setPosition(Vec2(offset_x, offset_y) * 0.5f);
                        nodeWall->addChild(sprWall);
                    }
                }
                if(map[i][j] & DIR_R) {
                    auto sprWall = Sprite::create("game/wall_r.png");
                    if(sprWall != nullptr) {
                        sprWall->setScale(vScale.x, vScale.y);
                        sprWall->setPosition(Vec2(offset_x, offset_y) * 0.5f);
                        nodeWall->addChild(sprWall);
                    }
                }
                if(map[i][j] & DIR_U) {
                    auto sprWall = Sprite::create("game/wall_u.png");
                    if(sprWall != nullptr) {
                        sprWall->setScale(vScale.x, vScale.y);
                        sprWall->setPosition(Vec2(offset_x, offset_y) * 0.5f);
                        nodeWall->addChild(sprWall);
                    }
                }
            }
        }
    }
    
    this->runAction(Sequence::create(DelayTime::create(SPEED_FALL * fScale * mapSize.height), CallFunc::create(CC_CALLBACK_0(MazeLayer::start, this)), NULL));
    
    int category = STAGE_MANAGER->currentStage.category;
    
    pathTrace = PathLayer::create();
    if(pathTrace != nullptr) {
        pathTrace->setColor(PATH_COLOR);
        nodeMaze->addChild(pathTrace);
    }
    pathHint = PathLayer::create();
    if(pathHint != nullptr) {
        pathHint->setVisible(false);
        pathHint->setColor(HINT_COLOR);
        pathHint->setPoints(vecHint, 1500.f);
        nodeMaze->addChild(pathHint);
    }
    
    if(STAGE_MANAGER->currentStage.type == StageManager::TYPE_TILE_WORD) {
        auto sprBack = Sprite::create("game/word_back.png");
        if(sprBack != nullptr) {
            sprBack->setColor(Color3B::WHITE);
            sprBack->setOpacity(128);
            sprBack->setScale(fScale);
            sprBack->setPosition(getPositionFromPoint(ptEnd));
            nodeMaze->addChild(sprBack);
            
            string strGoal = STAGE_MANAGER->currentStage.goal;
            auto labGoal = Label::createWithTTF(strGoal.c_str(), FONT_PATH, FONT_SIZE_WORD);
            if(labGoal != nullptr) {
                labGoal->setScale(fScale);
                labGoal->setPosition(sprBack->getPosition() + Vec2(0, 3) * fScale);
                nodeMaze->addChild(labGoal);
            }
        }
    }
    else {
        auto sprGoal = Sprite::create("game/goal.png");
        if(sprGoal != nullptr) {
            sprGoal->setPosition(getPositionFromPoint(ptEnd));
            sprGoal->setColor(Color3B::WHITE);
            sprGoal->setScale(fScale);
            nodeMaze->addChild(sprGoal);
        }
    }
    
    player = Animal::create();
    if(player != nullptr) {
        player->setAnimalByNumber(STAGE_MANAGER->currentStage.animal);
        player->setPosition(getPositionFromPoint(ptCurrent));
        player->setScale(fScale);
        nodeMaze->addChild(player);
        
        if(category == StageManager::CATEGORY_DARKNESS)
            player->setDarkness(true);
        
        if(STAGE_MANAGER->currentStage.animal == 0)
            player->sprCharacter->setColor(Color3B(255, 240, 0));
    }
    
    follower = Animal::create();
    if(follower != nullptr) {
        follower->setAnimalByNumber(0);
        follower->setVisible(false);
        follower->setPosition(getPositionFromPoint(ptCurrent));
        follower->setScale(fScale);
        nodeMaze->addChild(follower);
    }
    
    //HURRY UP
    if(labTime != nullptr && 0.f < fTime) {
        char buf[16];
        sprintf(buf, "%d", (int)fTime);
        
        labTime->setString(buf);
    }
    
    vecTrace.push_back(ptStart);
    vecTracePosition.push_back(getPositionFromPoint(ptStart));
    pathTrace->pushPoint(getPositionFromPoint(ptStart));
    pathTrace->setExtraPoint(getPositionFromPoint(ptCurrent));
    
    makeArrow(true);
    
    AUDIO->playEffect("sfx/begin.mp3");
    
    scheduleUpdate();
    
    if(sdkbox::PluginAdMob::isAvailable("home"))
        sdkbox::PluginAdMob::show("home");
    else
        sdkbox::PluginAdMob::cache("home");
}

void MazeLayer::onExit() {
    EVENT_DISPATCHER->removeEventListener(listener);
    EVENT_DISPATCHER->removeEventListener(customListener);
    EVENT_DISPATCHER->removeEventListener(finishListener);
    
    Layer::onExit();
}

bool MazeLayer::onTouchBegan(Touch* touch, Event* event) {
    if(state == IDLE) {
        isTouch = true;
        auto point = touch->getLocation();
        touchOrigin = point;
        
        return true;
    }
    
    return false;
}

void MazeLayer::onTouchMoved(Touch* touch, Event* event) {
    if(state == IDLE && isTouch == true) {
        auto point = touch->getLocation();
        Vec2 move = point - touchOrigin;
        float length = move.length();
        
        if(7.f < length) {
            int xlen = fabs(move.x);
            int ylen = fabs(move.y);
            
            bool bMoving = false;
            
            if(xlen < ylen) {
                if(0 < move.y) {
                    if(!(getMapFromPoint(ptCurrent) & DIR_U)) {
                        moveUp();
                        bMoving = true;
                    }
                }
                else {
                    if(!(getMapFromPoint(ptCurrent) & DIR_D)) {
                        moveDown();
                        bMoving = true;
                    }
                }
            }
            else {
                if(0 < move.x) {
                    if(!(getMapFromPoint(ptCurrent)& DIR_R)) {
                        moveRight();
                        bMoving = true;
                    }
                }
                else {
                    if(!(getMapFromPoint(ptCurrent) & DIR_L)) {
                        moveLeft();
                        bMoving = true;
                    }
                }
            }
            
            if(bMoving && moveSoundId == -1)
                moveSoundId = AUDIO->playEffect("sfx/move.mp3", true);
        }
    }
}

void MazeLayer::update(float dt) {
    if(state == FINISH || state == READY)
        return;
    
    if(state == MOVE) {
        pathTrace->setExtraPoint(player->getPosition());
    }
    
    if(follower && player) {
        Vec2 fPos = follower->getPosition();
        Vec2 pPos = player->getPosition();
        float dist = fPos.distance(pPos);
        if(110.f * fScale < dist)
            isFollow = true;
        else if(dist <= 90 * fScale)
            isFollow = false;
        
        if(isFollow) {
            Vec2 direction = pPos - fPos;
            direction.normalize();
            follower->setPosition(fPos + direction * dt * fScale * SPEED_GHOST);
        }
    }
    
    //HURRY UP
    if(labTime != nullptr && POPUP_MANAGER->isEmpty()) {
        fTime = MAX(fTime - dt, 0);
        char buf[16];
        sprintf(buf, "%d", (int)fTime);
        
        labTime->setString(buf);
        
        if(fTime == 0) {
            player->stopAllActions();
            state = FINISH;
            
            auto popup = TimeupPopup::create();
            if(popup != nullptr) {
                PopupManager::getInstance()->addPopup(popup);
            }
            
            if(moveSoundId != -1) {
                AUDIO->stopEffect(moveSoundId);
                moveSoundId = -1;
            }
        }
    }
}

void MazeLayer::onUseHint(EventCustom* event) {
    int hint = ACCOUNT->hint;
    if(0 < hint && isHintUsed == false) {
        isHintUsed = true;
        
        ACCOUNT->hint = hint - 1;
        ACCOUNT->sync();
        
        int category = STAGE_MANAGER->currentStage.category;
        if(category == StageManager::CATEGORY_CLASSIC || category == StageManager::CATEGORY_ICE) {
            pathHint->setVisible(true);
        }
        else if(category == StageManager::CATEGORY_HURRYUP) {
            //hurry up test
            fTime += 5.f;
        }
        else if(category == StageManager::CATEGORY_DARKNESS) {
            if(player != nullptr)
                player->setLightScale(2.f);
        }
        
        char buf[16];
        sprintf(buf, "%d", hint - 1);
        labHint->setString(buf);
    }
}

void MazeLayer::onFinish(EventCustom* event) {
    AUDIO->resumeBackgroundMusic();
    scheduleOnce(schedule_selector(MazeLayer::finish), 0.1f);
}

void MazeLayer::callbackPause(Ref* pSender) {
    auto popup = PausePopup::create();
    if(popup != nullptr) {
        PopupManager::getInstance()->addPopup(popup);
    }
    
    AUDIO->playEffect("sfx/click.mp3");
}

void MazeLayer::callbackRetry(Ref* pSender) {
    auto scene = MazeLayer::createScene();
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/click.mp3");
}

void MazeLayer::callbackHint(Ref* pSender) {
    auto popup = HintPopup::create();
    if(popup != nullptr) {
        PopupManager::getInstance()->addPopup(popup);
    }
    
    AUDIO->playEffect("sfx/click.mp3");
}

void MazeLayer::start() {
    state = IDLE;
    
    srand((unsigned int)time(NULL));
    int r = rand() % 2 + 1;
    char buf[32];
    sprintf(buf, "sfx/bgm%02d.mp3", r);
    AUDIO->playBackgroundMusic(buf, true);
}

void MazeLayer::moveUp() {
    state = MOVE;
    isTouch = false;
    ptLast = ptCurrent;
    ptCurrent.y++;
    
    if(2 <= vecTrace.size()) {
        int size = (int)vecTrace.size();
        if(vecTrace[size - 2] == ptCurrent) {
            vecTrace.pop_back();
            vecTracePosition.pop_back();
            pathTrace->popPoint();
            pathTrace->setExtraPoint(getPositionFromPoint(ptLast));
        }
    }
    
    player->hideArrowAll();
    lastDirection = DIR_U;
    
    float delay = SPEED_MOVE * fScale;
    float dleay2 = delay + 0.001f;
    MoveTo* move = MoveTo::create(delay, getPositionFromPoint(ptCurrent));
    move->setTag(TAG_MOVE);
    player->stopActionByTag(TAG_MOVE);
    player->setPosition(getPositionFromPoint(ptLast));
    player->runAction(move);
    
    this->runAction(Sequence::create(DelayTime::create(dleay2), CallFunc::create(CC_CALLBACK_0(MazeLayer::check, this)), NULL));
}

void MazeLayer::moveDown() {
    state = MOVE;
    isTouch = false;
    ptLast = ptCurrent;
    ptCurrent.y--;
    
    if(2 <= vecTrace.size()) {
        int size = (int)vecTrace.size();
        if(vecTrace[size - 2] == ptCurrent) {
            vecTrace.pop_back();
            vecTracePosition.pop_back();
            pathTrace->popPoint();
            pathTrace->setExtraPoint(getPositionFromPoint(ptLast));
        }
    }
    
    player->hideArrowAll();
    lastDirection = DIR_D;
    
    float delay = SPEED_MOVE * fScale;
    float dleay2 = delay + 0.001f;
    MoveTo* move = MoveTo::create(delay, getPositionFromPoint(ptCurrent));
    move->setTag(TAG_MOVE);
    player->stopActionByTag(TAG_MOVE);
    player->setPosition(getPositionFromPoint(ptLast));
    player->runAction(move);
    
    this->runAction(Sequence::create(DelayTime::create(dleay2), CallFunc::create(CC_CALLBACK_0(MazeLayer::check, this)), NULL));
}

void MazeLayer::moveLeft() {
    state = MOVE;
    isTouch = false;
    ptLast = ptCurrent;
    ptCurrent.x--;
    
    if(2 <= vecTrace.size()) {
        int size = (int)vecTrace.size();
        if(vecTrace[size - 2] == ptCurrent) {
            vecTrace.pop_back();
            vecTracePosition.pop_back();
            pathTrace->popPoint();
            pathTrace->setExtraPoint(getPositionFromPoint(ptLast));
        }
    }
    
    player->hideArrowAll();
    lastDirection = DIR_L;
    
    float delay = SPEED_MOVE * fScale;
    float dleay2 = delay + 0.001f;
    MoveTo* move = MoveTo::create(delay, getPositionFromPoint(ptCurrent));
    move->setTag(TAG_MOVE);
    player->stopActionByTag(TAG_MOVE);
    player->setPosition(getPositionFromPoint(ptLast));
    player->runAction(move);
    player->setFlip(true);
    
    this->runAction(Sequence::create(DelayTime::create(dleay2), CallFunc::create(CC_CALLBACK_0(MazeLayer::check, this)), NULL));
}

void MazeLayer::moveRight() {
    state = MOVE;
    isTouch = false;
    ptLast = ptCurrent;
    ptCurrent.x++;
    
    if(2 <= vecTrace.size()) {
        int size = (int)vecTrace.size();
        if(vecTrace[size - 2] == ptCurrent) {
            vecTrace.pop_back();
            vecTracePosition.pop_back();
            pathTrace->popPoint();
            pathTrace->setExtraPoint(getPositionFromPoint(ptLast));
        }
    }
    
    player->hideArrowAll();
    lastDirection = DIR_R;
    
    float delay = SPEED_MOVE * fScale;
    float dleay2 = delay + 0.001f;
    MoveTo* move = MoveTo::create(delay, getPositionFromPoint(ptCurrent));
    move->setTag(TAG_MOVE);
    player->stopActionByTag(TAG_MOVE);
    player->setPosition(getPositionFromPoint(ptLast));
    player->runAction(move);
    player->setFlip(false);
    
    this->runAction(Sequence::create(DelayTime::create(dleay2), CallFunc::create(CC_CALLBACK_0(MazeLayer::check, this)), NULL));
}

void MazeLayer::check() {
    if(state == FINISH)
        return;
    
    int size = (int)vecTrace.size();
    if(0 < size && vecTrace[size - 1] == ptCurrent) {
        pathTrace->setExtraPoint(getPositionFromPoint(ptCurrent));
    }
    else {
        vecTrace.push_back(ptCurrent);
        vecTracePosition.push_back(getPositionFromPoint(ptCurrent));
        pathTrace->pushPoint(getPositionFromPoint(ptCurrent));
        pathTrace->setExtraPoint(getPositionFromPoint(ptCurrent));
    }
    
    if(getMapFromPoint(ptCurrent) & DIR_ICE) {
        if(ptEnd == ptCurrent) {
            state = FINISH;
            clear();
        }
        else if(!(getMapFromPoint(ptCurrent) & lastDirection)) {
            if(lastDirection == DIR_R) moveRight();
            else if(lastDirection == DIR_L) moveLeft();
            else if(lastDirection == DIR_U) moveUp();
            else if(lastDirection == DIR_D) moveDown();
        }
        else {
            makeArrow();
            state = IDLE;
            
            if(moveSoundId != -1) {
                AUDIO->stopEffect(moveSoundId);
                moveSoundId = -1;
            }
        }
    }
    else {
        int count = 0;
        if(!(getMapFromPoint(ptCurrent) & DIR_R))
            count++;
        if(!(getMapFromPoint(ptCurrent) & DIR_L))
            count++;
        if(!(getMapFromPoint(ptCurrent) & DIR_U))
            count++;
        if(!(getMapFromPoint(ptCurrent) & DIR_D))
            count++;
        
        if(ptEnd == ptCurrent) {
            state = FINISH;
            clear();
        }
        else if(count == 2) {
            if(!(getMapFromPoint(ptCurrent) & DIR_R) && ptLast.x <= ptCurrent.x)
                moveRight();
            else if(!(getMapFromPoint(ptCurrent) & DIR_L) && ptCurrent.x <= ptLast.x)
                moveLeft();
            else if(!(getMapFromPoint(ptCurrent) & DIR_U) && ptLast.y <= ptCurrent.y)
                moveUp();
            else if(!(getMapFromPoint(ptCurrent) & DIR_D) && ptCurrent.y <= ptLast.y)
                moveDown();
        }
        else {
            makeArrow();
            state = IDLE;
            
            if(moveSoundId != -1) {
                AUDIO->stopEffect(moveSoundId);
                moveSoundId = -1;
            }
        }
    }
}

void MazeLayer::makeArrow(bool isFirst) {
    if(player != nullptr) {
        player->hideArrowAll();
        
        if(!(getMapFromPoint(ptCurrent) & DIR_U))
            player->showArrow(Animal::DIR_UP, isFirst);
        if(!(getMapFromPoint(ptCurrent) & DIR_D))
            player->showArrow(Animal::DIR_DOWN, isFirst);
        if(!(getMapFromPoint(ptCurrent)& DIR_R))
            player->showArrow(Animal::DIR_RIGHT, isFirst);
        if(!(getMapFromPoint(ptCurrent) & DIR_L))
            player->showArrow(Animal::DIR_LEFT, isFirst);
    }
}

void MazeLayer::clear() {
    if(moveSoundId != -1) {
        AUDIO->stopEffect(moveSoundId);
        moveSoundId = -1;
    }
    
    long current = time(NULL);
    long elapsed = current - ACCOUNT->ads_time;
    ACCOUNT->ads_count++;
    int count = ACCOUNT->ads_count;
    
    if(120 <= elapsed || 4 <= count) {
        if(sdkbox::PluginAppnext::isVideoReady("fullscreen")) {
            ACCOUNT->ads_time = current;
            ACCOUNT->ads_count = 0;
            
            sdkbox::PluginAppnext::showVideo("fullscreen");
            AUDIO->pauseBackgroundMusic();
        }
        else {
            finish();
        }
    }
    else {
        finish();
    }
}

void MazeLayer::finish(float dt) {
    if(moveSoundId != -1) {
        AUDIO->stopEffect(moveSoundId);
        moveSoundId = -1;
    }
    
    bool isComplete = STAGE_MANAGER->currentStage.complete;
    if(isComplete) {
        int idx = STAGE_MANAGER->currentStage.nextAnimal;
        string name = STAGE_MANAGER->currentStage.nameAnimal;
        
        CollectPopup::idxAnimal = idx;
        CollectPopup::strCollect = name;
        auto popup = CollectPopup::create();
        if(popup != nullptr) {
            PopupManager::getInstance()->addPopup(popup);
        }
    }
    else {
        auto popup = ClearPopup::create();
        if(popup != nullptr) {
            PopupManager::getInstance()->addPopup(popup);
        }
    }
    
    ACCOUNT->setStageState(STAGE_MANAGER->currentStage.category, STAGE_MANAGER->currentStage.index - 1, Account::CLEAR);
    if(STAGE_MANAGER->isLastStage == false)
        ACCOUNT->setStageState(STAGE_MANAGER->nextStage.category, STAGE_MANAGER->nextStage.index - 1, Account::OPEN);
    ACCOUNT->sync();
}

Vec2 MazeLayer::getPositionFromPoint(Point pt) {
    Vec2 unit = vScale * 100.f;
    
    Vec2 v = Vec2(pt);
    v += Vec2(0.5f, 0.5f);
    v.x *= unit.x;
    v.y *= unit.y;
    
    return v;
}

int MazeLayer::getMapFromPoint(Point pt) {
    return map[pt.y][pt.x];
}
