#include "GameScene.h"
#include "../cj/cjconfig.h"
#include "vigame_ad.h"
//#include "MobClickCpp.h"
#include "../gameconfig/PayScene.h"
#include"../gameconfig/AngleManage.h"
#include "../gameconfig/GameManage.h"
#include "../gameconfig/SceneManage.h"
#include"../gameconfig/StartManage.h"
#include"../gameconfig/FillUpManage.h"
#include"../gameconfig//BaseManage.h"
#include "../cj/Toast.h"
#include "vigame_pay.h"
#include "vigame_tj.h"
#include "cocostudio/CocoStudio.h"
#include "../rank/RankScene.h"
#include "../cj/Shader.h"
using namespace cocostudio;

#include <spine/spine-cocos2dx.h>
#include "spine/spine.h"
using namespace spine;
static const string SPINEPATH = "FillUpMotion/spine/";
static const Vec2 PEOPLEPOS = Vec2(240, 400);

const int uph=783;
const int downh=710;
const float movetime=0.1f;
const int nextboxoffest=110;
const int heightoffest=73;
const int selectboxoffest=136;	//ø…π©—°‘Ò∑ΩøÈµƒ∏ﬂ∂»
const float selectboxscale=0.5f;

GameScene::GameScene(){

}

GameScene::~GameScene(){

}

bool GameScene::init()
{
	if (!Layer::init())
		return false;

	this->setTag(99);

	cjMusic::stopBackgroundMusck();

	GameDataInstance()->d_gameEnter++;

#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	vigame::ad::ADManager::openAd("banner");
#endif

	int umid = 5;
	string csbstr;
	if (GameDataInstance()->d_Project == GAME_START)
	{
		csbstr = "animation/background_star.csb";
		umid = 5;
	}
	else if (GameDataInstance()->d_Project == GAME_ANGLE)
	{
		csbstr = "animation/background_angle.csb";
		umid = 6;
	}
	else if (GameDataInstance()->d_Project == GAME_FILLUP)
	{
		csbstr = "animation/background_fillup.csb";
		umid = 9;
	}
	else if (GameDataInstance()->d_Project == GAME_1010)
	{
		csbstr = "animation/background_1010.csb";
		umid = 7;
	}
	else if (GameDataInstance()->d_Project == GAME_TETRIS)
	{
		csbstr = "animation/background_1010.csb";
		umid = 8;
	}
	cj::clickUMCount(umid);

	_backnode=CSLoader::createNode(csbstr);
	this->addChild(_backnode,1);
	_backnode->setPosition(WINORIGIN);

	auto pause=dynamic_cast<ui::Button*>(_backnode->getChildByName("pause"));
	if (pause)
	{
		pause->setGlobalZOrder(5);
		pause->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType touchType) {
			if (touchType == ui::Widget::TouchEventType::ENDED)
			{
				auto lay = Director::getInstance()->getRunningScene()->getChildByTag(100);
				bool ttt = true;
				if ((GameDataInstance()->d_Project == GAME_FILLUP&&getgTime() <= 1)||GameDataInstance()->_guide)
					ttt = false;
				if (!lay&&ttt)
					pausePage();
			}
		});
	}

	auto diamondadd=dynamic_cast<ui::Button*>(_backnode->getChildByName("zuanshiadd"));
	if(diamondadd)
	{
		diamondadd->setGlobalZOrder(5);
		diamondadd->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType touchType){
			if(touchType==ui::Widget::TouchEventType::ENDED)
			{
				if (!GameDataInstance()->_guide)
				{
					auto lay = Director::getInstance()->getRunningScene()->getChildByTag(100);
					if (!lay)
					{
						cjMusic::playEffect("video/tap.mp3");
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
						if (GameDataInstance()->d_Project == GAME_START)
							cj::clickUMCount(11);
						else if (GameDataInstance()->d_Project == GAME_ANGLE)
							cj::clickUMCount(15);

						PayScene::getInstance()->freeDiamond();
#else
						PayScene::getInstance()->payPage1();
#endif
					}
				}
			}
		});
	}

	//≥ı ºªØ∑÷ ˝£¨µ»º∂£¨ƒø±Íœ‘ æ
	setgScore(0);
	setgLevel(0);
	setgTarget(0);
	setgDiamond(0);

	if (_backnode->getChildByName("level"))
	{
		_levelfont = cjAtlasLabel::createWithInt(0, ATLAS_PATH);
		this->addChild(_levelfont, 1, "levelfont");
		_levelfont->setPosition(_backnode->getChildByName("level")->getPosition());
		_levelfont->setScale(0.9f);
	}

	if (_backnode->getChildByName("mubiao"))
	{
		_targetfont = cjAtlasLabel::createWithInt(0, ATLAS_PATH);
		this->addChild(_targetfont, 1, "targetfont");
		_targetfont->setPosition(_backnode->getChildByName("mubiao")->getPosition() + Vec2(17, 0));
		_targetfont->setScale(0.8f);
	}

	if(_backnode->getChildByName("zuigao"))
	{
		int highsc=0;
		if(GameDataInstance()->d_Project==GAME_START)
			highsc=GameData::getSaveData()->_start._highestscore;
		else if(GameDataInstance()->d_Project==GAME_ANGLE)
			highsc=GameData::getSaveData()->_angle._highestscore;
		else if(GameDataInstance()->d_Project == GAME_FILLUP)
			highsc = GameData::getSaveData()->_fillup._highestscore;
		if (GameDataInstance()->d_Project == GAME_FILLUP)
		{
			_fillupHightScore = highsc;
			_highfont = cjAtlasLabel::createWithInt(highsc, ATLAS_PATH9);
			this->addChild(_highfont, 1, "highfont");
			_highfont->setScale(0.55f);
			_highfont->setAnchorPoint(Vec2(0.0f, 0.5f));
			_highfont->setPosition(_backnode->getChildByName("zuigao")->getPosition() + Vec2(25, -1));
		}
		else
		{
			_highfont = cjAtlasLabel::createWithInt(highsc, ATLAS_PATH);
			this->addChild(_highfont, 1, "highfont");
			_highfont->setPosition(_backnode->getChildByName("zuigao")->getPosition() + Vec2(17, 0));
			_highfont->setScale(0.8f);
		}
	}

	if (_backnode->getChildByName("fenshu"))
	{
		if (GameDataInstance()->d_Project == GAME_FILLUP)
		{
			_scorefont = cjAtlasLabel::createWithInt(0, ATLAS_PATH9);
			this->addChild(_scorefont, 1, "scorefont");

			_scorefont->setPosition(_backnode->getChildByName("fenshu")->getPosition());
		}
		else
		{
			_scorefont = cjAtlasLabel::createWithInt(0, ATLAS_PATH);
			this->addChild(_scorefont, 1, "scorefont");

			_scorefont->setPosition(_backnode->getChildByName("fenshu")->getPosition());

			if (GameDataInstance()->d_Project == GAME_ANGLE || GameDataInstance()->d_Project == GAME_START)
				_scorefont->setScale(1.3f);
			else
				_scorefont->setScale(0.9f);
		}
	}

	if(_backnode->getChildByName("zuanshi"))
	{
		_diamondfont=cjAtlasLabel::createWithInt(0,ATLAS_PATH);
		_diamondfont->setScale(0.8f);
		this->addChild(_diamondfont,1,"diamondfont");
		if(GameDataInstance()->d_Project == GAME_FILLUP)
			_diamondfont->setPosition(_backnode->getChildByName("zuanshi")->getPosition());
		else
			_diamondfont->setPosition(_backnode->getChildByName("zuanshi")->getPosition() + Vec2(12, 0));
	}

	//≥ı ºªØnextbox savebox
	if(GameDataInstance()->d_Project==GAME_TETRIS)
	{
		setgStep(0);
		for(int i=0;i<3;i++)
		{
			_nextBoxPosition[i]=Vec2(WINORIGIN+Vec2(60+nextboxoffest*i,downh));
		}
	}

	if (GameDataInstance()->d_Project == GAME_FILLUP)
	{
		auto help = dynamic_cast<ui::Button*>(_backnode->getChildByName("Button_help"));
		help->addTouchEventListener([=](Ref *rf,ui::Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::BEGAN)
			{
				auto lay = Director::getInstance()->getRunningScene()->getChildByTag(100);
				bool ttt = true;
				if (getgTime() <= 1 || GameDataInstance()->_guide)
					ttt = false;
				if (!lay&&ttt)
				{
					BaseManage *manage = dynamic_cast<BaseManage*>(getChildByName("manage"));
					manage->pause();
					PayScene::getInstance()->helpFillUp();
				}
			}
		});
	}

	rotate=cjMenu::createWithImage("button_1.png",[this](Ref *rf){
		if(GameDataInstance()->d_Project==GAME_TETRIS)
		{
			auto manage=(SceneManage*)this->getChildByName("manage");
			manage->Rotate();
		}
		else if(GameDataInstance()->d_Project==GAME_1010)
		{
			auto manage=(GameManage*)this->getChildByName("manage");
			manage->Rotate();
		}
	});
	this->addChild(rotate,5,"rotate");

	startGame();

	schedule(schedule_selector(GameScene::dateUpdate),0.1f);
	schedule(schedule_selector(GameScene::diamondUpdate));
	
	auto listener=EventListenerKeyboard::create();
	listener->onKeyReleased=CC_CALLBACK_2(GameScene::onKeyReleased,this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

	return true;
}

