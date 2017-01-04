#ifndef CJCONFIG_H_
#define CJCONFIG_H_

#include "SimpleAudioEngine.h"
#include"../gameconfig/ConfigState.h"
#include "core/macros.h"
#include"core/base/log.h"
#include"cocos2d.h"
#include"stdio.h"
#include "GameData.h"
USING_NS_CC;

static const float FntXOff=0.0f;
static const float FntYOff=0.0f;

#define ATLAS_PATH "ziti/sz_1.png"
#define ATLAS_PATH2 "ziti/sz_2.png"
#define ATLAS_PATH3 "ziti/sz_3.png"
#define ATLAS_PATH4 "ziti/sz_4.png"
#define ATLAS_PATH5 "ziti/sz_5.png"
#define ATLAS_PATH6 "ziti/sz_11.png"
#define ATLAS_PATH7 "ziti/sz_7.png"
#define ATLAS_PATH8 "ziti/sz_8.png"
#define ATLAS_PATH9 "ziti/score.png"
#define ATLAS_PATH10 "ziti/sz_10.png"
#define ATLAS_PATH11 "ziti/time01.png"
#define ATLAS_PATH12 "ziti/time02.png"
#define ATLAS_PATH13 "ziti/score_end.png"
#define ATLAS_PATH14 "ziti/fillup/score_blue.png"
#define ATLAS_PATH15 "ziti/fillup/score_green.png"
#define ATLAS_PATH16 "ziti/fillup/score_purple.png"
#define ATLAS_PATH17 "ziti/fillup/score_pink.png"
#define ATLAS_PATH18 "ziti/fillup/score_yellow.png"
#define ATLAS_PATH19 "ziti/fillup/button_add.png"
#define ATLAS_PATH20 "ziti/txtzuanshi.png"

#define NAMESPACE_CJ_BEGIN namespace cj{
#define NAMESPACE_CJ_END }

#define WINORIGIN (Director::getInstance()->getVisibleOrigin())	
#define WINSIZE (Director::getInstance()->getVisibleSize())

#define CREATE_SCENE(__TYPE__) \
	static Scene* createScene() \
{ \
	Scene *scene=Scene::create(); \
	__TYPE__ *layer=__TYPE__::create();\
	scene->addChild(layer,1,"mainlayer");\
	return scene;\
}

class cjMenu:public Menu
{
public:
	//create with image
	static Menu* createWithImage(const std::string &str1,const std::string &str2,const std::string &str3,const ccMenuCallback &call,bool tt=true);

	static Menu* createWithImage(const std::string &str,const ccMenuCallback &call,bool tt=true);

	static Menu* createWithImage2(const std::string &str,const ccMenuCallback &call,bool tt=true);

	//create with sprite
	static Menu* createWithSprite(Sprite* normalSp,Sprite* selectedSp, const ccMenuCallback &call);

	//create with font
	static Menu* createWithFont(const std::string &fontstring,const ccMenuCallback &call);

public:
	virtual void addChild(Node * child, int zOrder);
};

class cjTTFLabel:public Label
{
public:
	cjTTFLabel(){};
	~cjTTFLabel(){};
public:
	//create with TTF
	static Label* createWithString(const std::string &text,int size=20,const std::string textpath=TTF_PATH);

	static Label* createWithInt(const int number,int size=20,std::string textpath=TTF_PATH);

	static string getStringByKey(const char *str);

	static string getNameByInt(const char*str,int id);
	static string getNameByInt(const char*str, int id1, int id2);
	static string getNameByIntL(const char*str,int id);

};

class cjAtlasLabel:public Label
{
public:
	cjAtlasLabel(){};
	~cjAtlasLabel(){};
public:
	static LabelAtlas* createWithInt(const int number,const std::string &textpath=ATLAS_PATH);
	static LabelAtlas* createWithString(const std::string &str, const std::string &textpath = ATLAS_PATH);

	static void initSize(int &width, int &height,const std::string &textpath);
};

class cjSchedule
{
public:
	cjSchedule(){};
	~cjSchedule(){};

	static cjSchedule* getInstance()
	{
		static cjSchedule one;
		return &one;
	}

public:
	void runOnNextFrame(const ccSchedulerFunc &call);

	//鈭喡∑捖€欙？鈭偯?
	void shockBegin(Layer *lay);
	void shockEnd(Layer *lay);

	//jnitext
	void jnitext(const string& str1,const string &str2);
};

class cjSprite:public Sprite{
public:
	static Sprite* createWithImage(const std::string &str1); 
};

class cjMusic
{
public:
	static void playBackgroundMusic(const char *str,bool loop=false);

	static void stopBackgroundMusck()
	{
        if(GameData::getSaveData()->_music)
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}
    
    static void pauseBackgroundMusic()
    {
        if(GameData::getSaveData()->_music)
            CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
    
    static void resumeBackgroundMusic()
    {
        if(GameData::getSaveData()->_music)
            CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
    
    static bool isBackgroundMusic()
    {
        if(GameData::getSaveData()->_music)
            return CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
        else
            return false;
    }

	static int playEffect(const char *str,bool loop=false);

	static void stopEffect(int id);

	static void stopAllEffect()
	{
        if(GameData::getSaveData()->_music)
            CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
	}
};

class handNode:public Node
{
public:
	handNode(){};
	~handNode(){};

	static handNode* createHand(float ft)
	{
	 	handNode *one=new handNode();
		if(one&&one->init())
		{
			one->autorelease();
			one->setScale(ft);
			return one;
		}
		else 
		{
			CC_SAFE_DELETE(one);
			return nullptr;
		}
	}

	bool init();
};

NAMESPACE_CJ_BEGIN
	//鈥樎衡€濃€樏?
	void preLoad();

	//鈮ツ甭犅郝樎犓澝︹€?
	void initData();

	//Log聽鈥扳墺藛
	void LogText(string str);

	//禄鈥櫭访β犫€扳墺藛  
	void LogShow(string str);
	void LogShow(int time, string str);

	//鈭偮÷宦癱sv艗茠潞藳  
	std::map<int,std::vector<string>> getDataFromCsv(string str);
	std::map<pair<int, char>, string> getDataFromCsv2(string str);
	std::map<int, std::vector<int>> getDataFromReset(string str);

	//string鈭懨仿棵?
	std::vector<int> getIntByString(string str);  
	  
	//艗垄鈥撯増碌芦卢潞

    //按钮事件
    void clickUMCount(int id);
    //五星好评
    void starUMCount(int id);
    //推送
    void pushUMCount();
	//支付
	void payUMCount(int key, int state);
	//消费虚拟币
	void consumeUMCount(int key);
	//关卡分析
	void levelUMCount(int level, int state);

	//get screen hot
	cocos2d::Sprite* screenShot();
	void captureScene(std::string str="pic.png");

	//获取文件名称
	std::vector<string> getVideoName(std::string str);

NAMESPACE_CJ_END

#endif
