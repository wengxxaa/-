#include"MainScene.h"
//#include "MobClickCpp.h"
#include "vigame_ad.h"
#include "vigame_pay.h"
#include "vigame_core.h"
#include "../gameconfig/PayScene.h"
#include "../gameconfig/PayDhm.h"
#include "../rank/RankScene.h"
#include "../rank/Login.h"
#include "../rank/NetWork.h"
#include "../cj/OffersHelper.h"

#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

#include"../cj/Toast.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS|| IOSMODE)
void MainScene::checkVideoState(float dt) {
	auto pADItem = vigame::ad::ADManager::isAdReady("home_mfzs","video");
	auto zuanshi=this->getChildByName("csb")->getChildByName("Button_mfzs");
	if (pADItem&&GameData::getSaveData()->_freediamondnum<10&&GameDataInstance()->d_iosad) {
		if (zuanshi->isVisible() == false) {
			zuanshi->setVisible(true);
		}
	}
	else {
		zuanshi->setVisible(false);
	}
}
#endif

void MainScene::onEnter()
{
	Layer::onEnter();

	cj::LogText("onEnter");

	if (GameDataInstance()->d_giftfist)
	{
		this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]() {
			auto lay = Director::getInstance()->getRunningScene()->getChildByTag(100);
			if (!lay)
			{
				int key = vigame::pay::PayManager::getGiftCtrlFlagUse(3);
				if (key == 1 && GameData::getSaveData()->_xinshou&& GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo() && vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(4))
					PayScene::getInstance()->payPage4();
				else if (key == 2)
					PayScene::getInstance()->payPage1();
				else if (key == 3)
					PayScene::getInstance()->payPage3();

				cj::LogText(cjTTFLabel::getNameByInt("onEnter:%d", key));
			}
		}), nullptr));
	}
	else
	{
		GameDataInstance()->d_giftfist = true;
	}
}

bool MainScene::init()
{
	if(!Layer::init())
		return false;

    this->setTag(1);
    
    cj::LogText("MainScene");

    vigame::ad::ADManager::closeAd("banner");

	cjMusic::playBackgroundMusic("video/music.mp3", true);

	_xieyi = true;

	GameDataInstance()->main_num++;
	GameDataInstance()->d_PayType=0;
	GameDataInstance()->d_PayType= vigame::pay::PayManager::getDefaultPayType();
	GameDataInstance()->_guide=false;

	string csbname;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS|| IOSMODE)
	csbname = "animation/mainpage_ios.csb";
#else
	csbname = "animation/mainpage.csb";
#endif

	auto backcsb=CSLoader::createNode(csbname);
	this->addChild(backcsb,1,"csb");
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	string namestr[4]={
		"Button_dj",
		"Button_lj",
		"Button_td",
		"Button_ts"
	};

	for(int i=0;i<4;i++)
	{
		auto menu=dynamic_cast<ui::Button*>(backcsb->getChildByName(namestr[i]));
		menu->addTouchEventListener([=](Ref*,ui::Widget::TouchEventType type){
			if(type==ui::Widget::TouchEventType::BEGAN)
			{
				cjMusic::playEffect("video/tap.mp3");
				menu->setScale(1.05f);
			}
			else if(type==ui::Widget::TouchEventType::MOVED)
				menu->setScale(1.0f);
			else if(type==ui::Widget::TouchEventType::ENDED)
			{
				menu->setScale(1.0f);

				if(i==0)
					GameData::getInstance()->d_Project=GAME_START;
					//GameData::getInstance()->d_Project = GAME_FILLUP;
				else if(i==1)
					GameData::getInstance()->d_Project=GAME_ANGLE;
				else if(i==2)
					GameData::getInstance()->d_Project=GAME_1010;
				else if(i==3)
				{
					GameData::getInstance()->d_Model=MODEL_POINT;
					GameData::getInstance()->d_Project=GAME_TETRIS;
				}
				if(_xieyi)
				{
					if(GameData::getSaveData()->_xieyi)
					{
						GameData::getSaveData()->_xieyi=false;
						GameData::getInstance()->dataSave();
					}
	
					//进入游戏
					GameData::getInstance()->d_time=time(NULL);
					if (GameData::getInstance()->d_Project == GAME_FILLUP&&GameData::getSaveData()->_fillup._guider)
					{
						GameData::getInstance()->d_restart = false;
						PayScene::getInstance()->fillupProp();
					}
					else
						Director::getInstance()->replaceScene(GameScene::createScene());
				}
				else
				{
					Toast::makeText("xieyi")->show();
				}
			}
		});

		//if (i == 0)
		//{
		//	auto xin = Sprite::create("FillUp/box/new.png");
		//	this->addChild(xin, 20);
		//	xin->setPosition(menu->getPosition() + Vec2(125, 10));

		//	menu->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(ScaleTo::create(0.5f, 1.02f), ScaleTo::create(0.5f, 0.98f))));
		//}

		//if (i == 3)
		//	menu->setVisible(false);
		//else
		//{
		//	menu->setPosition(menu->getPosition() - Vec2(0,20+10*i));
		//}
	}

