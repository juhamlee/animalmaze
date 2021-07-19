//
//  MazeAlphaLayer.cpp
//  animalmaze-mobile
//
//  Created by 이주함 on 09/01/2019.
//

#include "MazeAlphaLayer.h"
#include "MazeLayer.h"
#include "PopupManager.h"
#include "ClearPopup.h"
#include "ShopPopup.h"
#include "PausePopup.h"
#include "HintPopup.h"
#include "CollectPopup.h"

#include "PluginAdMob/PluginAdMob.h"
#include "PluginAppnext/PluginAppnext.h"

static const int FONT_SIZE_HINT = 28;
static const int FONT_SIZE_STAGE = 50;

static const float SPEED_MOVE = 0.13f;

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

static const Color3B STAR_COLOR[6] = {
    Color3B(255, 240, 0),
    Color3B(91, 255, 234),
    Color3B(255, 112, 26),
    Color3B(255, 192, 2),
    Color3B(33, 195, 255),
    Color3B(255, 109, 202)
};

static const Color3B PATH_COLOR = Color3B(180, 180, 180);
static const Color3B HINT_COLOR = Color3B(220, 70, 70);

/*
static const Color4B TRACE_COLOR = Color4B(35, 69, 0, 255);
static const Color4B HINT_COLOR = Color4B(91, 116, 64, 255);
 */


Scene* MazeAlphaLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = MazeAlphaLayer::create();
    
    if(scene != nullptr && layer != nullptr) {
        scene->addChild(layer);
        
        POPUP_MANAGER->initWithBaseNode(layer);
        ScreenLog::getInstance()->attachToScene( scene );
        
        return scene;
    }
    
    return nullptr;
}

