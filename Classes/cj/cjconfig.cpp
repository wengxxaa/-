#include "cjconfig.h"
#include "Toast.h"
#include "../gameconfig/PayScene.h"
#include "../rank/NetWork.h"

#include "vigame_ad.h"
#include "vigame_core.h"
#include "vigame_pay.h"
#include "vigame_tj.h"
#include "vigame_social.h"
#include "vigame_share.h"

#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

#include <spine/spine-cocos2dx.h>
#include "spine/spine.h"
using namespace spine;

static const string SPINEPATH = "FillUpMotion/spine/";

//CJMENU
Menu* cjMenu::createWithImage(const std::string &str1,const std::string &str2,const std::string &str3,const ccMenuCallback &call,bool tt)
{
	auto normalSp=cjSprite::createWithImage(str1);
	auto selectSp=cjSprite::createWithImage(str2);
	selectSp->setScale(1.1f);
	auto disableSp=cjSprite::createWithImage(str3);
	auto menuItem=MenuItemSprite::create(normalSp,selectSp,disableSp,call);
	auto select=menuItem->getSelectedImage();
	select->setAnchorPoint(Vec2(0.5f,0.5f));
	if(tt)
		select->setScale(1.1f);
	select->setPosition(select->getPosition()+Vec2(select->getContentSize()/2.0f));
	auto menu=Menu::create(menuItem,nullptr);
	return menu;
}

void cjMenu::addChild(Node * child, int zOrder)
{
	this->addChild(child,zOrder);
}

Menu* cjMenu::createWithImage2(const std::string &str,const ccMenuCallback &call,bool tt)
{
	auto normalSp=Sprite::create(str);
	auto selectSp=Sprite::create(str);
	auto disableSp=Sprite::create(str);
	auto menuItem=MenuItemSprite::create(normalSp,selectSp,disableSp,call);
	auto select=menuItem->getSelectedImage();
	select->setAnchorPoint(Vec2(0.5f,0.5f));
	if(tt)
		select->setScale(1.1f);
	else
		select->setScale(1.0f);
	select->setPosition(select->getPosition()+Vec2(select->getContentSize()/2.0f));
	auto menu=Menu::create(menuItem,nullptr);
	return menu;
}

Menu* cjMenu::createWithImage(const std::string &str1,const ccMenuCallback &call,bool tt)
{
	return createWithImage(str1,str1,str1,call,tt);
}

Menu* cjMenu::createWithFont(const std::string &fontstring,const ccMenuCallback &call)
{
	auto menuItem=MenuItemFont::create(fontstring,call);
	auto menu=Menu::create(menuItem,nullptr);
	return menu;
}

Menu* cjMenu::createWithSprite(Sprite* normalSp,Sprite* selectedSp, const ccMenuCallback &call)
{
	auto menuItem=MenuItemSprite::create(normalSp,selectedSp,call);
	auto menu=Menu::create(menuItem,nullptr);
	return menu;
}

//CJATLASLABEL
void cjAtlasLabel::initSize(int &width, int &height, const std::string &textpath)
{
	if (textpath == ATLAS_PATH)
	{
		width = 13;
		height = 18;
	}
	else if (textpath == ATLAS_PATH2)
	{
		width = 21;
		height = 31;
	}
	else if (textpath == ATLAS_PATH3)
	{
		width = 17;
		height = 23;
	}
	else if (textpath == ATLAS_PATH4)
	{
		width = 13;
		height = 22;
	}
	else if (textpath == ATLAS_PATH5)
	{
		width = 22;
		height = 26;
	}
	else if (textpath == ATLAS_PATH6)
	{
		width = 48;
		height = 59;
	}
	else if (textpath == ATLAS_PATH7)
	{
		width = 17;
		height = 26;
	}
	else if (textpath == ATLAS_PATH8)
	{
		width = 17;
		height = 26;
	}
	else if (textpath == ATLAS_PATH9)
	{
		width = 20;
		height = 36;
	}
	else if (textpath == ATLAS_PATH10)
	{
		width = 17;
		height = 26;
	}
	else if (textpath == ATLAS_PATH11 || textpath == ATLAS_PATH12)
	{
		width = 9;
		height = 14;
	}
	else if (textpath == ATLAS_PATH13)
	{
		width = 30;
		height = 48;
	}
	else if (textpath == ATLAS_PATH14 || textpath == ATLAS_PATH15 || textpath == ATLAS_PATH16 || textpath == ATLAS_PATH17|| textpath == ATLAS_PATH18)
	{
		width = 30;
		height = 40;
	}
	else if (textpath == ATLAS_PATH19)
	{
		width = 12;
		height = 18;
	}
	else if (textpath == ATLAS_PATH20)
	{
		width = 12;
		height = 21;
	}
}