#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
	this->schedule(schedule_selector(MainScene::checkVideoState));

	auto Button_mfzs= dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_mfzs"));
	Button_mfzs->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			PayScene::getInstance()->turnTip();
		}
	});

	Button_mfzs->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(RotateTo::create(0.2f,5.0f),RotateTo::create(0.2f,-5.0f))));
	Button_mfzs->setVisible(false);

#else
	auto shop=dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_shop"));
	shop->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			shop->setScale(1.1f);
		}
		else if (type == ui::Widget::TouchEventType::MOVED)
			shop->setScale(1.0f);
		else if (type == ui::Widget::TouchEventType::ENDED)
		{
			shop->setScale(1.0f);
			PayScene::getInstance()->payPage2();
		}
	});

	auto dhm=dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_dhm"));
	dhm->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			dhm->setScale(MENU_SCALE);
		}
		else if (type == ui::Widget::TouchEventType::MOVED)
			dhm->setScale(1.0f);
		else if(type==ui::Widget::TouchEventType::ENDED)
		{
			dhm->setScale(1.0f);
			PayDhm::CreateLayer();
		}
	});

	auto xslb=dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_xslb"));
	xslb->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			xslb->setScale(1.1f);
		}
		else if (type == ui::Widget::TouchEventType::MOVED)
			xslb->setScale(1.0f);
		else if(type==ui::Widget::TouchEventType::ENDED)
		{
			xslb->setScale(1.0f);
			PayScene::getInstance()->payPage3();
		}
	});
	//xslb->setVisible(false);
	if(GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo()&& vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(6))
		xslb->setVisible(true);
	else
		xslb->setVisible(false);

	auto lb=dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_lb"));
	lb->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			lb->setScale(MENU_SCALE);
		}
		else if (type == ui::Widget::TouchEventType::MOVED)
			lb->setScale(1.0f);
		else if(type==ui::Widget::TouchEventType::ENDED)
		{
			lb->setScale(1.0f);
			PayScene::getInstance()->payPage4();
		}
	});

	auto lbmotion = CSLoader::createTimeline("animation/mainpage.csb");
	backcsb->runAction(lbmotion);
	lbmotion->play("xslbmotion", true);

	if(GameData::getSaveData()->_xinshou&&GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo()&&(vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(0)||vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(7)))
	{
		lb->setVisible(true);
	}
	else
		lb->setVisible(false);

	this->schedule(schedule_selector(MainScene::xinshouUpdate), 0.1f);

	//用户协议
	auto xieyi=backcsb->getChildByName("XieYi");
	if(GameData::getSaveData()->_xieyi)
	{
		auto xiyi_3=dynamic_cast<ui::Button*>(xieyi->getChildByName("Button_xiyi_3"));
		xiyi_3->addTouchEventListener([](Ref *,ui::Widget::TouchEventType type){
			if (type == ui::Widget::TouchEventType::ENDED)
			{
				cjMusic::playEffect("video/tap.mp3");
				vigame::UserAgreement::open();
			}
		});

		auto xiyi_5=xieyi->getChildByName("xiyi_5");

		auto xiyi_4=dynamic_cast<ui::Button*>(xieyi->getChildByName("Button_xiyi_4"));
		xiyi_4->addTouchEventListener([xiyi_5,this](Ref *,ui::Widget::TouchEventType type){
			if(type==ui::Widget::TouchEventType::BEGAN)
			{
				cjMusic::playEffect("video/tap.mp3");
				if(xiyi_5->isVisible())
				{
					xiyi_5->setVisible(false);
					_xieyi=false;
				}
				else
				{
					xiyi_5->setVisible(true);
					_xieyi=true;
				}
			}
		});
	}
	else
		xieyi->setVisible(false);
	//

	//更多游戏
	auto gdyx=dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_gdyx"));
	if (gdyx)
	{
		gdyx->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::BEGAN)
			{
				cjMusic::playEffect("video/tap.mp3");
				gdyx->setScale(MENU_SCALE);
			}
			else if (type == ui::Widget::TouchEventType::MOVED)
				gdyx->setScale(1.0f);
			else if (type == ui::Widget::TouchEventType::ENDED)
			{
				gdyx->setScale(1.0f);
				vigame::pay::PayManager::openMoreGame();
			}
		});
		gdyx->setVisible(false);

		if (cjTTFLabel::getStringByKey("EGAME") == "true")
		{
			if (vigame::pay::PayManager::isMoreGame())
			{
				gdyx->setVisible(true);
			}
		}

		//