void GameScene::dateUpdate(float ft)
{
	bool temp=false;
	int score=0;
	int level=1;
	int target=500;
	int stepnumber=0;
	ShapeIndex nbox[3];
	GameState gstate;

	BaseManage *manage =dynamic_cast<BaseManage*>(getChildByName("manage"));
	if (manage)
	{
		temp = true;
		score = manage->getmScore();
		level = manage->getmLevel();
		target = manage->getmTarget();
		stepnumber = manage->getmStepNumber();
		gstate = manage->getGameState();
		for (int i = 0; i<3; i++)
			nbox[i] = manage->getNextBox(i);

		if (GameDataInstance()->d_Project == GAME_FILLUP)
		{
			setgTime(dynamic_cast<FillUpManage*>(manage)->getmTime());
		}
	}

	if(temp)
	{
		if(getgScore()!=score)
		{
			int frontsc = 0;
			int endsc = 0;
			if (getgScore() < score)
			{
				frontsc = getgScore();
				endsc = score;
			}
			setgScore(score);
			float tt=0.6f;
			if (GameDataInstance()->d_Project == GAME_START || GameDataInstance()->d_Project == GAME_ANGLE)
				tt = 0.8f;
			else if (GameDataInstance()->d_Project == GAME_FILLUP)
				tt = 0.0f;
			runAction(Sequence::create(DelayTime::create(tt),CallFunc::create([=](){
				if (endsc == 0)
					_scorefont->setString(cjTTFLabel::getNameByInt("%d", score));
				else
				{
					if (GameDataInstance()->d_Project == GAME_FILLUP)
						_scorefont->setString(cjTTFLabel::getNameByInt("%d", endsc));
					else
						scoreChange(frontsc, endsc);
				}
			}),nullptr));
		}
		if(getgLevel()!=level)
		{
			setgLevel(level);
			_levelfont->setString(cjTTFLabel::getNameByInt("%d",getgLevel()));
		}
		if(getgTarget()!=target)
		{
			setgTarget(target);
			_targetfont->setString(cjTTFLabel::getNameByInt("%d",getgTarget()));
		}

		if(GameDataInstance()->d_Project==GAME_TETRIS&&getgStep()!=stepnumber&&stepnumber>0)
		{
			setgStep(stepnumber);
			if(getgStep()==1)
			{
				for(int i=0;i<3;i++)
				{
					_nextbox[i]=nbox[i];
					auto nextbox=BoxNode::createBox(_nextbox[i],int(_nextbox[i]));
					nextbox->setScale(selectboxscale);
					this->addChild(nextbox,3,15+i);
					nextbox->setPosition(nextbox->getCenterAnPoint(_nextBoxPosition[i]));
				}
			}
			else
			{
				this->removeChildByTag(15);
				for(int i=0;i<2;i++)
				{
					_nextbox[i]=nbox[i];
					auto box=this->getChildByTag(16+i);
					box->setTag(15+i);
					box->runAction(MoveBy::create(movetime,Vec2(-nextboxoffest,0)));
				}
				_nextbox[2]=nbox[2];
				auto nextbox=BoxNode::createBox(_nextbox[2],int(_nextbox[2]));
				nextbox->setScale(selectboxscale);
				this->addChild(nextbox,4,17);
				nextbox->setVisible(false);
				nextbox->setPosition(nextbox->getCenterAnPoint(_nextBoxPosition[2]));
				nextbox->runAction(Sequence::create(DelayTime::create(movetime),Show::create(),nullptr));
			}
		}

		if(STATE_FAILD==gstate)
		{
			runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]() {
				_scorefont->stopAllActions();
				_scorefont->setScale(1.0f);
				_scorefont->setString(cjTTFLabel::getNameByInt("%d", getgScore()));

				cjMusic::stopBackgroundMusck();

				faildPage();
			}), nullptr));
			unschedule(schedule_selector(GameScene::dateUpdate));
		}
	}
}

