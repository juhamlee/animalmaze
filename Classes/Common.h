//
//  common.h
//  animalmaze
//
//  Created by 이주함 on 05/01/2019.
//

#ifndef Common_h
#define Common_h

#include "Account.h"
#include "StageManager.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;
using namespace CocosDenshion;

#define AUDIO               SimpleAudioEngine::getInstance()
#define EVENT_DISPATCHER    Director::getInstance()->getEventDispatcher()
#define ACCOUNT             Account::getInstance()
#define STAGE_MANAGER       StageManager::getInstance()
#define POPUP_MANAGER       PopupManager::getInstance()

#define E_USE_HINT          "event_use_hint"
#define E_AD_BANNER_READY   "event_ad_banner_ready"
#define E_AD_MAIN_READY     "event_ad_main_ready"
#define E_AD_MAIN_CLOSE     "event_ad_main_close"
#define E_AD_VIDEO_CLOSE    "event_ad_video_close"

static const float MAZE_WIDTH = 700.f;
static const float MAZE_HEIGHT = 900.f;
static const float ANIMAL_SIZE = 120.f;

static const int DIR_R = 2;
static const int DIR_U = 1;
static const int DIR_L = 8;
static const int DIR_D = 4;
static const int DIR_ICE = 16;

static const int TAG_MOVE = 888;

static const char* FONT_PATH = "fonts/VCR_OSD_MONO_1.001.ttf";

#endif /* Common_h */