#endif
	}

	//签到
	auto qd = dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_qd"));
	if (qd)
	{
		qd->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
			auto lay = Director::getInstance()->getRunningScene()->getChildByTag(100);
			if (!lay)
			{
				if (type == ui::Widget::TouchEventType::BEGAN)
				{
					cjMusic::playEffect("video/tap.mp3");
					qd->setScale(MENU_SCALE);
				}
				else if (type == ui::Widget::TouchEventType::MOVED)
					qd->setScale(1.0f);
				else if (type == ui::Widget::TouchEventType::ENDED)
				{
					qd->setScale(1.0f);
					cj::clickUMCount(4);
					PayScene::getInstance()->dailyClick();
				}
			}
		});
	}
	//

	//自动弹框
	openVipGift();

#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS||IOSMODE)
	//	//大转�?
	auto turn=dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_turn"));
	turn->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			turn->setScale(1.0f);
		}
		else if (type == ui::Widget::TouchEventType::MOVED)
			turn->setScale(0.9f);
		else if(type==ui::Widget::TouchEventType::ENDED)
		{
			turn->setScale(0.9f);
			GameDataInstance()->d_jointurn = false;
			PayScene::getInstance()->turnTable();
		}
	});
    turn->setScale(0.9f);
#endif
	//

	//活动
	auto Action= dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_Action"));
	Action->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cj::LogText("Action");
			cjMusic::playEffect("video/tap.mp3");
			Action->setScale(1.1f);
		}
		else if (type == ui::Widget::TouchEventType::MOVED)
			Action->setScale(1.0f);
		else if (type == ui::Widget::TouchEventType::ENDED)
		{
			Action->setScale(1.0f);
			vigame::activity::open();
		}
	});
    Action->setVisible(false);
#if(CC_TARGET_PLATFORM!=CC_PLATFORM_IOS&&!IOSMODE)
	if (vigame::activity::getUrl().size() != 0)
		Action->setVisible(true);