LabelAtlas* cjAtlasLabel::createWithInt(const int number,const std::string &textpath/* =ATLAS_PATH */)
{
	int width = 0;
	int height = 0;
	initSize(width, height, textpath);
	assert(width != 0 && height != 0);
	char name[10];
	sprintf(name, "%d", number);
	auto diceCount = LabelAtlas::create(name, textpath, width, height, '0');
	diceCount->setAnchorPoint(Vec2(0.5f, 0.5f));
	return diceCount;
}

LabelAtlas* cjAtlasLabel::createWithString(const std::string &str, const std::string &textpath/* =ATLAS_PATH */)
{
	int width = 0;
	int height = 0;
	initSize(width, height, textpath);
	assert(width != 0 && height != 0);
	auto diceCount = LabelAtlas::create(str.c_str(), textpath, width, height, '0');
	diceCount->setAnchorPoint(Vec2(0.5f, 0.5f));
	return diceCount;
}

//CJTTFLABEL
Label* cjTTFLabel::createWithString(const std::string &text,int size/* =20 */,const std::string textpath/* =TTF_PATH */)
{
	TTFConfig ttfconfig;
	ttfconfig.fontSize=size;
	ttfconfig.fontFilePath=textpath;
	auto label=Label::createWithTTF(ttfconfig,text);
	label->setAnchorPoint(Vec2(0.5f+FntXOff,0.5f+FntYOff));
	return label;
}

Label* cjTTFLabel::createWithInt(const int number,int size/* =20 */,std::string textpath/* =TTF_PATH */)
{
	char name[30];
	sprintf(name,"%d",number);
	auto label=createWithString(string(name),size,textpath);
	return label;
}

string cjTTFLabel::getNameByInt(const char*str,int id)
{
	char name[30];
	sprintf(name,str,id);
	return name;
}

string cjTTFLabel::getNameByInt(const char*str, int id1, int id2)
{
	char name[30];
	sprintf(name, str, id1, id2);
	return name;
}

string cjTTFLabel::getNameByIntL(const char*str,int id)
{
	char name[100];
	sprintf(name,str,id);
	return name;
}

//jbk2utf8
std::string cjTTFLabel::getStringByKey(const char *str)
{
	auto dic = Dictionary::createWithContentsOfFileThreadSafe(HANZI_XML);  
	__String* fnt1name=(__String*)dic->objectForKey(str); 
	
	return fnt1name->getCString();
}

//CJSCHEDULE
void cjSchedule::runOnNextFrame(const ccSchedulerFunc &call)
{
	Director::getInstance()->getRunningScene()->getScheduler()->schedule(call,this,0.1f,0,0.0f,false,"NextFrame");
}

void cjSchedule::shockBegin(Layer *lay)
{
	//屏幕震动
	float dis=1.0f;
	auto seq1=RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(MoveTo::create(0.03f,Vec2(dis,0)),MoveTo::create(0.03f,Vec2(-dis,0))));
	auto seq2=RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(MoveTo::create(0.04f,Vec2(0,dis)),MoveTo::create(0.04f,Vec2(0,-dis))));
	seq1->setTag(101);
	seq2->setTag(102);
	lay->runAction(seq1);
	lay->runAction(seq2);

	long long pattern[1]={100};
//#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//	CocosDenshion::SimpleAudioEngine::getInstance()->vibrateWithPattern(pattern,true);
//#endif
}

void cjSchedule::shockEnd(Layer *lay)
{
	lay->stopActionByTag(101);
	lay->stopActionByTag(102);
	lay->setPosition(Vec2(0,0));
//#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//	CocosDenshion::SimpleAudioEngine::getInstance()->cancelVibrate();
//#endif
}

void cjSchedule::jnitext(const string& str1,const string &str2)
{
//#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//	cocos2d::JniMethodInfo t;
//	if(JniHelper::getStaticMethodInfo(t,EXTRA_FILE,"showTipDialog","()V"))
//	{
//		t.env->CallStaticVoidMethod(t.classID,t.methodID);
//	}
//#endif
}