void GameScene::pausePage()
{
	if(GameDataInstance()->_guide)
		return;

	cj::LogText("pausePage");

	vigame::ad::ADManager::openAd("pause");

	cjMusic::playEffect("video/tap.mp3");

	BaseManage *manage = dynamic_cast<BaseManage*>(getChildByName("manage"));
	manage->pause();

	float ft=0.0f;
	if (GameDataInstance()->d_Project == GAME_FILLUP)
	{
		ft = 0.05f;
		cj::captureScene();
	}

	//截屏发生在当前动作的下一帧执行
	runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([this]() {
		auto layer = Layer::create();
		Director::getInstance()->getRunningScene()->addChild(layer, 10, 100);
		layer->setIgnoreAnchorPointForPosition(false);
		layer->setAnchorPoint(Vec2(0.5f, 0.5f));
		layer->setPosition(WINORIGIN + WINSIZE / 2.0f);

		if (GameDataInstance()->d_Project == GAME_FILLUP)
			addCapture(layer);

		auto colorlay = LayerColor::create(Color4B(0, 0, 0, 255* OPA_BEIJING), WINSIZE.width, WINSIZE.height);
		layer->addChild(colorlay,2);
		colorlay->setPosition(WINORIGIN);

		auto csbnode = CSLoader::createNode("animation/PausePage.csb");
		colorlay->addChild(csbnode);

		char name[30];
		for (int i = 0; i<3; i++)
		{
			if (i == 0)
				sprintf(name, "Button_jx");
			else if (i == 1)
				sprintf(name, "Button_cx");
			else if (i == 2)
				sprintf(name, "Button_fh");

			auto menunode = dynamic_cast<ui::Button*>(csbnode->getChildByName(name));
			menunode->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::BEGAN)
				{
					cjMusic::playEffect("video/tap.mp3");
					menunode->setScale(1.05f);
				}
				else if (type == ui::Widget::TouchEventType::MOVED)
				{
					menunode->setScale(1.0f);
				}
				else if (type == ui::Widget::TouchEventType::ENDED)
				{
					menunode->setScale(1.0f);
					layer->removeFromParent();
					if (i == 0)
						start();
					else if (i == 1)
					{
						cjMusic::stopAllEffect();
						restart();
					}
					else if (i == 2)
					{
						recordStartHighScore();

						cjMusic::stopAllEffect();
						vigame::ad::ADManager::closeAd("banner");

						GameDataInstance()->d_giftfist = false;

						Director::getInstance()->replaceScene(MainScene::createScene());
					}
				}
			});
		}

		auto touchListener = EventListenerTouchOneByOne::create();
		touchListener->setSwallowTouches(true);
		touchListener->onTouchBegan = [](Touch* touch, Event* event) {
			return true;
		};
		layer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, layer);
	}), nullptr));
}