#endif

	//排行�?
	auto Button_rank = dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_rank"));
	if (Button_rank)
	{
		Button_rank->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::BEGAN)
			{
				cjMusic::playEffect("video/tap.mp3");
				Button_rank->setScale(1.1f);
			}
			else if (type == ui::Widget::TouchEventType::MOVED)
				Button_rank->setScale(1.0f);
			else if (type == ui::Widget::TouchEventType::ENDED)
			{
				Button_rank->setScale(1.0f);
				RankScene::getInstance()->rankMainPage();
			}
		});
		if (!RANK)
			Button_rank->setVisible(false);
	}

	//微信登录
	_wxloadclick = false;
	auto Wxload = dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_Wxload"));
	if (Wxload)
	{
		Wxload->setVisible(false);
		#if(CC_TARGET_PLATFORM!=CC_PLATFORM_ANDROID)
			Wxload->setVisible(false);
		#else
		    if (Wxload)
		    {
		        Wxload->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
					if (type == ui::Widget::TouchEventType::BEGAN)
					{
						cjMusic::playEffect("video/tap.mp3");
						Wxload->setScale(1.1f);
					}
					else if (type == ui::Widget::TouchEventType::MOVED)
					{
						Wxload->setScale(1.0f);
					}
					else if(type == ui::Widget::TouchEventType::ENDED)
					{
						Wxload->setScale(1.0f);
						if (!_wxloadclick)
						{
							_wxloadclick = true;
						}
						Login::getInstance()->wx_Login();
					}
		        });
		    }
		    if (!Login::getInstance()->if_Login()||GameData::getSaveData()->_isWxLoad)
				Wxload->setVisible(false);
			if (!RankScene::getInstance()->getUserData(101) && !RankScene::getInstance()->getUserData(102) && !RankScene::getInstance()->getUserData(102))
				Wxload->setVisible(false);
			Wxload->runAction(RepeatForever::create((CCActionInterval*)Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=]() {
				if (Login::getInstance()->getUserInfo())
					Wxload->setVisible(false);
			}))));

			
		#endif
	}

	//联通包月礼�?
	auto vip = dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_vip"));
    if(vip)
    {
//		vip->setVisible(false);
        vip->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
            if (type == ui::Widget::TouchEventType::BEGAN)
            {
                cjMusic::playEffect("video/tap.mp3");
                vip->setScale(1.1f);
            }
            else if (type == ui::Widget::TouchEventType::MOVED)
                vip->setScale(1.0f);
            else if (type == ui::Widget::TouchEventType::ENDED)
            {
                vip->setScale(1.0f);
                PayScene::getInstance()->vipPage();
            }
        });
        if ((vigame::pay::PayManager::getPayOperator()==vigame::pay::PAY_OPERATOR::PAY_Operator_UniCom)&&vigame::pay::PayManager::getDefaultFeeInfo() && vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(-100))
            vip->setVisible(true);
        else
            vip->setVisible(false);
    }
    
    //设置
	set();
	//

	auto listener=EventListenerKeyboard::create();
	listener->onKeyReleased=CC_CALLBACK_2(MainScene::onKeyReleased,this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

	if (GameDataInstance()->d_rank)
	{
		GameDataInstance()->d_rank = false;
		int key = 101;
		if (GameDataInstance()->d_Project == GAME_START)
			key = 101;
		else if (GameDataInstance()->d_Project == GAME_ANGLE)
			key = 102;
		else if (GameDataInstance()->d_Project == GAME_FILLUP)
			key = 103;

		this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([key,this]() {
			RankScene::getInstance()->rankMainPage(key);
		}), nullptr));
	}
	else
	{
        this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([]() {
            PayScene::getInstance()->starPage();
        }), nullptr));
	}

    this->schedule(schedule_selector(MainScene::adSoundUpdate));
	return true;
}

void MainScene::text(int k)
{
}

void exitGame()
{
	cj::LogText("exitGame");

	vigame::ad::ADManager::openAd("exit_game");
	Director::getInstance()->end();

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS|| IOSMODE)
	exit(0);
#endif
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	int dd = (int)keyCode;
	auto lay = Director::getInstance()->getRunningScene()->getChildByTag(100);
	if (lay)
	{
		lay->removeFromParent();
	}
	else if (dd == 6)
	{
		if (cjTTFLabel::getStringByKey("jinli") == "true")
		{
			auto backlayer = Director::getInstance()->getRunningScene()->getChildByName("jinli");
			if(!backlayer)
				jinliBack();
		}
		else
		{
			if (vigame::pay::PayManager::isExitGame() == 1)
			{
				vigame::pay::PayManager::setOnGameExit(exitGame);
				vigame::pay::PayManager::openExitGame();
			}
			else
			{
				exitGame();
			}
		}
	}
}

