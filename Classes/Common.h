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
#include "ScreenLog.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;
using namespace CocosDenshion;

//싱글턴 인스턴스
#define AUDIO               SimpleAudioEngine::getInstance()
#define EVENT_DISPATCHER    Director::getInstance()->getEventDispatcher()
#define ACCOUNT             Account::getInstance()
#define STAGE_MANAGER       StageManager::getInstance()
#define POPUP_MANAGER       PopupManager::getInstance()

//이벤트를 브로드캐스트하기 위한 이벤트명 정의
#define E_USE_HINT          "event_use_hint"
#define E_REFRESH_HINT      "event_refresh_hint"
#define E_RELOAD_SCENE      "event_reload_scene"
#define E_CLOSE_WAIT        "event_close_wait"
#define E_AD_BANNER_READY   "event_ad_banner_ready"
#define E_AD_MAIN_READY     "event_ad_main_ready"
#define E_AD_MAIN_CLOSE     "event_ad_main_close"
#define E_AD_VIDEO_CLOSE    "event_ad_video_close"

//인앱 결제를 위한 SKU 이름
#define SKU_UNLOCK1         "unlock_01"
#define SKU_UNLOCK2         "unlock_02"
#define SKU_UNLOCK3         "unlock_03"
#define SKU_UNLOCK4         "unlock_04"
#define SKU_NO_ADS          "no_ads"
#define SKU_HINT_5          "hint_5"
#define SKU_HINT_25         "hint_25"
#define SKU_HINT_100        "hint_100"
#define SKU_HINT_250        "hint_250"

//미로를 구현하는 기준 사이즈
static const float MAZE_WIDTH = 700.f;
static const float MAZE_HEIGHT = 900.f;

static const int DIR_R = 2;
static const int DIR_U = 1;
static const int DIR_L = 8;
static const int DIR_D = 4;
static const int DIR_ICE = 16;

static const int TAG_MOVE = 888;

static const char* FONT_PATH = "fonts/VCR_OSD_MONO_1.001.ttf";

#endif /* Common_h */