void GameScene::faildPage()
{
	if (GameDataInstance()->_guide)
		return;

	cjMusic::stopAllEffect();
	cjMusic::playEffect("video/faild.mp3");

	vigame::ad::ADManager::openAd("level_fail");

	if (GameDataInstance()->d_Project == GAME_TETRIS)
	{
		for (int i = 0; i < 3; i++)
		{
			auto box = (BoxNode*)getChildByTag(15 + i);
			box->setGray();
		}
	}

	faildlayer = Layer::create();
	Director::getInstance()->getRunningScene()->addChild(faildlayer, 12, "faildlayer");
	faildlayer->setTag(100);

	float ft = 0.0f;
	if (GameDataInstance()->d_Project == GAME_FILLUP)
	{
		ft = 0.05f;		
		cj::captureScene();
	}
	//截屏发生在当前动作的下一帧执行
	runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([this]() {

		if (GameDataInstance()->d_Project == GAME_FILLUP)
			addCapture(faildlayer);

		auto colorlay = LayerColor::create(Color4B(0, 0, 0, 255* 0.7f), WINSIZE.width, WINSIZE.height);
		faildlayer->addChild(colorlay,2);
		colorlay->setPosition(WINORIGIN);

#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
		string csbname = "animation/faildpage_ios.csb";
#else
		string csbname = "animation/faildpage.csb";
#endif
		if (GameDataInstance()->d_Project == GAME_FILLUP)
			csbname = "animation/faildpage_fillup_1.csb";

		auto backcsb = CSLoader::createNode(csbname);
		colorlay->addChild(backcsb,5);
		backcsb->setAnchorPoint(Vec2(0.5f, 0.5f));
		backcsb->setPosition(WINSIZE / 2.0f);

		auto bg = backcsb->getChildByName("bg");

#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)

		auto csbbox = bg->getChildByName("ren");
		auto csbaction = CSLoader::createTimeline("animation/faildpagenode.csb");
		csbbox->runAction(csbaction);
		csbaction->play("UI_fail", true);

		auto jxtg2 = dynamic_cast<ui::Button*>(bg->getChildByName("Button_jxtg2"));
		jxtg2->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::ENDED)
			{

				faildUMCount(1);

				cjMusic::stopAllEffect();
				cjMusic::playEffect("video/tap.mp3");
				if (GameData::getSaveData()->_diamondNumber >= PAY_2_DIAMOND)
				{
					GameData::getSaveData()->_diamondNumber -= PAY_2_DIAMOND;
					faildlayer->removeFromParent();
					resurrection();
				}
				else
				{
					PayScene::getInstance()->freeDiamond();
				}
			}
		});

		auto tip2 = bg->getChildByName("tips2");

		auto mffh = dynamic_cast<ui::Button*>(bg->getChildByName("Button_mffh"));
		mffh->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::ENDED)
			{

				faildUMCount(2);

				auto mffh = dynamic_cast<ui::Button*>(bg->getChildByName("Button_mffh"));
				mffh->setEnabled(false);
				//			auto cha = dynamic_cast<ui::Button*>(bg->getChildByName("cha"));
				//			cha->setEnabled(false);
				auto zsfh = dynamic_cast<ui::Button*>(bg->getChildByName("Button_jxtg2"));
				zsfh->setEnabled(false);

				PayScene::getInstance()->openTip();
				GameDataInstance()->d_iosad = false;

				cjMusic::stopAllEffect();
				cjMusic::playEffect("video/tap.mp3");
				vigame::ad::ADManager::openAd("level_fail_mfzs", [=](vigame::ad::ADSourceItem* pADItem, int result) {
					if (result == vigame::ad::ADSourceItem::open_result::OpenSuccess)
					{
						PayScene::getInstance()->closeTip();
						GameDataInstance()->d_iosad = true;

						PayScene::getInstance()->setPlayAd(true);

						auto falay = Director::getInstance()->getRunningScene()->getChildByName("faildlayer");
						if (falay)
						{
							falay->removeFromParent();
							resurrection();
						}
					}
				});
			}
		});

		mffh->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(ScaleTo::create(0.5f, 0.95f), ScaleTo::create(0.5f, 1.0f))));

		auto pADItem = vigame::ad::ADManager::isAdReady("level_fail_mfzs", "video");
		if (!pADItem || !GameDataInstance()->d_iosad)
		{
			mffh->setVisible(false);

			jxtg2->setPositionX(240);
			tip2->setPositionX(240);
		}
		else
		{
			auto hand = handNode::createHand(0.4f);
			layer->addChild(hand, 2);
			hand->setPosition(mffh->getPosition() + Vec2(50, -20));
	}

#else
		auto jxtg = dynamic_cast<ui::Button*>(bg->getChildByName("Button_jxtg"));
		jxtg->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::BEGAN)
			{
				cjMusic::stopAllEffect();
				cjMusic::playEffect("video/tap.mp3");
				jxtg->setScale(1.1f);
			}
			else if (type == ui::Widget::TouchEventType::MOVED)
			{
				jxtg->setScale(1.0f);
			}
			else if (type == ui::Widget::TouchEventType::ENDED)
			{
				jxtg->setScale(1.0f);

				int key = 12;
				if (GameDataInstance()->d_Project == GAME_START)
					key = 12;
				else if (GameDataInstance()->d_Project == GAME_ANGLE)
					key = 16;
				else if (GameDataInstance()->d_Project == GAME_1010)
					key = 18;
				else if (GameDataInstance()->d_Project == GAME_TETRIS)
					key = 20;
				else if (GameDataInstance()->d_Project == GAME_FILLUP)
					key = 22;
				cj::clickUMCount(key);

				int dianum = PAY_2_DIAMOND;
				if (GameDataInstance()->d_Project == GAME_FILLUP)
					dianum = 20;
				if (GameData::getSaveData()->_diamondNumber >= dianum)
				{
					GameData::getSaveData()->_diamondNumber -= dianum;
					faildlayer->removeFromParent();
					resurrection();
				}
				else
				{
					if (GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo() && vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(4))
						PayScene::getInstance()->payPage1();
					else
					{
						recordStartHighScore();
						faildlayer->removeFromParent();
						vigame::ad::ADManager::closeAd("banner");

						GameDataInstance()->d_giftfist = false;
						Director::getInstance()->replaceScene(MainScene::createScene());
					}
				}
			}
		});

		if (GameDataInstance()->d_Project != GAME_FILLUP)
		{
			auto csbbox = bg->getChildByName("ren");
			auto csbaction = CSLoader::createTimeline("animation/faildpagenode.csb");
			csbbox->runAction(csbaction);
			csbaction->play("UI_fail", true);

			auto csbaction2 = CSLoader::createTimeline("animation/faildpage.csb");
			backcsb->runAction(csbaction2);
			csbaction2->play("loop", true);

			auto hand = handNode::createHand(0.6f);
			colorlay->addChild(hand, 6);
			hand->setPosition(jxtg->getPosition() + Vec2(120, 20));
		}
		else
		{
			auto text8 = dynamic_cast<ui::Text*>(bg->getChildByName("frame")->getChildByName("Text_8"));
			if (text8)
				text8->setString(cjTTFLabel::getNameByInt("%d", getPersent()));

			auto Button_mffh = bg->getChildByName("Button_mffh");
			Button_mffh->setVisible(false);
			//jxtg->setPosition(Vec2(238, 192));
			jxtg->setPosition(Vec2(238, 220));

			auto Text = bg->getChildByName("Text");
			Text->setPosition(Vec2(240, 130));

			jxtg->setScale(1.3f);
			jxtg->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(ScaleTo::create(0.5f, 1.32f), ScaleTo::create(0.5f, 1.28f))));

			auto hand = handNode::createHand(0.4f);
			colorlay->addChild(hand,10);
			hand->setPosition(Vec2(238+WINSIZE.width*0.22f,220));

			//people
			auto people = SkeletonAnimation::createWithFile(SPINEPATH+"lost_export/export/lost.json", SPINEPATH + "lost_export/export/lost.atlas");
			colorlay->addChild(people, 3);
			people->addAnimation(0, "lost_start", false);
			people->addAnimation(1, "lost_loop1", true);
			people->setPosition(PEOPLEPOS);

			auto people2 = SkeletonAnimation::createWithFile(SPINEPATH + "lost_export/export/lost.json", SPINEPATH + "lost_export/export/lost.atlas");
			colorlay->addChild(people2, 3);
			people2->addAnimation(1, "lost_loop2", true);
			people2->setPosition(PEOPLEPOS);

			auto guangquan = Sprite::create(SPINEPATH + "lost_export/guang1.png");
			colorlay->addChild(guangquan, 2);
			guangquan->runAction(RepeatForever::create(RotateBy::create(5.0f,360.0f)));
			guangquan->setPosition(PEOPLEPOS+Vec2(0,150));
		}