//jinli退出
void MainScene::jinliBack()
{
	auto jinlilayer = LayerColor::create(Color4B(0, 0, 0, 120), WINSIZE.width, WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(jinlilayer, 200, "jinli");
	jinlilayer->setPosition(WINORIGIN);

	auto backcsb = CSLoader::createNode("animation/jinliBack.csb");
	jinlilayer->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f, 0.5f));
	backcsb->setPosition(WINSIZE / 2.0f);

	bool isjinli = true;
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	isjinli = OffersHelper::getInstance()->isSupportGamePrivilege();
#endif

	for (int i = 0; i < 3; i++)
	{
		auto button = dynamic_cast<ui::Button*>(backcsb->getChildByName(cjTTFLabel::getNameByInt("Button_%d",i+1)));
		button->addTouchEventListener([=](Ref* re, ui::Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::BEGAN)
				button->setScale(1.02f);
			else if (type == ui::Widget::TouchEventType::MOVED)
				button->setScale(1.0f);
			else if (type == ui::Widget::TouchEventType::ENDED)
			{
				button->setScale(1.0f);
				if (i == 0)
				{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
					OffersHelper::getInstance()->launchGNGameHall();
#endif
				}
				else if (i == 1)
				{
					if (vigame::pay::PayManager::isExitGame() == 1)
					{
						vigame::pay::PayManager::setOnGameExit(exitGame);
						vigame::pay::PayManager::openExitGame();
					}
					else
					{
						exitGame();
					}
				}
				else if (i == 2)
				{
					jinlilayer->removeFromParent();
				}
			}
		});
		if (isjinli)
		{
			if(i==2)
				button->setVisible(false);
		}
		else
		{
			if(i==0)
				button->setVisible(false);
		}
	}

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};
	jinlilayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, jinlilayer);
}

void MainScene::set()
{
	_setstate=1;

	Menu* setbu=cjMenu::createWithImage2("main/set.png",[this](Ref *mu){
		auto setbu=getChildByName("set");
		if (_setstate == 1)
		{
			setbu->setPosition(Vec2(35, 35));
			getChildByName("help")->setVisible(true);
			if (GameData::getSaveData()->_music)
			{
				getChildByName("sound2")->setVisible(true);
			}
			else
			{
				getChildByName("sound1")->setVisible(true);
			}

			if (cjTTFLabel::getStringByKey("EGAME") == "true")
				getChildByName("aboutus")->setVisible(true);

			_setstate=2;
		}
		else if (_setstate == 2)
		{
			setbu->setPosition(Vec2(50, 50));
			getChildByName("help")->setVisible(false);
			if (GameData::getSaveData()->_music)
				getChildByName("sound2")->setVisible(false);
			else
				getChildByName("sound1")->setVisible(false);

			if (cjTTFLabel::getStringByKey("EGAME") == "true")
				getChildByName("aboutus")->setVisible(false);

			_setstate=1;
		}
	},false);
	this->addChild(setbu,2,"set");
	setbu->setAnchorPoint(Vec2(0.0f,0.0f));
	setbu->setPosition(Vec2(50,50));

	auto help=cjMenu::createWithImage2("main/anniu_help.png",[](Ref *rf){
		PayScene::getInstance()->help();
	},false);
	this->addChild(help,1,"help");
	help->setAnchorPoint(Vec2(0.0f,0.0f));
	help->setPosition(Vec2(45,95));
	help->setVisible(false);

	for(int i=0;i<2;i++)
	{
		string str;
		if(i==0)
			str="main/anniu_sound1.png";
		else if(i==1)
			str="main/anniu_sound2.png";
		auto sound=cjMenu::createWithImage2(str,[i,this](Ref *rf){
			if(i==0)
			{
				GameData::getSaveData()->_music=true;
				getChildByName("sound1")->setVisible(false);
				getChildByName("sound2")->setVisible(true);
				cjMusic::playBackgroundMusic("video/music.mp3", true);
			}
			else if(i==1)
			{
				cjMusic::stopBackgroundMusck();
				GameData::getSaveData()->_music=false;
				getChildByName("sound1")->setVisible(true);
				getChildByName("sound2")->setVisible(false);
			}
			GameData::getInstance()->dataSave();
		},false);
		sound->setAnchorPoint(Vec2(0.0f,0.0f));
		this->addChild(sound,1,cjTTFLabel::getNameByInt("sound%d",i+1));

		if (cjTTFLabel::getStringByKey("EGAME") == "true")
		{
			sound->setScale(0.85f);
			sound->setPosition(Vec2(94,30));
		}
		else
			sound->setPosition(Vec2(97,45));


		//if(GameData::getSaveData()->_music&&i==0)
		//	sound->setVisible(false);
		//else if(!GameData::getSaveData()->_music&&i==1)
		sound->setVisible(false);
	}

	if (cjTTFLabel::getStringByKey("EGAME") == "true")
	{
		help->setScale(0.9f);
		help->setPosition(Vec2(32, 92));

		auto aboutus = cjMenu::createWithImage("anniu_about.png", [](Ref *mu) {
			PayScene::getInstance()->aboutus();
		});
		this->addChild(aboutus, 1, "aboutus");
		aboutus->setPosition(Vec2(77, 75));
		aboutus->setVisible(false);
		aboutus->setAnchorPoint(Vec2(0.0f, 0.0f));
		aboutus->setScale(0.9f);
	}
}