MazeAlphaLayer::MazeAlphaLayer() {
    AUDIO->preloadBackgroundMusic("sfx/bgm01.mp3");
    AUDIO->preloadBackgroundMusic("sfx/bgm02.mp3");
    
    AUDIO->preloadEffect("sfx/clear.mp3");
    AUDIO->preloadEffect("sfx/move.mp3");
    
    moveSoundId = -1;
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

MazeAlphaLayer::~MazeAlphaLayer() {
    AUDIO->stopBackgroundMusic();
    AUDIO->stopAllEffects();
    
    AUDIO->unloadEffect("sfx/clear.mp3");
    AUDIO->unloadEffect("sfx/move.mp3");
}

bool MazeAlphaLayer::init()
{
    Layer::init();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    int animal = STAGE_MANAGER->currentStage.animal;
    
    static const int arrColorAnimal[6][8] = {
        {3, 6, 12, 13, 22, 29, 30, -1},
        {11, 18, 21, 25, 26, 28, -1, -1},
        {1, 8, 10, 19, 23, -1, -1, -1},
        {4, 7, 14, 16, 27, 31, -1, -1},
        {2, 5, 9, 15, 17, 20, 24, -1},
        {32, -1, -1, -1, -1, -1, -1, -1}
    };
    
    coloridx = 0;
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
    sprintf(buf, "CLASSIC-%d", STAGE_MANAGER->currentStage.index);
    auto labStage = Label::createWithTTF(buf, FONT_PATH, FONT_SIZE_STAGE);
    if(labStage != nullptr) {
        labStage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labStage->setPosition(20, 1217);
        this->addChild(labStage, Z_UI);
    }
    
    auto btnPause = MenuItemImage::create("ui/btn_small.png",
                                          "ui/btn_small.png");
    if(btnPause != nullptr) {
        btnPause->setScaleY(-1.f);
        btnPause->setOpacity(128);
        btnPause->setCallback(CC_CALLBACK_1(MazeAlphaLayer::callbackPause, this));
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
        btnRetry->setCallback(CC_CALLBACK_1(MazeAlphaLayer::callbackRetry, this));
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
        btnHint->setCallback(CC_CALLBACK_1(MazeAlphaLayer::callbackHint, this));
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
    
    state = READY;
    isTouch = false;
    isFollow = false;
    isHintUsed = false;
    vecMazeNode.clear();
    
    vecTrace.clear();
    vecTracePosition.clear();
    vecHint.clear();
    
    pathTrace = PathLayer::create();
    if(pathTrace != nullptr) {
        pathTrace->setPosition(10, 190);
        pathTrace->setColor(PATH_COLOR);
        this->addChild(pathTrace, Z_PATH_TRACE);
    }
    pathHint = PathLayer::create();
    if(pathHint != nullptr) {
        pathHint->setVisible(false);
        pathHint->setPosition(10, 190);
        pathHint->setColor(HINT_COLOR);
        this->addChild(pathHint, Z_PATH_HINT);
    }
    
    return true;
}

void MazeAlphaLayer::onEnter() {
    Layer::onEnter();
    
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(MazeAlphaLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MazeAlphaLayer::onTouchMoved, this);
    
    customListener = EventListenerCustom::create(E_USE_HINT, CC_CALLBACK_1(MazeAlphaLayer::onUseHint, this));
    refreshListener = EventListenerCustom::create(E_REFRESH_HINT, CC_CALLBACK_1(MazeAlphaLayer::onRefreshHint, this));
    finishListener = EventListenerCustom::create(E_AD_VIDEO_CLOSE, CC_CALLBACK_1(MazeAlphaLayer::onFinish, this));
    
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(listener, this);
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(customListener, this);
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(refreshListener, this);
    EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(finishListener, this);
    
    string filepath = "";
    string imagepath = "";
    {
        filepath = STAGE_MANAGER->currentStage.filepath;
        imagepath = STAGE_MANAGER->currentStage.imagepath;
        
        if(STAGE_MANAGER->currentStage.diff == StageManager::DIFF_NORM)
            fScale = 0.5f;
        else if(STAGE_MANAGER->currentStage.diff == StageManager::DIFF_HARD)
            fScale = 0.33f;
        else
            fScale = 1.f;
    }
    
    vecMazeNode.clear();
    string str = FileUtils::getInstance()->getStringFromFile(filepath);
    rapidjson::Document doc;
    doc.Parse(str.c_str());
    if(!doc.HasParseError()) {
        int size = doc["nodes"].Size();
        for(int i = 0; i < size; i++) {
            MazeNode newNode;
            newNode.index = doc["nodes"][i]["idx"].GetInt();
            newNode.pos.x = doc["nodes"][i]["x"].GetInt();
            newNode.pos.y = doc["nodes"][i]["y"].GetInt();
            
            vecMazeNode.push_back(newNode);
        }
        
        size = doc["edges"].Size();
        for(int i = 0; i < size; i++) {
            int idx = doc["edges"][i]["idx"].GetInt();
            int adjsNumber = doc["edges"][i]["adjs"].Size();
            for(int j = 0; j < adjsNumber; j++) {
                int adjIdx = doc["edges"][i]["adjs"][j].GetInt();
                vecMazeNode[idx - 1].vecpAdjs.push_back(&vecMazeNode[adjIdx - 1]);
            }
        }
        
        if(doc.HasMember("hint")) {
            int size = (int)doc["hint"].Size();
            for(int i = 0; i < size; i++) {
                int idx = doc["hint"][i].GetInt();
                vecHint.push_back(getPositionFromIndex(idx));
            }
            pathHint->setPoints(vecHint, 1500.f);
        }
        
        startIndex = doc["start"].GetInt();
        endIndex = doc["end"].GetInt();
        currentIndex = vecMazeNode[startIndex - 1].index;
        lastIndex = currentIndex;
    }
    
    auto nodeMaze = Node::create();
    nodeMaze->setPosition(10, 190);
    this->addChild(nodeMaze, Z_MAZE);
    
    auto sprMaze = Sprite::create(imagepath);
    if(sprMaze != nullptr) {
        sprMaze->setPosition(sprMaze->getContentSize() * 0.5f);
        nodeMaze->addChild(sprMaze);
    }
    
    auto sprGoal = Sprite::create("game/goal.png");
    if(sprGoal != nullptr) {
        sprGoal->setPosition(vecMazeNode[endIndex - 1].pos);
        sprGoal->setColor(Color3B::WHITE);
        sprGoal->setScale(fScale);
        nodeMaze->addChild(sprGoal);
    }
    
    vecFollow.clear();
    
    follower = Animal::create();
    if(follower != nullptr) {
        follower->setAnimalByNumber(0);
        follower->setPosition(vecMazeNode[startIndex - 1].pos);
        follower->setScale(fScale);
        follower->sprCharacter->setColor(STAR_COLOR[coloridx]);
        nodeMaze->addChild(follower);
        
        if(STAGE_MANAGER->currentStage.animal == 0)
            follower->setVisible(false);
    }
    
    player = Animal::create();
    if(player != nullptr) {
        player->setAnimalByNumber(STAGE_MANAGER->currentStage.animal);
        player->setPosition(vecMazeNode[startIndex - 1].pos);
        player->setScale(fScale);
        nodeMaze->addChild(player);
        
        if(STAGE_MANAGER->currentStage.animal == 0)
            player->sprCharacter->setColor(STAR_COLOR[coloridx]);
    }
    
    Vec2 current = getPositionFromIndex(startIndex);
    vecTrace.push_back(startIndex);
    vecTracePosition.push_back(current);
    pathTrace->pushPoint(current);
    pathTrace->setExtraPoint(current);
    
    makeArrow(true);
    
    this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create(CC_CALLBACK_0(MazeAlphaLayer::start, this)), NULL));
    
    scheduleUpdate();
    
    if(ACCOUNT->isNoAds == false) {
        if(sdkbox::PluginAdMob::isAvailable("home"))
            sdkbox::PluginAdMob::show("home");
        else
            sdkbox::PluginAdMob::cache("home");
    }
    else
        sdkbox::PluginAdMob::hide("home");
}