#endif
		////

		auto cancel = dynamic_cast<ui::Button*>(bg->getChildByName("cha"));
		cancel->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::BEGAN) {
				PayScene::getInstance()->closeTip();
				if (!rankUp())
				{
					cjMusic::stopAllEffect();
					cjMusic::playEffect("video/tap.mp3");
					faildlayer->removeFromParent();
					if (GameDataInstance()->d_Project == GAME_FILLUP)
					{
						jiesuanPage();
					}
					else
					{
						vigame::ad::ADManager::closeAd("banner");

						GameDataInstance()->d_giftfist = false;
						Director::getInstance()->replaceScene(MainScene::createScene());
					}
				}
				else
				{
					cjMusic::stopAllEffect();
					cjMusic::playEffect("video/tap.mp3");

					faildlayer->removeFromParent();
				}
			}
		});
	}),nullptr));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	faildlayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, faildlayer);
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	int dd = (int)keyCode;
	if (dd == 6)
	{
		auto lay = Director::getInstance()->getRunningScene()->getChildByTag(100);
		if (!lay)
		{
			pausePage();
		}
	}
}

void GameScene::faildUMCount(int id)
{
	int szid = 7;
	int szkey = id;
	if (GameData::getInstance()->d_Project == GAME_TETRIS)
	{
		szid = 10;
		id = 19 + id;
	}
    else if (GameData::getInstance()->d_Project == GAME_1010)
    {
		szid = 11;
		id = 17 + id;
    }
    else if (GameData::getInstance()->d_Project == GAME_START)
    {
		szid = 9;
		id = 11 + id;
    }
    else if (GameData::getInstance()->d_Project == GAME_ANGLE)
    {
		szid = 8;
		id = 15 + id;
    }
	else if (GameData::getInstance()->d_Project == GAME_FILLUP)
	{
		szid = 7;
		id = 21 + id;
	}
	cj::clickUMCount(id);
	if (szkey == 1)
		cj::consumeUMCount(szid);
}

void GameScene::start()
{
	BaseManage *manage = dynamic_cast<BaseManage*>(getChildByName("manage"));
	manage->start();
}

void GameScene::resurrection()
{
	if (GameDataInstance()->d_Project == GAME_TETRIS)
	{
		for (int i = 0; i<3; i++)
		{
			auto box = (BoxNode*)getChildByTag(15 + i);
			box->setNormal();
		}
	}

	BaseManage *manage = dynamic_cast<BaseManage*>(getChildByName("manage"));
	manage->Resurrection();

	if(GameDataInstance()->d_Project==GAME_START)
	{
		int kk=manage->getmScore();
		if(getgScore()!=kk)
		{
			setgScore(kk);
			_scorefont->setString(cjTTFLabel::getNameByInt("%d",kk));
		}
	}
	runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([this](){
		schedule(schedule_selector(GameScene::dateUpdate),0.1f);
	}),nullptr));
}

void GameScene::restart()
{
	recordStartHighScore();

	if (_backnode->getChildByName("zuigao"))
	{
		int highsc = 0;
		if (GameDataInstance()->d_Project == GAME_START)
			highsc = GameData::getSaveData()->_start._highestscore;
		else if (GameDataInstance()->d_Project == GAME_ANGLE)
			highsc = GameData::getSaveData()->_angle._highestscore;
		else if (GameDataInstance()->d_Project == GAME_FILLUP)
			highsc = GameData::getSaveData()->_fillup._highestscore;
		_highfont->setString(cjTTFLabel::getNameByInt("%d", highsc));
	}

	int umid = 9;

	setgLevel(0);
	setgTarget(0);
	setgScore(-1);
	setgStep(0);
	if (GameDataInstance()->d_Project != GAME_START)
	{
		((LabelAtlas*)getChildByName("scorefont"))->setString("0");
	}
	if(GameDataInstance()->d_Project==GAME_1010)
	{
		umid = 7;
		GameData::getSaveData()->_onezero._istip=false;
		removeChildByName("manage");
		auto manage=GameManage::create();
		this->addChild(manage,1,"manage");
		manage->run();
	}
	else if(GameDataInstance()->d_Project==GAME_START)
	{
		umid = 5;
		GameData::getSaveData()->_start._istip=false;
		removeChildByName("manage");
		auto manage=StartManage::create();
		this->addChild(manage,1,"manage");
		manage->run();
	}
	else if(GameDataInstance()->d_Project==GAME_TETRIS)
	{
		umid = 8;
		if(GameDataInstance()->d_Model==MODEL_POINT)
			GameData::getSaveData()->_challengesstetris._istip=false;
		removeChildByName("manage");
		auto manage=SceneManage::create();
		this->addChild(manage,1,"manage");
		manage->run();
		for(int i=0;i<3;i++)
			this->removeChildByTag(15+i);
	}
	else if(GameDataInstance()->d_Project==GAME_ANGLE)
	{
		umid = 6;
		GameData::getSaveData()->_angle._istip=false;
		removeChildByName("manage");
		auto manage=AngleManage::create();
		this->addChild(manage,1,"manage");
		manage->run();
	}
	else if (GameDataInstance()->d_Project == GAME_FILLUP)
	{
		umid = 9;

		GameData::getInstance()->d_restart = true;
		PayScene::getInstance()->fillupProp();
	}
	cj::clickUMCount(umid);
	GameData::getInstance()->dataSave();
}