//CJSPRITE
Sprite* cjSprite::createWithImage(const std::string &framename)
{
	auto frame=SpriteFrameCache::getInstance()->getSpriteFrameByName(framename);
	CCAssert(frame,"the framename not exit!");
	return Sprite::createWithSpriteFrame(frame);
}

//CJMUSCI
void cjMusic::playBackgroundMusic(const char *str,bool loop)
{
	if(GameData::getSaveData()->_music)
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(str,loop);
}

int cjMusic::playEffect(const char *str,bool loop)
{
	if(GameData::getSaveData()->_music)
		return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(str,loop);
	else
		return 0;
}

void cjMusic::stopEffect(int id)
{
	if(GameData::getSaveData()->_music)
		return CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(id);
}

//handNode
bool handNode::init()
{
	if(!Node::init())
		return false;

	auto hand1=cjSprite::createWithImage("yba.png");
	addChild(hand1);
	hand1->setVisible(false);
	hand1->setAnchorPoint(Vec2(0.15f,0.95f));
	auto seq1=Sequence::createWithTwoActions(DelayTime::create(0.5f),Show::create());
	auto seq2=Sequence::createWithTwoActions(DelayTime::create(0.5f),Hide::create());
	hand1->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(seq1,seq2)));
	auto hand2=cjSprite::createWithImage("ybb.png");
	auto seq3=Sequence::createWithTwoActions(DelayTime::create(0.5f),Hide::create());
	auto seq4=Sequence::createWithTwoActions(DelayTime::create(0.5f),Show::create());
	hand2->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(seq3,seq4)));
	addChild(hand2);
	hand2->setAnchorPoint(Vec2(0.15f,0.95f));

	return true;
}

NAMESPACE_CJ_BEGIN
//Log输出
void LogText(string str)
{
	cocos2d::log(str.c_str(), str.c_str());
}