void MazeAlphaLayer::onExit() {
    EVENT_DISPATCHER->removeEventListener(listener);
    EVENT_DISPATCHER->removeEventListener(customListener);
    EVENT_DISPATCHER->removeEventListener(refreshListener);
    EVENT_DISPATCHER->removeEventListener(finishListener);
    
    Layer::onExit();
}

bool MazeAlphaLayer::onTouchBegan(Touch* touch, Event* event) {
    if(state == IDLE) {
        isTouch = true;
        auto point = touch->getLocation();
        touchOrigin = point;
        
        return true;
    }
    
    return false;
}

void MazeAlphaLayer::onTouchMoved(Touch* touch, Event* event) {
    if(state == IDLE && isTouch == true) {
        auto point = touch->getLocation();
        Vec2 move = point - touchOrigin;
        float length = move.length();
        
        if(7.f < length) {
            MazeNode* pNode = &vecMazeNode[currentIndex - 1];
            for(int i = 0; i < pNode->vecpAdjs.size(); i++) {
                MazeNode* pAdj = pNode->vecpAdjs[i];
                Vec2 target = pAdj->pos - pNode->pos;
                float angle = target.getAngle(move);
                
                if(fabs(angle) < M_PI_4 * 0.5f) {
                    moveTo(pAdj->index);
                    
                    if(moveSoundId == -1)
                        moveSoundId = AUDIO->playEffect("sfx/move.mp3", true);
                    
                    break;
                }
            }
        }
    }
}

void MazeAlphaLayer::update(float dt) {
    if(state == MOVE) {
        pathTrace->setExtraPoint(player->getPosition());
    }
    
    if(follower && player) {
        float offset = 100.f * fScale;
        int size = (unsigned int)vecFollow.size();
        for(int i = size; 0 < i; i--) {
            Vec2 prev;
            Vec2 next = vecFollow[i - 1];
            if(i == size)
                prev = player->getPosition();
            else
                prev = vecFollow[i];
            
            float dist = prev.distance(next);
            if(dist <= offset)
                offset = offset - dist;
            else {
                Vec2 dir = next - prev;
                dir.normalize();
                follower->setPosition(prev + (dir * offset));
                
                break;
            }
        }
        /*
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
         */
    }
}

void MazeAlphaLayer::onUseHint(EventCustom* event) {
    int hint = ACCOUNT->hint;
    if(0 < hint && isHintUsed == false) {
        isHintUsed = true;
        
        ACCOUNT->hint = hint - 1;
        ACCOUNT->sync();
        
        pathHint->setVisible(true);
        
        char buf[16];
        sprintf(buf, "%d", hint - 1);
        labHint->setString(buf);
    }
}

void MazeAlphaLayer::onRefreshHint(EventCustom* event) {
    int hint = ACCOUNT->hint;
    
    char buf[16];
    sprintf(buf, "%d", hint);
    labHint->setString(buf);
}

void MazeAlphaLayer::onFinish(EventCustom* event) {
    if(ACCOUNT->isNoAds == false) {
        sdkbox::PluginAppnext::refreshVideo("fullscreen");
    }
    
    AUDIO->resumeBackgroundMusic();
    scheduleOnce(schedule_selector(MazeLayer::finish), 0.1f);
}

void MazeAlphaLayer::callbackPause(Ref* pSender) {
    auto popup = PausePopup::create();
    if(popup != nullptr) {
        POPUP_MANAGER->addPopup(popup);
    }
    
    AUDIO->playEffect("sfx/click.mp3");
}

void MazeAlphaLayer::callbackRetry(Ref* pSender) {
    auto scene = MazeAlphaLayer::createScene();
    auto director = Director::getInstance();
    director->replaceScene(scene);
    
    AUDIO->playEffect("sfx/click.mp3");
}

void MazeAlphaLayer::callbackHint(Ref* pSender) {
    auto popup = HintPopup::create();
    if(popup != nullptr) {
        POPUP_MANAGER->addPopup(popup);
    }
    
    AUDIO->playEffect("sfx/click.mp3");
}

void MazeAlphaLayer::start() {
    state = IDLE;
    
    srand((unsigned int)time(NULL));
    int r = rand() % 2 + 1;
    char buf[32];
    sprintf(buf, "sfx/bgm%02d.mp3", r);
    AUDIO->playBackgroundMusic(buf, true);
}