//º«¬º–«–«◊Ó∏ﬂ∑÷
void GameScene::recordStartHighScore()
{
	if(GameDataInstance()->d_Project==GAME_START)
	{
		if(getgScore()>GameData::getSaveData()->_start._highestscore)
		{
			GameData::getSaveData()->_start._highestscore=getgScore();
			GameData::getInstance()->dataSave();
		}
		if(RankScene::getInstance()->getUserData(101)&&(getgScore()>RankScene::getInstance()->getUserData(101)->score))
			RankScene::getInstance()->sumbitUserScore(getgScore(), 101);
	}
	else if(GameDataInstance()->d_Project==GAME_ANGLE)
	{
		if(getgScore()>GameData::getSaveData()->_angle._highestscore)
		{
			GameData::getSaveData()->_angle._highestscore=getgScore();
			GameData::getInstance()->dataSave();
		}
		if (RankScene::getInstance()->getUserData(102)&&(getgScore()>RankScene::getInstance()->getUserData(102)->score))
			RankScene::getInstance()->sumbitUserScore(getgScore(), 102);
	}
	else if (GameDataInstance()->d_Project == GAME_FILLUP)
	{
		if (getgScore()>GameData::getSaveData()->_fillup._highestscore)
		{
			GameData::getSaveData()->_fillup._highestscore = getgScore();
			GameData::getInstance()->dataSave();
		}
		if (RankScene::getInstance()->getUserData(103) && (getgScore()>RankScene::getInstance()->getUserData(103)->score))
			RankScene::getInstance()->sumbitUserScore(getgScore(), 103);
	}
}

void GameScene::startGame()
{
	BaseManage *manage;

	if(GameDataInstance()->d_Project==GAME_TETRIS)
	{
		rotate->setPosition(WINORIGIN+Vec2(415,downh));
		manage=SceneManage::create();
	}
	else if(GameDataInstance()->d_Project==GAME_1010)
	{
		rotate->setVisible(false);
		manage = GameManage::create();
	}
	else if(GameDataInstance()->d_Project==GAME_START)
	{
		rotate->setVisible(false);
		manage=StartManage::create();
	}
	else if(GameDataInstance()->d_Project==GAME_ANGLE)
	{
		rotate->setVisible(false);
		manage=AngleManage::create();
	}
	else if (GameDataInstance()->d_Project == GAME_FILLUP)
	{
		rotate->setVisible(false);
		manage = FillUpManage::create();
	}
	this->addChild(manage, 4, "manage");
	manage->run();
}

//∑÷ ˝±‰ªØ
void GameScene::scoreChange(int frontsc, int endsc)
{
	_scorefont->stopAllActions();
	_scorefont->setString(cjTTFLabel::getNameByInt("%d", frontsc));

	static int addnum = 1;
	addnum = 1;
	int addsc=1;
	int dissc = endsc - frontsc;
	int round = dissc;
	if (dissc >= 60)
	{
		addsc = (endsc - frontsc) / 60;
		round = 60;
	}

	Vector<FiniteTimeAction*> action;
	for (int i = 1; i <= round; i++)
	{
		action.pushBack(CallFunc::create([=]() {
			if (addnum == round)
				_scorefont->setString(cjTTFLabel::getNameByInt("%d", endsc));
			else
				_scorefont->setString(cjTTFLabel::getNameByInt("%d", frontsc + addnum*addsc));
			addnum++;
		}));
		action.pushBack(DelayTime::create(1 / 60.0f));
	}
	if(action.size()>0)
		_scorefont->runAction(Sequence::create(action));

	float ff;
	if (GameDataInstance()->d_Project == GAME_ANGLE || GameDataInstance()->d_Project == GAME_START)
		ff = 1.3f;
	else
		ff = 0.9f;
	_scorefont->runAction(Sequence::create(ScaleTo::create(0.5f,ff*1.3f),ScaleTo::create(0.5f,ff),nullptr));
}

//◊Í Ø±‰ªØ
void GameScene::diamondChange(int frontsc, int endsc)
{
	static int addnum = 1;
	int addsc = 1;
	int dissc = endsc - frontsc;
	int round = dissc;
	if (dissc >= 60)
	{
		addsc = (endsc - frontsc) / 60;
		round = 60;
	}

	Vector<FiniteTimeAction*> action;
	for (int i = 1; i <= round; i++)
	{
		action.pushBack(CallFunc::create([=]() {
			if (addnum == round)
				_diamondfont->setString(cjTTFLabel::getNameByInt("%d", endsc));
			else
				_diamondfont->setString(cjTTFLabel::getNameByInt("%d", frontsc + addnum*addsc));
			addnum++;
		}));
		action.pushBack(DelayTime::create(1 / 60.0f));
	}
	if(action.size()>0)
		_scorefont->runAction(Sequence::create(action));
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	//_wavenum++;
	//_gridnode->runAction(Sequence::create(Ripple3D::create(1.5f, CCSizeMake(32, 24), touch->getLocation(), 300, 4, 30),
	//	CallFunc::create([this]() {
	//	_wavenum--;
	//	if (_wavenum == 0)
	//		_gridnode->runAction(Ripple3D::create(0.1f, CCSizeMake(32, 24), Vec2(0, 0), 10, 1, 0));
	//}), nullptr));
	return false;
}