void preLoad()
{
	//init data
	GameDataInstance()->loadSaveFile();
	GameDataInstance()->configInit();
	//

	GameDataInstance()->main_num = 0;
	GameDataInstance()->d_gameEnter = 0;
	GameDataInstance()->d_starNum = 3;

	auto t1 = time(NULL);
	GameDataInstance()->d_daytime = (t1 / (24 * 3600));
	auto t2 = localtime(&t1);
	GameDataInstance()->d_monthtime = t2->tm_mon;
	GameDataInstance()->d_monthday = t2->tm_mday;

	//new one day
	if (GameDataInstance()->d_daytime != GameData::getSaveData()->_day_num)
	{
		GameData::getSaveData()->_day_num = GameDataInstance()->d_daytime;
		GameData::getSaveData()->_levelreward_num = 0;

		GameData::getSaveData()->_gg_guanshu = 0;

		GameData::getSaveData()->_freeroundnum = 0;
		GameData::getSaveData()->_freediamondnum = 0;
	}

	if (GameDataInstance()->d_daytime != GameData::getSaveData()->_freeroundday)
	{
		GameData::getSaveData()->_freeroundday = GameDataInstance()->d_daytime;
		GameData::getSaveData()->_freeround = true;
	}

	if (GameDataInstance()->d_daytime != GameData::getSaveData()->_qd_day)
	{
		if ((GameDataInstance()->d_daytime - GameData::getSaveData()->_qd_day) != 1 || (GameData::getSaveData()->_qd_number == 7))
		{
			GameData::getSaveData()->_qd_number = 0;
			GameData::getInstance()->dataSave();
		}
	}

	if (RANK)
	{
		NetWork::getInstance()->getRankData();
		auto node = Node::create();
		node->retain();
		Director::getInstance()->getScheduler()->schedule([](float ft) {
			cj::LogText("preLoad get RankData");
			if ((RankScene::getInstance()->getUserData(101)&&RankScene::getInstance()->getUserData(102)&&RankScene::getInstance()->getUserData(103))||(!RankScene::getInstance()->getUserData(101) && !RankScene::getInstance()->getUserData(102) && !RankScene::getInstance()->getUserData(103)))
			{
				NetWork::getInstance()->getRankData();
			}
		}, node, RANK_UPDATE_TIME, false, "RankData");
	}

	LogText("preLoad");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("main/tupian.plist", "main/tupian.png");

	std::string lineNameArray[] = { "red", "yellow", "blue", "green", "purple" };
	for (int i = 0; i<5; i++)
	{
		auto path = "animation/daoju_images/skill4_" + lineNameArray[i] + ".png";
		auto spriteframe = SpriteFrame::create(path, Rect(0, 0, 50, 60));
		SpriteFrameCache::getInstance()->addSpriteFrame(spriteframe, path);
	}
	
	//
	//spine
	//
	//auto spinenode1 = SkeletonAnimation::createWithFile(SPINEPATH + "bdms_export/bddaoju_export/export/bddaoju.json", SPINEPATH + "bdms_export/bddaoju_export/export/bddaoju.atlas");
	//auto spinenode2 = SkeletonAnimation::createWithFile(SPINEPATH + "chongpai_Export/export/chongpai.json", SPINEPATH + "chongpai_Export/export/chongpai.atlas");
	//auto spinenode3 = SkeletonAnimation::createWithFile(SPINEPATH + "bdms_export/bing_EXPort/bing.json", SPINEPATH + "bdms_export/bing_EXPort/bing.atlas");
	//auto spinenode4 = SkeletonAnimation::createWithFile(SPINEPATH + "mancao_export/mancao.json", SPINEPATH + "mancao_export/mancao.atlas");
	//auto spinenode5 = SkeletonAnimation::createWithFile(SPINEPATH + "fkms_export/EXPort/fkms.json", SPINEPATH + "fkms_export/EXPort/fkms.atlas");
	//auto people1 = SkeletonAnimation::createWithFile(SPINEPATH + "lost_export/export/lost.json", SPINEPATH + "lost_export/export/lost.atlas");
	//auto people2 = SkeletonAnimation::createWithFile(SPINEPATH + "win_export/win/win.json", SPINEPATH + "win_export/win/win.atlas");
	//auto tishiyu = SkeletonAnimation::createWithFile(SPINEPATH + "tishiyu_export/tishiyu.json", SPINEPATH + "tishiyu_export/tishiyu.atlas");
	//auto par = SkeletonAnimation::createWithFile(SPINEPATH + "bq/skeleton.json", SPINEPATH + "bq/skeleton.atlas");
	//auto spinelizi = SkeletonAnimation::createWithFile(SPINEPATH + "fkms_export/fkms_lizi/fkms_lizi.json", SPINEPATH + "fkms_export/fkms_lizi/fkms_lizi.atlas");
	//char* XIAOCHUCOLOR[5] = {
	//	"blue","green","purple","red","yellow"
	//};
	//for (int i = 0; i < 5; i++)
	//{
	//	char name1[30];
	//	sprintf(name1, "xc/%s.json", XIAOCHUCOLOR[i]);
	//	char name2[30];
	//	sprintf(name2, "xc/%s.atlas", XIAOCHUCOLOR[i]);

	//	auto xiaochu = SkeletonAnimation::createWithFile(SPINEPATH + name1, SPINEPATH + name2);
	//}

	auto file1 = cj::getVideoName("video/LIST.TXT");
	for (auto &i : file1)
	{
		string str = "video/" + i;
		if (i == "music.mp3")
			CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(str.c_str());
		else
			CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(str.c_str());
	}
}

//日志输出
void LogShow(int time,string str)
{
	FILE *file;
	file = fopen("record.txt","a");
	if (file)
	{
		char name[30];
		sprintf(name,"%02d:%02d        ",time / 60,time % 60);
		string stt = name + str +"\n";
		fwrite(stt.c_str(), 1, stt.size(), file);
		fclose(file);
	}
}

void LogShow(string str)
{
	FILE *file;
	file = fopen("record.txt", "a");
	if (file)
	{
		str += "\n";
		fwrite(str.c_str(), 1, str.size(), file);
		fclose(file);
	}
}

//读取csv文件
std::map<int, std::vector<string>> getDataFromCsv(string str)
{
	std::map<int, std::vector<string>> rtdata;
	int row = 0;
	std::vector<string> vecstr;
	string onestr;

	auto file = FileUtils::getInstance()->getDataFromFile(str);
	auto data = file.getBytes();
	auto length = file.getSize();

	for (int i = 0; i < length; i++)
	{
		if (data[i] == ','||data[i]=='\r')
		{
			vecstr.push_back(onestr);
			onestr = "";
		}
		else if (data[i] == '\n')
		{
            vecstr.push_back(onestr);
			if(row>0)
				rtdata.insert(make_pair(row, vecstr));
			vecstr.clear();
			row++;
		}
		else
		{
			onestr.push_back(data[i]);
		}
	}
	return rtdata;

}