void MazeAlphaLayer::moveTo(int index) {
    state = MOVE;
    isTouch = false;
    
    Vec2 currentPos = vecMazeNode[currentIndex - 1].pos;
    Vec2 nextPos = vecMazeNode[index - 1].pos;
    float dist = currentPos.distance(nextPos);
    float delay = dist / 100.f * SPEED_MOVE;
    
    if(2 <= vecTrace.size()) {
        int size = (int)vecTrace.size();
        if(vecTrace[size - 2] == index) {
            vecTrace.pop_back();
            vecTracePosition.pop_back();
            pathTrace->popPoint();
            pathTrace->setExtraPoint(getPositionFromIndex(currentIndex));
        }
    }
    vecFollow.push_back(currentPos);
    
    player->hideArrowAll();
    
    MoveTo* move = MoveTo::create(delay, nextPos);
    move->setTag(TAG_MOVE);
    player->stopActionByTag(TAG_MOVE);
    player->setPosition(currentPos);
    player->runAction(move);
    
    this->runAction(Sequence::create(DelayTime::create(delay + 0.001f), CallFunc::create(CC_CALLBACK_0(MazeAlphaLayer::check, this)), NULL));
    
    if(nextPos.x - currentPos.x < 0)
        player->setFlip(true);
    else
        player->setFlip(false);
    
    lastIndex = currentIndex;
    currentIndex = index;
}

void MazeAlphaLayer::check() {
    if(state == FINISH)
        return;
    
    int size = (int)vecTrace.size();
    if(0 < size && vecTrace[size - 1] == currentIndex) {
        pathTrace->setExtraPoint(getPositionFromIndex(currentIndex));
    }
    else {
        Vec2 current = getPositionFromIndex(currentIndex);
        vecTrace.push_back(currentIndex);
        vecTracePosition.push_back(current);
        pathTrace->pushPoint(current);
        pathTrace->setExtraPoint(current);
    }
    
    if(currentIndex == endIndex) {
        state = FINISH;
        clear();
    }
    else {
        int count = (int)vecMazeNode[currentIndex - 1].vecpAdjs.size();
        if(count == 2) {
            for(int i = 0; i < count; i++) {
                int nextIndex = vecMazeNode[currentIndex - 1].vecpAdjs[i]->index;
                if(nextIndex != lastIndex) {
                    moveTo(nextIndex);
                    break;
                }
            }
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

void MazeAlphaLayer::makeArrow(bool isFist) {
    if(player != nullptr) {
        player->hideArrowAll();
        
        MazeNode* pNode = &vecMazeNode[currentIndex - 1];
        for(int i = 0; i < pNode->vecpAdjs.size(); i++) {
            MazeNode* pAdj = pNode->vecpAdjs[i];
            Vec2 target = pAdj->pos - pNode->pos;
            Vec2 axis(1, 0);
            float angle = axis.getAngle(target);
            
            player->addArrow(angle, isFist);
        }
    }
}

void MazeAlphaLayer::clear() {
    if(moveSoundId != -1) {
        AUDIO->stopEffect(moveSoundId);
        moveSoundId = -1;
    }
    
    long current = time(NULL);
    long elapsed = current - ACCOUNT->ads_time;
    ACCOUNT->ads_count++;
    int count = ACCOUNT->ads_count;
    
    if(ACCOUNT->isNoAds == false) {
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
    else {
        finish();
    }
}

void MazeAlphaLayer::finish(float dt) {
    bool isComplete = STAGE_MANAGER->currentStage.complete;
    if(isComplete) {
        int idx = STAGE_MANAGER->currentStage.nextAnimal;
        string name = STAGE_MANAGER->currentStage.nameAnimal;
        
        CollectPopup::idxAnimal = idx;
        CollectPopup::strCollect = name;
        auto popup = CollectPopup::create();
        if(popup != nullptr) {
            POPUP_MANAGER->addPopup(popup);
        }
    }
    else {
        auto popup = ClearPopup::create();
        if(popup != nullptr) {
            POPUP_MANAGER->addPopup(popup);
        }
    }
    
    ACCOUNT->setStageState(STAGE_MANAGER->currentStage.category, STAGE_MANAGER->currentStage.index - 1, Account::CLEAR);
    if(STAGE_MANAGER->isLastStage == false)
        ACCOUNT->setStageState(STAGE_MANAGER->nextStage.category, STAGE_MANAGER->nextStage.index - 1, Account::OPEN);
    ACCOUNT->sync();
}

Vec2 MazeAlphaLayer::getPositionFromIndex(int idx) {
    int size = (int)vecMazeNode.size();
    if(0 < idx && idx <= size)
        return vecMazeNode[idx - 1].pos;
    else
        return Vec2(0,0);
}