void GameScene::onTouchesMoved(Touch *touch, Event *unused_event)
{}

void GameScene::onTouchEnded(Touch *touch, Event *unused_event)
{}

//钻石刷新
void GameScene::diamondUpdate(float ft)
{
	if (_backnode->getChildByName("zuanshi") && GameData::getSaveData()->_diamondNumber != getgDiamond())
	{
		_diamondfont->setString(cjTTFLabel::getNameByInt("%d", GameData::getSaveData()->_diamondNumber));
		setgDiamond(GameData::getSaveData()->_diamondNumber);
        GameData::getInstance()->dataSave();
	}
}

//排行升段
bool GameScene::rankUp()
{
	if (GameDataInstance()->d_Project == GAME_START||GameDataInstance()->d_Project == GAME_ANGLE|| GameDataInstance()->d_Project == GAME_FILLUP)
	{
		int type = 101;
		if (GameDataInstance()->d_Project == GAME_ANGLE)
			type = 102;
		else if (GameDataInstance()->d_Project == GAME_FILLUP)
			type = 103;

		int frontorder = 1;
		auto ziji = RankScene::getInstance()->getUserData(type);
		if (ziji)
			frontorder = ziji->order;

		recordStartHighScore();

		if (ziji)
		{
			RankScene::getInstance()->rankSort(type);
			int lateorder = RankScene::getInstance()->getUserData(type)->order;
			if ((lateorder < frontorder) && RankScene::getInstance()->getUserData(lateorder + 1, type))
			{
				rankUpUI(lateorder + 1, type, frontorder - lateorder,true);
				return true;
			}
			else if ((lateorder > frontorder) && RankScene::getInstance()->getUserData(lateorder - 1, type))
			{
				rankUpUI(lateorder -1, type, lateorder-frontorder, false);
				return true;
			}
		}

		//if (RankScene::getInstance()->getUserData(type)&&getgScore()<=RankScene::getInstance()->getUserData(type)->score)
		//{
		//	int frontorder = RankScene::getInstance()->getUserData(type)->order;
		//	RankScene::getInstance()->rankSort(type);
		//	int lateorder = RankScene::getInstance()->getUserData(type)->order;
		//	if ((lateorder < frontorder)&& RankScene::getInstance()->getUserData(lateorder+1, type))
		//	{
		//		rankUpUI(lateorder + 1, type, frontorder - lateorder);
		//		return true;
		//	}
		//}
	}
	return false;
}

void GameScene::rankUpUI(int order, int type, int uporder, bool upOrDown)
{
	auto colorlay = LayerColor::create(Color4B(0, 0, 0, 255 * OPA_BEIJING), WINSIZE.width, WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay, 100, 100);
	colorlay->setPosition(WINORIGIN);

	auto backcsb = CSLoader::createNode("animation/RankPage2.csb");
	colorlay->addChild(backcsb);
	backcsb->setPosition(WINORIGIN);

	auto upcsb = CSLoader::createNode("FillUpMotion/Rank.csb");
	colorlay->addChild(upcsb, 1);
	upcsb->setPosition(Vec2(WINSIZE/2.0f)+Vec2(0,30));

	colorlay->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=]() {
		auto upaction = CSLoader::createTimeline("FillUpMotion/Rank.csb");
		upcsb->runAction(upaction);
		upaction->play("updown", false);
		upaction->setLastFrameCallFunc([=]() {
			upaction->play("loop", true);
		});
	}), nullptr));

	auto ziji = RankScene::getInstance()->rankBanner2(*RankScene::getInstance()->getUserData(type), 2);
	auto other = RankScene::getInstance()->rankBanner2(*RankScene::getInstance()->getUserData(order, type),1);

	auto bar_up = dynamic_cast<Sprite*>(upcsb->getChildByName("bar_up"));
	auto bar_down= dynamic_cast<Sprite*>(upcsb->getChildByName("bar_down"));

	if (upOrDown)
	{
		bar_up->addChild(ziji);
		ziji->setPosition(bar_up->getContentSize() / 2.0f);

		bar_down->addChild(other);
		other->setPosition(bar_down->getContentSize() / 2.0f);

		auto Text_down = backcsb->getChildByName("Text_down");
		Text_down->setVisible(false);

		auto downarrow = Sprite::create("animation/Rank/down.png");
		other->addChild(downarrow);
		downarrow->setPosition(Vec2(360, 50));
	}
	else
	{
		bar_up->addChild(other);
		other->setPosition(bar_up->getContentSize() / 2.0f);

		bar_down->addChild(ziji);
		ziji->setPosition(bar_down->getContentSize() / 2.0f);

		auto Text_down = backcsb->getChildByName("Text_up");
		Text_down->setVisible(false);

		auto downarrow = Sprite::create("animation/Rank/down.png");
		ziji->addChild(downarrow);
		downarrow->setPosition(Vec2(360, 50));
	}

	auto font = cjAtlasLabel::createWithInt(0, ATLAS_PATH6);
	colorlay->addChild(font);
	font->setPosition(Vec2(310, 217));
    PayScene::getInstance()->fontAddMotion(font, uporder);

	auto fanhui = dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_Fanhui"));
	fanhui->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
			fanhui->setScale(1.1f);
		else if (type == ui::Widget::TouchEventType::MOVED)
			fanhui->setScale(1.0f);
		else if (type == ui::Widget::TouchEventType::ENDED) {
			fanhui->setScale(1.0f);
			GameDataInstance()->d_giftfist = false;
			Director::getInstance()->replaceScene(MainScene::createScene());
		}
	});

	auto jixu = dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_Jixu"));
	jixu->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
			jixu->setScale(1.1f);
		else if (type == ui::Widget::TouchEventType::MOVED)
			jixu->setScale(1.0f);
		else if (type == ui::Widget::TouchEventType::ENDED) {
			jixu->setScale(1.0f);
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
			restart();
            
            runAction(Sequence::create(DelayTime::create(0.2f),CallFunc::create([this](){
                schedule(schedule_selector(GameScene::dateUpdate),0.1f);
            }),nullptr));
        }
    });

	auto reward = dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_reward"));
	reward->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED) {
			cjMusic::playEffect("video/tap.mp3");
			GameDataInstance()->d_giftfist = false;
			GameDataInstance()->d_rank = true;
			Director::getInstance()->replaceScene(MainScene::createScene());
		}
	});

	auto action = CSLoader::createTimeline("animation/RankPage2.csb");
	backcsb->runAction(action);
	action->play("reward", true);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//加模糊遮罩