std::map<pair<int, char>, string> getDataFromCsv2(string str)
{
	std::map<pair<int,char>, string> rtdata;
	int row = 1;
	char lie = 'A';
	string onestr;

	auto file = FileUtils::getInstance()->getDataFromFile(str);
	auto data = file.getBytes();
	auto length = file.getSize();

	for (int i = 0; i < length; i++)
	{
		if (data[i] == ',' || data[i] == '\r')
		{
			rtdata[make_pair(row,lie)]=onestr;
			onestr = "";
			lie += 1;
		}
		else if (data[i] == '\n')
		{
			row++;
			lie = 'A';
		}
		else
		{
			onestr.push_back(data[i]);
		}
	}
	return rtdata;
}

std::map<int, std::vector<int>> getDataFromReset(string str)
{
	auto file = FileUtils::getInstance()->getDataFromFile(str);
	auto data = file.getBytes();
	auto length = file.getSize();

	std::map<int, std::vector<int>> resdata;
	int key = 1;
	vector<int> vec;
	for (int i = 0; i < length; i++)
	{
		if (data[i] == 'K')
		{
			resdata.insert(make_pair(key,vec));
			key++;
			vec.clear();
		}
		else if (data[i] >= '1'&&data[i] <= '9')
		{
			std::string ss = "";
			ss += data[i];
			vec.push_back(atoi(ss.c_str()));
		}
	}
	return resdata;
}

//string分离
std::vector<int> getIntByString(string str)
{
	vector<int> vec;
	string onestr;
	for (auto &i : str)
	{
		if (i == ' ')
		{
			vec.push_back(atoi(onestr.c_str()));
			onestr = "";
		}
		else
			onestr.push_back(i);
	}
	vec.push_back(atoi(onestr.c_str()));
	return vec;
}

//initdata
void initData()
{
	//

	//	FileUtils::getInstance()->addZpkFile("002.zpk");

	vigame::Thread::setOnRequestAppMainThreadCallback([](std::function<void()> function2)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]()
		{
			function2();
		});
	});

	//
	vigame::MMChnlManager::getInstance()->addMMChnlChangedListener([](vigame::MMChnl* mmChnl)
	{

	});

	//֧
	vigame::pay::PayManager::setPayFeeInfoChangedCallback([]()
	{
	});

	//
	vigame::ad::ADManager::setAddGameCoinCallback([](vigame::ad::ADSourceItem* adSourceItem, int type, int num, std::string reson)
	{
	});

	vigame::pay::PayManager::setOnPayFinishCallback([](vigame::pay::PayParams payParams) {
		int result = payParams.getPayResult();
		int diamondnum = 0;
		if(payParams.getPayId()>=0)
			diamondnum=PAY_DIAMOND[payParams.getPayId()];
		cj::LogText(cjTTFLabel::getNameByInt("%d",result));

		switch (result) {
		case vigame::pay::PAY_RESULT_CANCEL:
			Toast::makeText("PAY_RESULT_CANCEL")->show();
			break;
		case vigame::pay::PAY_RESULT_SUCCESS:
			if (payParams.getPayType() == vigame::pay::PAY_TYPE::PAY_TYPE_AliPay || payParams.getPayType() == vigame::pay::PAY_TYPE::PAY_TYPE_WXPay)
				diamondnum += diamondnum*0.3f;
			GameData::getSaveData()->_diamondNumber += diamondnum;

			payUMCount(payParams.getPayId(), 2);

			if (GameDataInstance()->d_xianshidalibao)
			{
				GameDataInstance()->d_xianshidalibao = false;
				for (int i = 0; i<4; i++)
					GameData::getSaveData()->_propnum[i] += 1;
			}
			else if ((payParams.getPayId() == 0))
			{
				GameData::getSaveData()->_xinshou = false;
				Director::getInstance()->getRunningScene()->removeChildByName("xinshou");
			}

			cjMusic::playEffect("video/coinsin.mp3");

			GameData::getInstance()->dataSave();
			Toast::makeText("PAY_RESULT_SUCCESS")->show();
			break;
		case vigame::pay::PAY_RESULT_FAIL:
			Toast::makeText("PAY_RESULT_FAIL")->show();
			break;
		case vigame::pay::PAY_RESULT_OTHER:
			Toast::makeText("Unknown")->show();
			break;
		}
	});

	//ad init
	vigame::pay::PayManager::init();
	vigame::CoreManager::init();
	vigame::ad::ADManager::init();
	vigame::tj::DataTJManager::init();
	vigame::social::SocialManager::init();
	vigame::share::ShareManager::init();

	vigame::ad::ADManager::openAd("splash");
}