void MainScene::xinshouUpdate(float ft)
{
	//
	auto csb=getChildByName("csb");

	auto xslb = dynamic_cast<ui::Button*>(csb->getChildByName("Button_xslb"));
	if (!xslb->isVisible())
	{
		if (GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo() && vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(6))
			xslb->setVisible(true);
	}

	auto lb = dynamic_cast<ui::Button*>(csb->getChildByName("Button_lb"));
	if (!lb->isVisible()&&GameData::getSaveData()->_xinshou&&GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo() && (vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(0) || vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(7)))
	{
		lb->setVisible(true);
	}
	else if(lb->isVisible()&&!GameData::getSaveData()->_xinshou)
	{
		lb->setVisible(false);
	}

	auto viplb= dynamic_cast<ui::Button*>(csb->getChildByName("Button_vip"));
	if (viplb)
	{
		if (!viplb->isVisible())
		{
			if (GameDataInstance()->d_PayType == vigame::pay::PAY_TYPE::PAY_TYPE_UNIWo&&vigame::pay::PayManager::getDefaultFeeInfo() && vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(-100))
				viplb->setVisible(true);
		}
	}

}

bool MainScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	//_wavenum++;
	//_gridnode->runAction(Sequence::create(Ripple3D::create(1.5f, CCSizeMake(32, 24), touch->getLocation(), 300, 4, 30),
	//	CallFunc::create([this]() {
	//	_wavenum--;
	//	if (_wavenum == 0)
	//		_gridnode->runAction(Ripple3D::create(0.1f, CCSizeMake(32, 24), Vec2(0,0), 10, 1, 0));
	//}), nullptr));
	return false;
}

void MainScene::onTouchesMoved(Touch *touch, Event *unused_event)
{}

void MainScene::onTouchEnded(Touch *touch, Event *unused_event)
{}

//弹包月礼包奖�?
void MainScene::openVipGift()
{
	if (GameData::getSaveData()->_isMonthPay)
	{
		if ((GameData::getSaveData()->_monthpay_day != GameDataInstance()->d_monthday) || (GameData::getSaveData()->_monthpay_month != GameDataInstance()->d_monthtime))
		{
			this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([]() {
				auto lay = Director::getInstance()->getRunningScene()->getChildByTag(100);
				if (!lay)
				{
					PayScene::getInstance()->vipRewardPage();
				}
			}), nullptr));
		}
		else
			openDayClick();
	}
	else
	{
		openDayClick();
	}
}

//弹每日签�?
void MainScene::openDayClick()
{
	if (GameDataInstance()->main_num == 1)
	{
		this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([this]() {
			auto lay = Director::getInstance()->getRunningScene()->getChildByTag(100);
			if (!lay)
			{
				//auto Button_rank = dynamic_cast<ui::Button*>(this->getChildByName("csb")->getChildByName("Button_rank"));
				//if (Button_rank->isVisible())
				//{
				//	RankScene::getInstance()->rankMainPage();
				//}
				//else
				//{
					//自动弹出每日奖励
					int k1 = GameData::getSaveData()->_qd_day;
					int k2 = GameDataInstance()->d_daytime;
					if (GameData::getSaveData()->_qd_day != GameDataInstance()->d_daytime)
					{
						PayScene::getInstance()->dailyClick();
					}
				//}
			}
		}), nullptr));
	}
}

void MainScene::adSoundUpdate(float ft)
{
    if(GameData::getSaveData()->_music)
    {
        if(PayScene::getInstance()->canPlayAd()&&!cjMusic::isBackgroundMusic())
            cjMusic::resumeBackgroundMusic();
        else if(!PayScene::getInstance()->canPlayAd()&&cjMusic::isBackgroundMusic())
            cjMusic::pauseBackgroundMusic();
    }
}