void GameScene::addCapture(Layer *layer)
{

	return;
	auto sp = cj::screenShot();
	if (sp)
	{
		ShaderEffect::SpriteBlur(sp);

		auto viewScaleX = Director::getInstance()->getOpenGLView()->getScaleX();
		auto viewScaleY = Director::getInstance()->getOpenGLView()->getScaleY();

#if(CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
		sp->setScale(1.0f / 0.7f);
#else
		//sp->setScaleX(1.0f / viewScaleX);
		//sp->setScaleY(1.0f / viewScaleY);
#endif
		//sp->setScale(0.5f);

		layer->addChild(sp);
		sp->setAnchorPoint(Vec2(0.0f, 0.0f));
		sp->setPosition(WINORIGIN);
	}
	else
	{
		layer->runAction(Sequence::create(DelayTime::create(1.0f/60.0f), CallFunc::create([=]() {
			addCapture(layer);
		}), nullptr));
	}
}

//结算界面
void GameScene::jiesuanPage()
{
	auto layer = Layer::create();
	Director::getInstance()->getRunningScene()->addChild(layer, 10, 100);
	layer->setIgnoreAnchorPointForPosition(false);
	layer->setAnchorPoint(Vec2(0.5f, 0.5f));
	layer->setPosition(WINORIGIN + WINSIZE / 2.0f);

	auto colorlay = LayerColor::create(Color4B(0, 0, 0, 255 * OPA_BEIJING), WINSIZE.width, WINSIZE.height);
	layer->addChild(colorlay, 2);
	colorlay->setPosition(WINORIGIN);

	string csbname = "animation/faildpage_fillup_2.csb";
	auto csbnode = CSLoader::createNode(csbname);
	colorlay->addChild(csbnode,5);

	auto bg = csbnode->getChildByName("bg");

	//people
	auto people = SkeletonAnimation::createWithFile(SPINEPATH + "win_export/win/win.json", SPINEPATH + "win_export/win/win.atlas");
	colorlay->addChild(people, 3);
	people->addAnimation(0, "win_start", false);
	people->addAnimation(1, "win_loop", true);
	people->setPosition(PEOPLEPOS);

	for (int i = 0; i < 2; i++)
	{
		int k = 2;
		int h = 150;
		if (i == 1)
		{
			k = 5;
			h = 50;
		}
		auto winpar = ParticleSystemQuad::create(SPINEPATH + cjTTFLabel::getNameByInt("win_export/lizi/win%d.plist", i + 1));
		colorlay->addChild(winpar, k);
		winpar->setAutoRemoveOnFinish(true);
		winpar->setPosition(PEOPLEPOS+Vec2(0,h));
	}

	auto guangquan = Sprite::create(SPINEPATH + "win_export/guangquan.png");
	colorlay->addChild(guangquan, 2);
	guangquan->setPosition(PEOPLEPOS+Vec2(0,150));
	guangquan->runAction(RepeatForever::create(RotateBy::create(5.0f, 360.0f)));

	auto score = cjAtlasLabel::createWithInt(0, ATLAS_PATH13);
	bg->addChild(score);
	score->setPosition(bg->getChildByName("score")->getPosition());

	int k = getgScore();
	if (getgScore() > _fillupHightScore)
	{
		runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]() {
			auto csbaction = CSLoader::createTimeline(csbname);
			csbnode->runAction(csbaction);
			csbaction->play("score_top", false);
		}), nullptr));
	}

	if(getgScore()>0)
		PayScene::getInstance()->fontAddMotion(score, getgScore(),30);

	auto jxtg = dynamic_cast<ui::Button*>(bg->getChildByName("Button_jxtg"));
	jxtg->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			jxtg->setScale(1.1f);
		}
		else if (type == ui::Widget::TouchEventType::MOVED)
		{
			jxtg->setScale(1.0f);
		}
		else if (type == ui::Widget::TouchEventType::ENDED)
		{
			jxtg->setScale(1.0f);
			if (GameDataInstance()->d_Project == GAME_FILLUP)
			{
				layer->removeFromParent();
				restart();

				runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([this]() {
					schedule(schedule_selector(GameScene::dateUpdate), 0.1f);
				}), nullptr));
			}
		}
	});

	auto cancel = dynamic_cast<ui::Button*>(bg->getChildByName("cha"));
	cancel->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN) 
		{
			cjMusic::playEffect("video/tap.mp3");

			layer->removeFromParent();
			vigame::ad::ADManager::closeAd("banner");

			GameDataInstance()->d_giftfist = false;
			Director::getInstance()->replaceScene(MainScene::createScene());
		}
	});

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};
	layer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, layer);
}

//获取百分比
int GameScene::getPersent()
{ 
	int k = 99;
	int order = 0;
	bool tt = true;
	int orderlevel[5] = { 3,10,50,100,500 };
	int orderk[5] = { 99,95,85,75,60 };
	do
	{
		InfoData* key = RankScene::getInstance()->getUserData(orderlevel[order],103);
		if (key)
		{
			if((getgScore() >= key->score) || (order == 4))
			{ 
				k = orderk[order];
				tt = false;
			}
		}
		else
		{
			k = orderk[order];
			tt = false;
		}
		order++;
	} while (tt);
	return k;
}