//按钮事件友盟统计
void clickUMCount(int id)
{
    char name1[30];
    switch (id)
    {
        case 1:
            sprintf(name1, "tap_mfcj");			//主页免费抽奖
            break;
        case 2:
            sprintf(name1, "tap_mfzs");			//主页免费钻石
            break;
        case 3:
            sprintf(name1, "tap_rank");			//主页排行榜
            break;
        case 4:
            sprintf(name1, "tap_sign");			//主页签到
            break;
		case 5:
			sprintf(name1, "ui_star");			//经典模式
			break;
		case 6:
			sprintf(name1, "ui_angle");			//六角模式
			break;	
		case 7:
			sprintf(name1, "ui_drop");			//拖动模式
			break;
		case 8:
			sprintf(name1, "ui_challenge");		//方块模式
			break;
		case 9:
			sprintf(name1, "ui_jelly");			//果冻模式
			break;
		case 10:
			sprintf(name1, "star_qhb");			//星星模式抢红包
			break;
		case 11:
			sprintf(name1, "star_mfzs");		//星星模式免费钻石
			break;
		case 12:
			sprintf(name1, "star_relive");		//星星模式钻石复活
			break;
		case 13:
			sprintf(name1, "star_video_relive");//星星模式看视频复活
			break;
		case 14:
			sprintf(name1, "angle_qhb");		//六角模式抢红包
			break;
		case 15:
			sprintf(name1, "angle_mfzs");		//六角模式免费钻石
			break;
		case 16:
			sprintf(name1, "angle_relive");		//六角模式钻石复活
			break;
		case 17:
			sprintf(name1, "angle_video_relive");//六角模式看视频复活
			break;
		case 18:
			sprintf(name1, "drop_relive");		//拖动模式钻石复活
			break;
		case 19:
			sprintf(name1, "drop_video_relive");//拖动模式看视频复活
			break;
		case 20:
			sprintf(name1, "challenge_relive");	//方块模式钻石复活
			break;
		case 21:
			sprintf(name1, "challenge_video_relive");//方块模式看视频复活
			break;
		case 22:
			sprintf(name1, "jelly_relive");		//果冻模式钻石复活
			break;
		case 23:
			sprintf(name1, "jelly_video_relive");//果冻模式看视频复活
			break;
		case 24:
			sprintf(name1, "jelly_guide_ignore");//果冻模式指引跳过
			break;
		case 25:
			sprintf(name1, "fillup_prop1");//果冻模式道具1
			break;
		case 26:
			sprintf(name1, "fillup_prop2");//果冻模式道具2
			break;
		case 27:
			sprintf(name1, "fillup_prop3");//果冻模式道具3
			break;
		case 28:
			sprintf(name1, "jelly_cancel");//果冻模式道具界面关闭
			break;
		case 29:
			sprintf(name1, "jelly_start");//果冻模式开始游戏按钮
			break;
		case 30:
			sprintf(name1, "weixin");	//微信
			break;
        default:
            break;
    }
    vigame::tj::DataTJManager::event(name1, "ButtonClick");
}

//五星好评统计
void starUMCount(int id)
{
    char name1[30];
    //umeng
    std::unordered_map<std::string, std::string> dict;
    switch (id)
    {
        case 1:
            sprintf(name1, "good");
            break;
        case 2:
            sprintf(name1, "bad");
            break;
        case 3:
            sprintf(name1, "refuse");
            break;
        default:
            break;
    }
    dict["click_id"] = name1;
    vigame::tj::DataTJManager::event("comment", dict);
}

//推送统计
void pushUMCount()
{
    //umeng
    std::unordered_map<std::string, std::string> dict;

    dict["click"] = "join";
    vigame::tj::DataTJManager::event("push_click", dict);
}

