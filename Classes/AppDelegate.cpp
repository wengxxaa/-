#include "AppDelegate.h"
#include "gameconfig/ConfigState.h"
#include "gameconfig/PayScene.h"
#include "gameconfig/BaseConfig.h"
#include "cj/cjconfig.h"
#include "cj/GameData.h"
#include "gamescene/MainScene.h"
#include "gamescene/FirstScene.h"
#if(CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
//#include "vld.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "MobClickCpp.h"
#include "CommentLayer.h"
#endif

USING_NS_CC;

//const int WIDTH = 750;
//const int HEIGHT = 1334;
//const float SCALE=0.45f;

const int WIDTH = 480;
const int HEIGHT = 800;
const float SCALE=0.7f;

static cocos2d::Size designResolutionSize = cocos2d::Size(750, 1334);
static cocos2d::Size smallResolutionSize = cocos2d::Size(320, 480);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(768, 1024);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1536, 2048);
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
    return 0; //flag for packages manager
}
bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("My Game", cocos2d::Rect(0, 0, WIDTH, HEIGHT));
#else
        glview = GLViewImpl::create("My Game");
#endif
        director->setOpenGLView(glview);
    }

	
	
	//
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    auto data=FileUtils::getInstance()->getValueMapFromFile("umeng_ios.plist");

	auto umengdata=data.at("umengNumber").asString();

	MOBCLICKCPP_START_WITH_APPKEY(umengdata.c_str());
#endif
	//
#if(CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	GameDataInstance()->BANER_HEIGHT=68;
#else
	GameDataInstance()->BANER_HEIGHT=(Director::getInstance()->getWinSize().width/320.0f)*50*(800.0f/Director::getInstance()->getWinSize().height);	//baner的高�?
#endif
	//GameDataInstance()->BANER_HEIGHT -= 6;

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
	//模式一
	glview->setDesignResolutionSize(WIDTH, HEIGHT, ResolutionPolicy::EXACT_FIT);
	glview->setFrameSize(WIDTH, HEIGHT);
	//模式一

	////模式二
	//// Set the design resolution
	//glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
	//auto frameSize = glview->getFrameSize();
	//// if the frame's height is larger than the height of medium size.
	//if (frameSize.height > mediumResolutionSize.height)
	//{
	//	director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height, largeResolutionSize.width / designResolutionSize.width));
	//}
	//// if the frame's height is larger than the height of small size.
	//else if (frameSize.height > smallResolutionSize.height)
	//{
	//	director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height, mediumResolutionSize.width / designResolutionSize.width));
	//}
	//// if the frame's height is smaller than the height of medium size.
	//else
	//{
	//	director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height, smallResolutionSize.width / designResolutionSize.width));
	//}
	////模式二

	cj::LogText("judge0");
	
	srand(time(NULL));
    register_all_packages();

	cj::LogText("judge00");

	FileUtils::getInstance()->addZpkFile("001.zpk");
	
	cj::LogText("judge000");

	//ǩ�����
	//if (!vigame::signature::check())
	//{
	//	return false;
	//}

#ifdef WIN32
	glview->setFrameZoomFactor(SCALE);
#endif

	cj::LogText("judge1");

	cj::initData();

	cj::LogText("judge1");
	
	GameDataInstance()->d_rank = false;
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS||IOSMODE)
	cj::preLoad();
	director->runWithScene(MainScene::createScene());
	//公告
	vigame::notice::open();
#else
	director->runWithScene(FirstScene::createScene());
#endif
	
	Device::setKeepScreenOn(true);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    //CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
    //IOS 评论相关--------------------
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    bool comment = UserDefault::getInstance()->getBoolForKey("GoComment", false);
    if(comment == true){
        unsigned long second = CommentUtil::get_ElapsedRealtime();
        char secondString[265] = {0};
        sprintf(secondString,"%ld", second);
        UserDefault::getInstance()->setStringForKey("LeaveCommentTime",  secondString);
    }
#endif
    //IOS 评论相关--------------------
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    //CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    
    //IOS 评论相关--------------------
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //跳出 游戏 10 秒后回来才算评论成功
    bool comment = UserDefault::getInstance()->getBoolForKey("GoComment", false);
    if(comment == true){
        string secondString = UserDefault::getInstance()->getStringForKey("LeaveCommentTime","");
        unsigned long second = atol(secondString.c_str());
        unsigned long nowSecond = CommentUtil::get_ElapsedRealtime();
        //跳出 游戏 10 秒后回来才算评论成功
        if((nowSecond - second) >= 10000 ){
            UserDefault::getInstance()->setBoolForKey("HaveComment", true);
            //评论回来，这里给奖励
			if (!GameData::getSaveData()->_starPraise)
			{
				GameData::getSaveData()->_starPraise = true;
				GameData::getSaveData()->_diamondNumber += 10;
				PayScene::getInstance()->dailyClickTip(10, 2);
				GameData::getInstance()->dataSave();
			}
        }
        GameDataInstance()->d_haoping=false;
    }
#endif
    //IOS 评论相关--------------------
}