//支付
void payUMCount(int key, int state)
{
	std::unordered_map<std::string, std::string> dict;
	char name[30];
	sprintf(name, "tap_diamond_00");
	switch (key)
	{
		case 0:
		case 7:
			sprintf(name, "tap_ui_novice");
			break;
		case 1:
			if (GameDataInstance()->d_Project == GAME_START)
				sprintf(name, "tap_star_fast");
			else if (GameDataInstance()->d_Project == GAME_ANGLE)
				sprintf(name, "tap_angle_fast");
			break;
		case 2:
			sprintf(name, "tap_diamond_30");
			break;
		case 3:
			sprintf(name, "tap_diamond_50");
			break;
		case 4:
			sprintf(name, "tap_diamond_100");
			break;
		case 5:
			sprintf(name, "tap_diamond_200");
			break;
		case 6:
			sprintf(name, "tap_ui_limittime");
			break;
		case 9:
			sprintf(name, "tap_diamond_10");
			break;
		default:
			break;
	}

	if (state == 1)
		vigame::tj::DataTJManager::event(name,"Pay_Click");
	else if (state == 2)
	{
		char name2[30];
		sprintf(name2, "%s_success", name);
		vigame::tj::DataTJManager::event(name2, "Pay_Success");
	}
}

//消费虚拟币
void consumeUMCount(int key)
{
	char item_ID[30];
	int price = 10;
	switch (key)
	{
		case 1:
			sprintf(item_ID, "star_bomb");
			break;
		case 2:
			sprintf(item_ID, "star_change");
			break;
		case 3:
			sprintf(item_ID, "star_refush");
			break;
		case 4:
			price = 50;
			sprintf(item_ID, "star_meteor");
			break;
		case 5:
			sprintf(item_ID, "angle_refush");
			break;
		case 6:
			price = 50;
			sprintf(item_ID, "angle_meteor");
			break;
		case 7:
			sprintf(item_ID, "jelly_relive");
			break;
		case 8:
			sprintf(item_ID, "angle_relive");
			break;
		case 9:
			sprintf(item_ID, "star_relive");
			break;
		case 10:
			sprintf(item_ID, "challenge_relive");
			break;
		case 11:
			sprintf(item_ID, "drop_relive");
			break;
		default:
			break;
	}
	vigame::tj::DataTJManager::use(item_ID, 1, price);
}

//关卡分析
void levelUMCount(int level, int state)
{
	char name[30];
	switch (GameDataInstance()->d_Project)
	{
		case GAME_START:
			sprintf(name, "star_%d", level);
			break;
		case GAME_ANGLE:
			sprintf(name, "angle_%d", level);
			break;
		case GAME_1010:
			sprintf(name, "drop_%d", level);
			break;
		case GAME_TETRIS:
			sprintf(name, "challenge_%d", level);
			break;
		case GAME_FILLUP:
			sprintf(name, "jelly_%d", level);
			break;
		default:
			break;
	}
	if (state == 1)
	{
		vigame::tj::DataTJManager::startLevel(name);
	}
	else if (state == 2)
	{
		vigame::tj::DataTJManager::finishLevel(name);
	}
	else if (state == 3)
	{
		vigame::tj::DataTJManager::failLevel(name);
	}
}

//get screen hot
cocos2d::Sprite* screenShot()
{
	auto filepath = FileUtils::getInstance()->getWritablePath() + "pic.png";
	TextureCache::getInstance()->removeTextureForKey(filepath);

	auto sp = Sprite::create(filepath);

	return sp;
}

void captureScene(std::string str)
{
	utils::captureScreen([](bool bo, const std::string str) {

	},str);
	//CCSize size = CCDirector::getInstance()->getWinSize();
	//CCRenderTexture *screen = CCRenderTexture::create(size.width, size.height);
	//CCScene *scene = CCDirector::getInstance()->getRunningScene();
	//screen->begin();
	//scene->visit();//将当前的整个scene绘出来  
	//screen->end();
	//screen->saveToFile(str, kCCImageFormatPNG);
}

//获取文件名称
std::vector<string> getVideoName(std::string str)
{
	auto file = FileUtils::getInstance()->getDataFromFile("video/LIST.TXT");
	auto data = file.getBytes();
	auto size = file.getSize();
	vector<string> name;
	string na = "";
	for (int i = 0; i<size; i++)
	{
		if (data[i] == '\r')
		{
			name.push_back(na);
		}
		else if (data[i] == '\n')
		{
			na.clear();
		}
		else
		{
			na += data[i];
		}
	}
	return name;
}

NAMESPACE_CJ_END
