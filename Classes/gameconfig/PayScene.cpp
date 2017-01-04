#include"PayScene.h"
#include"../cj/cjconfig.h"
#include"../cj/GameData.h"
#include"vigame_pay.h"
//#include"MobClickCpp.h"
#include"vigame_ad.h"
#include "BaseConfig.h"
#include"../cj/Toast.h"
#include "cocostudio/CocoStudio.h"
#include "ui/UIText.h"
#include "vigame_tj.h"
#include "../rank/NetWork.h"
#include "../CommentLayer.h"
#include"../gameconfig//BaseManage.h"
#include"../gamescene/GameScene.h"
#include"../gamescene/MainScene.h"
using namespace cocostudio;

static const int FILLUPPPRICE[3] = { 10,20,30 };

static int tablenum=1;

const string GameUrl="https://itunes.apple.com/us/app/xiao-mie-tang-guo-mian-liu/id882710395?mt=8";

bool PayScene::_adVideo=true;

void PayScene::setPlayAd(bool bo)
{
    _adVideo=bo;
}

bool PayScene::canPlayAd()
{
    return _adVideo;
}

//kuaichong
void PayScene::payPage1()
{
	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	diamondRain(colorlay);

	auto backcsb=CSLoader::createNode("animation/LimitFavorable.csb");
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	auto action=CSLoader::createTimeline("animation/LimitFavorable.csb");
	backcsb->runAction(action);
	action->play("loop",true);

	string num="29";
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
    num="30";
#endif
	
	auto price=dynamic_cast<ui::TextBMFont*>(backcsb->getChildByName("bg")->getChildByName("Label_Price"));
	char name[30];
	sprintf(name,price->getString().c_str(),num.c_str());
	price->setString(name);

	auto cancel=dynamic_cast<ui::Button*>(backcsb->getChildByName("bg")->getChildByName("Button_Cancel"));
	cancel->addTouchEventListener([colorlay](Ref*,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

	auto sure=dynamic_cast<ui::Button*>(backcsb->getChildByName("bg")->getChildByName("sure"));
	sure->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");

			PayScene::getInstance()->paysuccess(1);
		}
	});

	auto hand=handNode::createHand(0.6f);
	backcsb->getChildByName("bg")->addChild(hand,2);
	hand->setPosition(sure->getPosition()+Vec2(140,20));

	backcsb->setScale(0.0f);
	backcsb->runAction(Sequence::create(ScaleTo::create(0.1f,1.1f),ScaleTo::create(0.1f,1.0f),NULL));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//shop
void PayScene::payPage2()
{
	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	diamondRain(colorlay);

	auto animationlay=Layer::create();
	colorlay->addChild(animationlay);

	bool haveshop=false;
	bool have2yuan = false;
	string csbstr="animation/shop2.csb";

	cj::LogText(cjTTFLabel::getNameByInt("%d", vigame::pay::PayManager::getDefaultPayType()));/*GameDataInstance()->d_PayType*/

	//	96897

	auto data = vigame::pay::PayManager::getDefaultFeeInfo();

	if(vigame::pay::PayManager::getDefaultFeeInfo())
	{
		cj::LogText("FeeInfo true");
		if(vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(2))
			cj::LogText("Fee id 2 true");
		else
			cj::LogText("Fee id 2 false");	
	}
	else
		cj::LogText("FeeInfo false");
	
	
	if(GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo()&&vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(2))
	{
		haveshop=true;

		if (vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(9))
		{
			csbstr = "animation/shop_2.csb";
			have2yuan = true;
		}
		else
			csbstr = "animation/shop_1.csb";
	}
	
	auto backcsb=CSLoader::createNode(csbstr);
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	int chiyoukey=0;

	if(haveshop)
	{
		chiyoukey=200;

		//
		int znum[5] = { 10,30,50,100,200 };
		int znum2[5] = { 5,25,50,80,200 };
		int zprice[5] = { 2,6,10,15,29 };

		int fristk = 0;
		if (have2yuan)
			fristk = 0;
		else
			fristk = 1;

		for (int i = fristk; i<5; i++)
		{
			auto anniu = dynamic_cast<ui::Button*>(backcsb->getChildByName(cjTTFLabel::getNameByInt("Node_%d", i + 1))->getChildByName("pay"));
			anniu->addTouchEventListener([i](Ref*, ui::Widget::TouchEventType type) {

				if (type == ui::Widget::TouchEventType::ENDED)
				{
					cjMusic::playEffect("video/tap.mp3");

					if (i == 0)
						PayScene::getInstance()->paysuccess(9);
					else
						PayScene::getInstance()->paysuccess(i + 1);
				}
			});
		}
	}

	auto diamond = dynamic_cast<ui::TextBMFont*>(backcsb->getChildByName("dianum")->getChildByName("diamondnum"));
	diamond->setString(cjTTFLabel::getNameByInt("%d", GameData::getSaveData()->_diamondNumber));
	auto time = DelayTime::create(0.1f);
	auto call = CallFunc::create([diamond]() {
		diamond->setString(cjTTFLabel::getNameByInt("%d", GameData::getSaveData()->_diamondNumber));
	});
	diamond->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(time, call)));

	auto cancel=dynamic_cast<ui::Button*>(backcsb->getChildByName("cha"));
	cancel->addTouchEventListener([colorlay](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

	backcsb->setScale(0.0f);
	backcsb->runAction(ScaleTo::create(0.2f,1.0f));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//xianshi
void PayScene::payPage3()
{
	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	diamondRain(colorlay);

	auto backcsd=CSLoader::createNode("animation/LimitGift.csb");
	colorlay->addChild(backcsd);
	backcsd->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsd->setPosition(WINSIZE/2.0f);

	auto action1=CSLoader::createTimeline("animation/LimitGift.csb");
	backcsd->runAction(action1);
	action1->play("loop",true);

	auto node=dynamic_cast<Node*>(backcsd->getChildByName("GiftBox"));
	auto action2=CSLoader::createTimeline("animation/LimitGiftNode.csb");
	node->runAction(action2);
	action2->play("lihe",false);
	action2->setLastFrameCallFunc([action2](){
		action2->play("lihe_jihe",true);
	});

	int price=29;    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || IOSMODE)
	price=30;
#endif
	
	auto pricefont=dynamic_cast<ui::Text*>(backcsd->getChildByName("Label_Price"));
	char name[30];
	sprintf(name,pricefont->getString().c_str(),price);
	pricefont->setString(name);
	//

	auto cancel=dynamic_cast<ui::Button*>(backcsd->getChildByName("Button_Cancel"));
	cancel->addTouchEventListener([colorlay](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}

	});

	auto sure=dynamic_cast<ui::Button*>(backcsd->getChildByName("Button_Lingqu"));
	sure->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");

			PayScene::getInstance()->paysuccess(6);
		}
	});

	auto hand=handNode::createHand(0.6f);
	colorlay->addChild(hand);
	hand->setPosition(sure->getPosition()+Vec2(120,20));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//xinshou
void PayScene::payPage4()
{
	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay, 100, "xinshou");
	colorlay->setTag(100);
	colorlay->setPosition(WINORIGIN);

	diamondRain(colorlay);

	auto backcsb=CSLoader::createNode("animation/newhand.csb");
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	auto action=CSLoader::createTimeline("animation/newhand.csb");
	backcsb->runAction(action);
	action->play("suremotion",true);

	int k=20;
	string s="0.1";
	if(vigame::pay::PayManager::getDefaultPayType()==2)
	{
		k=10;
		s="0.01";
	}

	char name[30];
	auto pricefont=dynamic_cast<ui::TextBMFont*>(backcsb->getChildByName("pricefont"));
	pricefont->setString(s);
	auto numfont=dynamic_cast<ui::TextBMFont*>(backcsb->getChildByName("numfont"));
	sprintf(name,numfont->getString().c_str(),k);
	numfont->setString(name);

	auto cancel=dynamic_cast<ui::Button*>(backcsb->getChildByName("cha"));
	cancel->addTouchEventListener([colorlay](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

	auto sure=dynamic_cast<ui::Button*>(backcsb->getChildByName("sure"));
	sure->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");

			if(vigame::pay::PayManager::getDefaultPayType()==2)
				PayScene::getInstance()->paysuccess(7);
			else
				PayScene::getInstance()->paysuccess(0);
		}
	});

	auto hand=handNode::createHand(0.6f);
	backcsb->addChild(hand,2);
	hand->setPosition(sure->getPosition()+Vec2(120,20));

	backcsb->setScale(0.0f);
	backcsb->runAction(ScaleTo::create(0.2f,1.0f));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//viplb
void PayScene::vipPage()
{
	auto colorlay = LayerColor::create(Color4B(0, 0, 0, 255 * OPA_BEIJING), WINSIZE.width, WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay, 100, "viplibao");
	colorlay->setTag(100);
	colorlay->setPosition(WINORIGIN);

	diamondRain(colorlay);

	string nodename = "animation/monthPay_0.csb";
	if(GameData::getSaveData()->_isMonthPay)
		nodename= "animation/monthPay_1.csb";

	auto backcsb = CSLoader::createNode(nodename);
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f, 0.5f));
	backcsb->setPosition(WINSIZE / 2.0f);

	if (!GameData::getSaveData()->_isMonthPay)
	{
		auto sure = dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_sure"));
		sure->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::BEGAN)
			{
				cjMusic::playEffect("video/tap.mp3");
				sure->setScale(1.1f);
			}
			else if (type == ui::Widget::TouchEventType::MOVED)
			{
				sure->setScale(1.0f);
			}
			else if (type == ui::Widget::TouchEventType::ENDED)
			{
				sure->setScale(1.0f);
				PayScene::getInstance()->paysuccess(-100);
			}
		});

		auto hand = handNode::createHand(0.6f);
		backcsb->addChild(hand, 2);
		hand->setPosition(sure->getPosition() + Vec2(120, 20));
	}

	auto cancel = dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_cha"));
	cancel->addTouchEventListener([colorlay](Ref *, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

void PayScene::vipRewardPage(bool first)
{
	auto colorlay = LayerColor::create(Color4B(0, 0, 0, 255 * OPA_BEIJING), WINSIZE.width, WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay, 100, 100);
	colorlay->setPosition(WINORIGIN);

	diamondRain(colorlay);

	auto backcsb = CSLoader::createNode("animation/monthPay_2.csb");
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f, 0.5f));
	backcsb->setPosition(WINSIZE / 2.0f);

	string nodename;
	int num=10;
	if (first)
	{
		nodename = "dianode2";
		num = 410;
	}
	else
	{
		nodename= "dianode2";
		if (GameData::getSaveData()->_monthpay_month != GameDataInstance()->d_monthtime)
		{
			num = 400 + GameDataInstance()->d_monthday*10;
		}
		else if(GameDataInstance()->d_monthday>GameData::getSaveData()->_monthpay_day)
		{
			num = (GameDataInstance()->d_monthday- GameData::getSaveData()->_monthpay_day) * 10;
		}
	}

	auto label = dynamic_cast<ui::TextBMFont*>(backcsb->getChildByName("PYSHL_kuang_15")->getChildByName("dianode1")->getChildByName("dia_num"));
	label->setString(cjTTFLabel::getNameByInt("%d", num));

	auto node = backcsb->getChildByName("PYSHL_kuang_15")->getChildByName(nodename);
	node->setVisible(false);

	auto sure = dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_sure"));
	sure->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			sure->setScale(1.1f);
		}
		else if (type == ui::Widget::TouchEventType::MOVED)
		{
			sure->setScale(1.0f);
		}
		else if (type == ui::Widget::TouchEventType::ENDED)
		{
			sure->setScale(1.0f);
			GameData::getSaveData()->_diamondNumber += num;
			GameData::getSaveData()->_monthpay_day = GameDataInstance()->d_monthday;
			GameData::getSaveData()->_monthpay_month = GameDataInstance()->d_monthtime;
			GameDataInstance()->dataSave();

			colorlay->removeFromParent();

			dailyClickTip(num, 2);
		}
	});

	auto hand = handNode::createHand(0.6f);
	backcsb->addChild(hand, 2);
	hand->setPosition(sure->getPosition() + Vec2(100, 20));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//
void PayScene::dailyClick()
{
	bool canClick=true;
	int k1 = GameData::getSaveData()->_qd_day;
	int k2 = GameDataInstance()->d_daytime;
	if(GameData::getSaveData()->_qd_day==GameDataInstance()->d_daytime)
		canClick=false;

	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	diamondRain(colorlay);

	auto backcsb=CSLoader::createNode("animation/dailygift.csb");
	colorlay->addChild(backcsb,1,"csb");
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	auto cancel=dynamic_cast<ui::Button*>(backcsb->getChildByName("cha"));
	cancel->addTouchEventListener([colorlay](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

	for(int i=0; i<7;i++)
	{
		if(i>=GameData::getSaveData()->_qd_number)
		{
			auto right=backcsb->getChildByName(cjTTFLabel::getNameByInt("day_%d",i+1))->getChildByName("right");
			right->setVisible(false);
		}
	}

	auto sure=dynamic_cast<ui::Button*>(backcsb->getChildByName("sure"));
	auto mflq=sure->getChildByName("fnt_mflq");
	auto ylq=sure->getChildByName("fnt_ylq");
	if(canClick)
	{
		ylq->setVisible(false);
		auto hand=handNode::createHand(0.6f);
		backcsb->addChild(hand,1,"hand");
		hand->setPosition(sure->getPosition()+Vec2(120,20));
	}
	else
		mflq->setVisible(false);

	sure->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			if(GameData::getSaveData()->_qd_day!=GameDataInstance()->d_daytime&&canClick)
			{
				GameData::getSaveData()->_qd_day=GameDataInstance()->d_daytime;
				GameData::getSaveData()->_qd_number++;
				assert(GameData::getSaveData()->_qd_number<8);
				//GameData::getSaveData()->_diamondNumber+=DAILY_REWARD[GameData::getSaveData()->_qd_number-1];
				//GameData::getInstance()->dataSave();

				dailyClickTip(DAILY_REWARD[GameData::getSaveData()->_qd_number-1]);

				auto right=backcsb->getChildByName(cjTTFLabel::getNameByInt("day_%d",GameData::getSaveData()->_qd_number))->getChildByName("right");
				right->setVisible(true);

				mflq->setVisible(false);
				ylq->setVisible(true);
				backcsb->removeChildByName("hand");
			}
			else
			{
				colorlay->removeFromParent();
			}
		}
	});
	 
	backcsb->setScale(0.0f);
	backcsb->runAction(ScaleTo::create(0.2f,1.0f));

	auto diamondnum = dynamic_cast<ui::TextBMFont*>(backcsb->getChildByName("diamondnum")->getChildByName("num"));
	diamondnum->setString(cjTTFLabel::getNameByInt("%d", GameData::getSaveData()->_diamondNumber));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);

	return;
}

//
void PayScene::help()
{
	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	auto backcsb=CSLoader::createNode("animation/help.csb");
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	auto cancel=dynamic_cast<ui::Button*>(backcsb->getChildByName("cha"));
	cancel->addTouchEventListener([colorlay](Ref*,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

	backcsb->setScale(0.0f);
	backcsb->runAction(ScaleTo::create(0.2f,1.0f));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

void PayScene::helpFillUp()
{
	auto colorlay = LayerColor::create(Color4B(0, 0, 0, 255 * OPA_BEIJING), WINSIZE.width, WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay, 100, 100);
	colorlay->setPosition(WINORIGIN);

	auto backcsb = CSLoader::createNode("animation/help_fillup.csb");
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f, 0.5f));
	backcsb->setPosition(WINSIZE / 2.0f);

	auto cancel = dynamic_cast<ui::Button*>(backcsb->getChildByName("cha"));
	cancel->addTouchEventListener([colorlay](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");

			auto lay = Director::getInstance()->getRunningScene()->getChildByTag(99);
			BaseManage *manage = dynamic_cast<BaseManage*>(lay->getChildByName("manage"));
			manage->start();

			colorlay->removeFromParent();
		}
	});

	backcsb->setScale(0.0f);
	backcsb->runAction(ScaleTo::create(0.2f, 1.0f));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//
void PayScene::dailyClickTip(int number,int key)
{
	if(key==2)
	{
		auto downsp=Scale9Sprite::createWithSpriteFrameName("tsk.png");
		Director::getInstance()->getRunningScene()->addChild(downsp,100);
		downsp->setPreferredSize(CCSizeMake(250,60));
		downsp->setOpacity(200);
		downsp->setPosition(Vec2(WINSIZE.width/2.0f,400));

		auto tip=LabelTTF::create(cjTTFLabel::getNameByIntL(cjTTFLabel::getStringByKey("hb3").c_str(),number),TTF_PATH,25);
		downsp->addChild(tip);
		tip->setColor(Color3B::WHITE);
		tip->setPosition(downsp->getContentSize()/2.0f);

		downsp->runAction(Sequence::create(MoveBy::create(0.3f,Vec2(0,150)),MoveBy::create(0.2f,Vec2(0,-10)),DelayTime::create(2.0f),
			CallFunc::create([=](){
				downsp->removeFromParent();
		}),nullptr));
	}
	else
	{
		auto colorlay=dynamic_cast<Layer*>(Director::getInstance()->getRunningScene()->getChildByTag(100));

		auto secondlayer=LayerColor::create(Color4B(0,0,0,255*0.7f),WINSIZE.width,WINSIZE.height);
		colorlay->addChild(secondlayer,100,100);
		secondlayer->setPosition(WINORIGIN);

		auto gx=cjSprite::createWithImage("gxn.png");
		secondlayer->addChild(gx);
		gx->setPosition(Vec2(WINSIZE.width*0.5f,WINSIZE.height*0.7f));
		gx->setScale(0.0f);
		gx->runAction(ScaleTo::create(0.2f,1.0f));

		auto guangnode=CSLoader::createNode("animation/guang.csb");
		secondlayer->addChild(guangnode);
		guangnode->setScale(0.8f);
		guangnode->setPosition(Vec2(WINSIZE.width*0.5f,WINSIZE.height*0.5f));

		auto guangaction=CSLoader::createTimeline("animation/guang.csb");
		guangnode->runAction(guangaction);
		guangaction->play("guang",true);

		auto backsp=Sprite::create("animation/guoguan/34.png");
		secondlayer->addChild(backsp);
		backsp->setPosition(guangnode->getPosition());

		string str;
		int num=1;

		if(number<50)
		{
			str="zs.png";
			num=number;
			GameData::getSaveData()->_diamondNumber+=num;

            addDiamondPlist(secondlayer,2);
		}
		else if(number>50)
		{
			str=cjTTFLabel::getNameByInt("prop%02d.png",number-50);
			GameData::getSaveData()->_propnum[number-51]++;

			yanhua(secondlayer);
		}

		GameDataInstance()->dataSave();

		auto sp=cjSprite::createWithImage(str);
		secondlayer->addChild(sp);
		if(key==53||key==54)
			sp->setScale(1.0f);
		else
			sp->setScale(1.2f);
		sp->setPosition(guangnode->getPosition()+Vec2(0,10));

		auto font=cjAtlasLabel::createWithInt(num,ATLAS_PATH3);
		secondlayer->addChild(font);
		font->setScale(1.1f);
		font->setAnchorPoint(Vec2(1.0f,0.5f));
		font->setPosition(guangnode->getPosition()-Vec2(2,50));

		auto ge=cjSprite::createWithImage("zi_ge.png");
		secondlayer->addChild(ge);
		ge->setAnchorPoint(Vec2(0.0f,0.5f));
		ge->setPosition(font->getPosition()+Vec2(5,0));

		auto touchListener = EventListenerTouchOneByOne::create();
		touchListener->setSwallowTouches(true);
		touchListener->onTouchBegan = [secondlayer,this](Touch* touch, Event* event){
			return true;
		};
		touchListener->onTouchEnded = [=](Touch* touch, Event* event){
			secondlayer->removeFromParent();
			float time = 0.5f;
			if (number < 50)
			{
				auto diamondnum = dynamic_cast<ui::TextBMFont*>(colorlay->getChildByName("csb")->getChildByName("diamondnum")->getChildByName("num"));
				turnAddDiamond(colorlay, num, diamondnum, Vec2(28, 772));
				time = 1.5f;
			}
			colorlay->runAction(Sequence::create(DelayTime::create(time), CallFunc::create([=]() {
				Director::getInstance()->getRunningScene()->removeChildByTag(100);
			}), nullptr));
		};
		secondlayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, secondlayer);
	}
}

void PayScene::addDiamondPlist(Layer *lay,int level)
{
    auto diamonds=ParticleSystemQuad::create("lizi/zuanshiyu.plist");
    lay->addChild(diamonds,level);
    diamonds->setPosition(Vec2(WINSIZE.width/2.0f,WINSIZE.height*1.0f));
    diamonds->setAutoRemoveOnFinish(true);
}

//
void PayScene::levelReward()
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS|| IOSMODE)
	auto pADItem = vigame::ad::ADManager::isAdReady("home_mfzs", "video");
	if (pADItem&&GameData::getSaveData()->_freeroundnum < 20) {
		GameDataInstance()->d_jointurn = true;
		turnTable();
	}
	return;
#endif

	if (GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo() && vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(8) == 0)
		return;

	int key = vigame::pay::PayManager::getGiftCtrlFlagUse(4);
	if (key == 4)
	{

	}
	else
	{
		if (key == 2)
			payPage1();
		else if (key == 3)
			payPage3();
		return;
	}

	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	diamondRain(colorlay);

	auto backcsb=CSLoader::createNode("animation/guoguangift.csb");
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	auto action=CSLoader::createTimeline("animation/guoguangift.csb");
	backcsb->runAction(action);
	action->play("loop",true);

	auto bg=backcsb->getChildByName("bg");

	auto price=dynamic_cast<ui::TextBMFont*>(bg->getChildByName("Label_Price"));
	char name[30];
	sprintf(name,price->getString().c_str(),"29");
	price->setString(name);
	price->setVisible(false);

	auto out=bg->getChildByName("Out");
	auto In=bg->getChildByName("In");
	In->setVisible(false);

	_canTouch=true;

	for(int i=0;i<9;i++)
	{
		auto Card=bg->getChildByName(cjTTFLabel::getNameByInt("Card%d",i+1));
		auto cout=Card->getChildByName("Out");
		auto cin=Card->getChildByName("In");
		cin->setVisible(false);
		   
		auto button=dynamic_cast<ui::Button*>(cout->getChildByName("Button_out"));
		button->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
			if(type==ui::Widget::TouchEventType::ENDED&&_canTouch)
			{
				cjMusic::playEffect("video/tap.mp3");
				_canTouch=false;
				int ddd[9]={4,5,11,30,50,60,80,160};

				int num=(rand()%5)+1;
				ddd[2]-=num;
				GameData::getSaveData()->_diamondNumber+=num;
				GameData::getInstance()->dataSave();
				dailyClickTip(num,2);

				auto diamonds = ParticleSystemQuad::create("lizi/diamonds.plist");
				colorlay->addChild(diamonds, 2);
				diamonds->setPosition(Vec2(WINSIZE.width / 2.0f, WINSIZE.height*0.6f));
				diamonds->setAutoRemoveOnFinish(true);

				float ff=0.2f;
				int knum=0;
				for(int j=0;j<9;j++)
				{
					auto jcard=bg->getChildByName(cjTTFLabel::getNameByInt("Card%d",j+1));
					auto jout=jcard->getChildByName("Out");
					auto jin=jcard->getChildByName("In");
					Vector<FiniteTimeAction*> action;
					if(j!=i)
						action.pushBack(DelayTime::create(0.3f+2*ff));
					action.pushBack(ScaleTo::create(ff,0.0f,1.0f));
					action.pushBack(FadeOut::create(0.0f));
					int jnum=num;
					if(j!=i)
					{
						jnum=ddd[knum];
						knum++;
					}
					action.pushBack(CallFunc::create([=](){
						jin->setVisible(true);
						jin->setScaleX(0.0f);
						jin->runAction(ScaleTo::create(ff,1.0f));

						auto numfont=dynamic_cast<ui::TextBMFont*>(jin->getChildByName("Num"));
						numfont->setString(cjTTFLabel::getNameByInt("x%d",jnum));
					}));
					if(j==i)
					{
						action.pushBack(DelayTime::create(0.3f+3*ff));
						action.pushBack(CallFunc::create([=](){
							out->setVisible(false);
							In->setVisible(true);
							price->setVisible(true);
							auto hand=handNode::createHand(0.6f);
							colorlay->addChild(hand,1);
							hand->setPosition(In->getPosition()+Vec2(130,80));
							auto Inbutton=dynamic_cast<ui::Button*>(In->getChildByName("Button_Lingqu"));
							Inbutton->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
								if(type==ui::Widget::TouchEventType::ENDED)
								{
									cjMusic::playEffect("video/tap.mp3");

									PayScene::getInstance()->paysuccess(8);
									colorlay->removeFromParent();
								}
							});
						}));
					}
					jout->runAction(Sequence::create(action));
				}
			}
		});
	}

	auto cancel=dynamic_cast<ui::Button*>(bg->getChildByName("Button_Cancel"));
	cancel->addTouchEventListener([colorlay](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			cjSchedule::getInstance()->runOnNextFrame([=](float ft) {
				colorlay->removeFromParent();
			});
		}
	});

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);

	return;
}

//
void PayScene::aboutus()
{
	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	auto backcsb=CSLoader::createNode("animation/aboutus.csb");
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	auto gamename=dynamic_cast<ui::Text*>(backcsb->getChildByName("name"));
	char name[100];
	sprintf(name,gamename->getString().c_str(),cjTTFLabel::getStringByKey("gamename").c_str());
	gamename->setString(name);

	auto banben=dynamic_cast<ui::Text*>(backcsb->getChildByName("banben"));
	sprintf(name,banben->getString().c_str(), vigame::SysConfig::getInstance()->getAppVersion().c_str());
	banben->setString(name);

	auto cancel=dynamic_cast<ui::Button*>(backcsb->getChildByName("cha"));
	cancel->addTouchEventListener([=](Ref*,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

	backcsb->setScale(0.0f);
	backcsb->runAction(ScaleTo::create(0.2f,1.0f));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//
void PayScene::turnTable()
{
	cj::clickUMCount(1);

	_canTouch=true;
	tablenum=0;
	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	diamondRain(colorlay);

	string csbname="animation/Truntable_android.csb";
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
	csbname="animation/Truntable_ios.csb";
#endif

	auto csb=CSLoader::createNode(csbname);
	colorlay->addChild(csb,1,"csb");
	csb->setPosition(WINORIGIN);

	for(int i=0;i<8;i++)
		_sp[i]=dynamic_cast<Sprite*>(csb->getChildByName("tab_back")->getChildByName(cjTTFLabel::getNameByInt("reward_%d",i+1)));

	auto csbaction=CSLoader::createTimeline(csbname);
	csb->runAction(csbaction);
	csbaction->play("light",true);

	auto cancel=dynamic_cast<ui::Button*>(csb->getChildByName("cha"));
	cancel->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
            
            if(!cjMusic::isBackgroundMusic())
                cjMusic::resumeBackgroundMusic();
			colorlay->removeFromParent();
		}
	});

#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
	auto mfcj = dynamic_cast<Node*>(csb->getChildByName("mfcj"));
	auto lottery = dynamic_cast<ui::Button*>(mfcj->getChildByName("menu_1"));
	auto diamondnum = dynamic_cast<ui::TextBMFont*>(csb->getChildByName("diamondnum")->getChildByName("num"));
	diamondnum->setString(cjTTFLabel::getNameByInt("%d", GameData::getSaveData()->_diamondNumber));
	auto tip = csb->getChildByName("tipShow");

    lottery->setPosition(lottery->getPosition()+Vec2(0,10));
	auto hand = handNode::createHand(0.4f);
	colorlay->addChild(hand, 2, "hand");
	hand->setPosition(Vec2(320,120));

	int kk = GameData::getSaveData()->_freeroundnum;
	auto cishunum = dynamic_cast<ui::Text*>(csb->getChildByName("tab_back")->getChildByName("turnshow")->getChildByName("num"));
	cishunum->setString(cjTTFLabel::getNameByInt("%d", MAX(0,(20 - GameData::getSaveData()->_freeroundnum))));

	auto menuno= dynamic_cast<ui::Button*>(mfcj->getChildByName("menu_2"));	
	//menuno->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
	//	if(type == ui::Widget::TouchEventType::BEGAN)
	//		turnTip();
	//});

	lottery->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
          
			cjMusic::playEffect("video/tap.mp3");
			colorlay->stopAllActions();
			tableCallBack(csb);

			lottery->setScale(0.9f);
		}
		else if (type == ui::Widget::TouchEventType::ENDED)
		{
			lottery->setScale(1.0f);
			lottery->setEnabled(false);
		}
	});
    
    auto pADItem = vigame::ad::ADManager::isAdReady("home_mfzs","video");
    if(pADItem&&GameData::getSaveData()->_freeroundnum < 20){
        menuno->setVisible(false);
		tip->setVisible(false);
    }
    else{
        lottery->setVisible(false);
		hand->setVisible(false);

		if (GameData::getSaveData()->_freeroundnum >= 20)
		{
			auto tip1 = tip->getChildByName("tip1");
			tip1->setVisible(false);
		}
		else
		{
			auto tip2 = tip->getChildByName("tip2");
			tip2->setVisible(false);

			auto time = DelayTime::create(1.0f);
			auto call = CallFunc::create([=]() {
				auto pADItem2 = vigame::ad::ADManager::isAdReady("home_mfzs", "video");
				if (pADItem2)
				{
					tip->setVisible(false);
					menuno->setVisible(false);
					lottery->setVisible(true);
					hand->setVisible(true);
					diamondnum->stopAllActions();
				}
			});
			diamondnum->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(time,call)));
		}
    }
    
#else

	if (GameData::getSaveData()->_freeround)
		csb->getChildByName("label2")->setVisible(false);
	else
		csb->getChildByName("label1")->setVisible(false);

	auto lottery = dynamic_cast<ui::Button*>(csb->getChildByName("tab_back")->getChildByName("lottery"));
	lottery->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			if (GameData::getSaveData()->_freeround)
			{
				csb->getChildByName("label2")->setVisible(true);
				csb->getChildByName("label1")->setVisible(false);
				colorlay->stopAllActions();
				tableCallBack(csb);
			}
			else
			{
				if (GameData::getSaveData()->_diamondNumber<10)
				{
					Toast::makeText("nomoney")->show();
				}
				else
				{
					GameData::getSaveData()->_diamondNumber -= 10;
					GameData::getInstance()->dataSave();
					colorlay->stopAllActions();
					tableCallBack(csb);
				}
			}

			lottery->setScale(0.9f);
		}
		else if (type == ui::Widget::TouchEventType::ENDED)
		{
			lottery->setScale(1.0f);
			lottery->setEnabled(false);
		}
	});
#endif

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);

	_sp[0]->getChildByName("shade")->setVisible(false);
	_rewardid =0;


	return ;
}

void PayScene::menuchange()
{
	_sp[_rewardid]->getChildByName("shade")->setVisible(true);
	_sp[(_rewardid+1)%8]->getChildByName("shade")->setVisible(false);
	_rewardid=(_rewardid+1)%8;
	cjMusic::playEffect("video/Wheel_move.mp3");
}

void PayScene::tableCallBack(Node *csb)
{	
	srand(time(NULL));
	int prop=0;
	int key = rand() % 100;
	int tempkey = 0;
	for (int i = 0; i < 8; i++)
	{
		tempkey += GameDataInstance()->d_turndata.at(i+1)[4];
		if (key < tempkey)
		{
			prop = i;
			break;
		}
	}
    
    if (GameDataInstance()->d_jointurn)
    {
        if(prop==2||prop==4||prop==6)
            prop-=1;
    }

	int addprop = prop- _rewardid;

	Vector<FiniteTimeAction*> action;
	for(int i=0;i<6;i++)
	{
		action.pushBack(DelayTime::create(0.3f-(i*0.05f)));
		action.pushBack(CallFunc::create([this](){
			menuchange();
		}));
	}
	for(int i=0;i<(44+addprop);i++)
	{
		action.pushBack(DelayTime::create(0.05f));
		action.pushBack(CallFunc::create([this](){
			menuchange();
		}));
	}
	for(int i=0;i<6;i++)
	{
		action.pushBack(DelayTime::create(0.05f+(i*0.05f)));
		action.pushBack(CallFunc::create([this](){
			menuchange();
		}));
	}
	action.pushBack(DelayTime::create(0.5f));
	action.pushBack(CallFunc::create([=](){
		cjMusic::playEffect("video/Wheel_stop.mp3");
		tableReward(_rewardid);

		//_sp[_rewardid]->getChildByName("shade")->setVisible(true);
	}));
	csb->runAction(Sequence::create(action));
}

void PayScene::tableReward(int key)
{
	cjMusic::playEffect("video/Wheel_get.mp3");

	GameData::getSaveData()->_freeround = false;
	GameData::getSaveData()->_freeroundday = GameData::getInstance()->d_daytime;
	GameData::getInstance()->dataSave();

	auto colorlay=dynamic_cast<Layer*>(Director::getInstance()->getRunningScene()->getChildByName("colorlay"));
	auto csb = colorlay->getChildByName("csb");
	auto hand = colorlay->getChildByName("hand");
	auto menuno = csb->getChildByName("mfcj")->getChildByName("menu_2");
	auto lottery = csb->getChildByName("mfcj")->getChildByName("menu_1");
	auto tip = csb->getChildByName("tipShow");
	auto tip1 = tip->getChildByName("tip1");
	auto tip2 = tip->getChildByName("tip2");
    GameData::getSaveData()->_freeroundnum++;
	if (GameData::getSaveData()->_freeroundnum >= 20)
	{
		menuno->setVisible(true);
		lottery->setVisible(false);
		hand->setVisible(false);
		tip->setVisible(true);
		tip1->setVisible(false);
		tip2->setVisible(true);
	}
	GameData::getInstance()->dataSave();
	auto cishunum = dynamic_cast<ui::Text*>(csb->getChildByName("tab_back")->getChildByName("turnshow")->getChildByName("num"));
	cishunum->setString(cjTTFLabel::getNameByInt("%d", MAX(0,(20 - GameData::getSaveData()->_freeroundnum))));

	auto secondlayer=LayerColor::create(Color4B(0,0,0,255*0.7f),WINSIZE.width,WINSIZE.height);
	colorlay->addChild(secondlayer,100,100);
	secondlayer->setPosition(WINORIGIN);

	auto gx=cjSprite::createWithImage("gxn.png");
	secondlayer->addChild(gx);
	gx->setPosition(Vec2(WINSIZE.width*0.5f,WINSIZE.height*0.7f));
	gx->setScale(0.0f);
	gx->runAction(ScaleTo::create(0.2f,1.0f));

	auto guangnode=CSLoader::createNode("animation/guang.csb");
	secondlayer->addChild(guangnode);
	guangnode->setScale(0.8f);
	guangnode->setPosition(Vec2(WINSIZE.width*0.5f,WINSIZE.height*0.5f));

	auto guangaction=CSLoader::createTimeline("animation/guang.csb");
	guangnode->runAction(guangaction);
	guangaction->play("guang",true);

	auto backsp = Sprite::create("animation/guoguan/34.png");
	secondlayer->addChild(backsp);
	backsp->setPosition(guangnode->getPosition());

	string str;
	int type = GameDataInstance()->d_turndata.at(key + 1)[1];
	int num;
	if (GameDataInstance()->d_turndata.at(key + 1)[2] == GameDataInstance()->d_turndata.at(key + 1)[3])
		num = GameDataInstance()->d_turndata.at(key + 1)[2];
	else
		num = GameDataInstance()->d_turndata.at(key + 1)[2]+rand()%(GameDataInstance()->d_turndata.at(key + 1)[3]- GameDataInstance()->d_turndata.at(key + 1)[2]+1);

#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
	if (type==1)
	{
		str = "zs.png";
		GameData::getSaveData()->_diamondNumber += num;
		diamondRain(secondlayer);
	}
	else if (type==2)
	{
		// ”∆µπ„∏Ê

	//	auto cha = dynamic_cast<ui::Button*>(csb->getChildByName("cha"));
	//	cha->setEnabled(false);
        
        openTip();
        GameDataInstance()->d_iosad=false;

        
        
        vigame::ad::ADManager::openAd("home_mfzs", [=](vigame::ad::ADSourceItem* pADItem, int result) {
            if (result == vigame::ad::ADSourceItem::open_result::OpenSuccess)
            {
                GameDataInstance()->d_iosad=true;
                
                PayScene::getInstance()->setPlayAd(true);
                closeTip();

				dailyClickTip(num, 2);
                GameData::getSaveData()->_diamondNumber += num;
                GameData::getInstance()->dataSave();
                
                auto clay=dynamic_cast<Layer*>(Director::getInstance()->getRunningScene()->getChildByName("colorlay"));
                if(clay)
                {
                    diamondRain(clay);
                    
                    if (!GameDataInstance()->d_jointurn)
                    {
                        auto lottery = dynamic_cast<ui::Button*>(csb->getChildByName("mfcj")->getChildByName("menu_1"));
                        lottery->setEnabled(true);
     //                   cha->setEnabled(true);
                    }
                    auto diamondnum = dynamic_cast<ui::TextBMFont*>(colorlay->getChildByName("csb")->getChildByName("diamondnum")->getChildByName("num"));
                    turnAddDiamond(colorlay,num,diamondnum,Vec2(28,772));
                    
                    if (GameDataInstance()->d_jointurn)
                    {
                        colorlay->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]() {
                            clay->removeFromParent();
                        }), nullptr));
                    }
                    
                    if (!GameDataInstance()->d_jointurn)
                    {
                        auto pADItem = vigame::ad::ADManager::isAdReady("home_mfzs", "video");
                        if (!pADItem&&GameData::getSaveData()->_freeroundnum < 20)
                        {
                            menuno->setVisible(true);
                            lottery->setVisible(false);
                            hand->setVisible(false);
                            tip->setVisible(true);
                            tip1->setVisible(true);
                            tip2->setVisible(false);
                            auto time = DelayTime::create(1.0f);
                            auto call = CallFunc::create([=]() {
                                auto pADItem2 = vigame::ad::ADManager::isAdReady("home_mfzs", "video");
                                if (pADItem2&&GameDataInstance()->d_iosad)
                                {
                                    tip->setVisible(false);
                                    menuno->setVisible(false);
                                    lottery->setVisible(true);
                                    hand->setVisible(true);
                                    diamondnum->stopAllActions();
                                }
                            });
                            diamondnum->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(time, call)));
                        }
                    }
                }
                
            }
        });
        
		secondlayer->removeFromParent();	
		return;
	}
	else if (type>50)
	{
		str = cjTTFLabel::getNameByInt("prop%02d.png", type - 50);
		GameData::getSaveData()->_propnum[type-51]++;
		yanhua(secondlayer);
	}

#else
	if(key==0||key==4)
	{
		str="zs.png";
		num=rand()%10+1;
		GameData::getSaveData()->_diamondNumber+=num;
		diamondRain(secondlayer);
	}
	else if(key==2||key==6)
	{
		str="zs.png";
		num=rand()%10+10;
		GameData::getSaveData()->_diamondNumber+=num;
		diamondRain(secondlayer);
	}
	else if(key==1)
	{
		str="prop01.png";
		GameData::getSaveData()->_propnum[0]++;
		yanhua(secondlayer);
	}
	else if(key==3)
	{
		str="prop03.png";
		GameData::getSaveData()->_propnum[2]++;
		yanhua(secondlayer);
	}
	else if(key==5)
	{
		str="prop04.png";
		GameData::getSaveData()->_propnum[3]++;
		yanhua(secondlayer);
	}
	else if(key==7)
	{
		str="prop02.png";
		GameData::getSaveData()->_propnum[1]++;
		yanhua(secondlayer);
	}
#endif
	GameDataInstance()->dataSave();

	auto sp=cjSprite::createWithImage(str);
	secondlayer->addChild(sp);
	if(key==5||key==7)
		sp->setScale(1.0f);
	else
		sp->setScale(1.2f);
	sp->setPosition(guangnode->getPosition()+Vec2(0,10));

	auto font=cjAtlasLabel::createWithInt(num,ATLAS_PATH3);
	secondlayer->addChild(font);
	font->setScale(1.1f);
	font->setAnchorPoint(Vec2(1.0f,0.5f));
	font->setPosition(guangnode->getPosition()-Vec2(2,50));

	auto ge=cjSprite::createWithImage("zi_ge.png");
	secondlayer->addChild(ge);
	ge->setAnchorPoint(Vec2(0.0f,0.5f));
	ge->setPosition(font->getPosition()+Vec2(5,0));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [=](Touch* touch, Event* event){
		return true;
	};
	touchListener->onTouchEnded = [=](Touch* touch, Event* event){
		secondlayer->removeFromParent();
		if (IOSMODE && (str == "zs.png"))
		{
			auto diamondnum = dynamic_cast<ui::TextBMFont*>(colorlay->getChildByName("csb")->getChildByName("diamondnum")->getChildByName("num"));
            turnAddDiamond(colorlay,num,diamondnum,Vec2(28,772));
			if (GameDataInstance()->d_jointurn)
			{
				colorlay->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]() {
					colorlay->removeFromParent();
				}), nullptr));
			}
		}
		else
		{
			if (GameDataInstance()->d_jointurn)
				colorlay->removeFromParent();
		}
		if (!GameDataInstance()->d_jointurn)
		{
			auto lottery = dynamic_cast<ui::Button*>(csb->getChildByName("mfcj")->getChildByName("menu_1"));
			lottery->setEnabled(true);
		}
	};
	secondlayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, secondlayer);
}

//
void PayScene::turnAddDiamond(Layer *colorlay,int num,ui::TextBMFont *diamondnum,Vec2 position)
{
    int addnum=MAX(num/5,1);
    int round=num;
    if(round>5)
        round=5;

	auto downsp = Sprite::create("animation/Shop/diamond4.png");
	colorlay->addChild(downsp,21);
	downsp->setPosition(Vec2(WINSIZE / 2.0f) + Vec2(0, 10));

    for (int i = 0; i < round; i++)
    {
        Vector<FiniteTimeAction*> taction;
        auto diamond = cjSprite::createWithImage("zs.png");
        colorlay->addChild(diamond, 20);
        diamond->setScale(1.2f);
        diamond->setRotation(30);
        diamond->setPosition(Vec2(WINSIZE / 2.0f) + Vec2(0, 10));
        taction.pushBack(DelayTime::create(i*0.1f));
        taction.pushBack(CallFunc::create([=]() {
            diamond->runAction(Sequence::create(MoveTo::create(0.7f, position), ScaleTo::create(0.3f, 1.0f), CallFunc::create([=]() {
                parShow(colorlay, 10, position);
                diamond->removeFromParent();

                cjMusic::playEffect("video/coinsin.mp3");
                
				if (i == (round - 1))
				{
					diamondnum->setString(cjTTFLabel::getNameByInt("%d", GameData::getSaveData()->_diamondNumber));
					downsp->removeFromParent();
				}
                else
                    diamondnum->setString(cjTTFLabel::getNameByInt("%d", GameData::getSaveData()->_diamondNumber-num+addnum*(i+1)));
            }), nullptr));
        }));
        diamond->runAction(Sequence::create(taction));
    }
}

void PayScene::turnAddDiamond(Layer *colorlay, int num, Vec2 position)
{
	int addnum = MAX(num / 5, 1);
	int round = num;
	if (round>5)
		round = 5;

	auto downsp = Sprite::create("animation/Shop/diamond4.png");
	colorlay->addChild(downsp, 21);
	downsp->setPosition(Vec2(WINSIZE / 2.0f) + Vec2(0, 10));

	for (int i = 0; i < round; i++)
	{
		Vector<FiniteTimeAction*> taction;
		auto diamond = cjSprite::createWithImage("zs.png");
		colorlay->addChild(diamond, 20);
		diamond->setScale(1.2f);
		diamond->setRotation(30);
		diamond->setPosition(Vec2(WINSIZE / 2.0f) + Vec2(0, 10));
		taction.pushBack(DelayTime::create(i*0.1f));
		taction.pushBack(CallFunc::create([=]() {
			diamond->runAction(Sequence::create(MoveTo::create(0.7f, position), ScaleTo::create(0.3f, 1.0f), CallFunc::create([=]() {
				parShow(colorlay, 10, position);
				diamond->removeFromParent();
                
                cjMusic::playEffect("video/coinsin.mp3");

				if (i == (round - 1))
				{
					GameData::getSaveData()->_diamondNumber += num-addnum*(round-1);
					downsp->removeFromParent();
				}
				else
					GameData::getSaveData()->_diamondNumber += addnum;
			}), nullptr));
		}));
		diamond->runAction(Sequence::create(taction));
	}
	GameData::getInstance()->dataSave();
}

void PayScene::turnTip()
{
	cj::clickUMCount(2);

	auto secondlayer = LayerColor::create(Color4B(0, 0, 0, 255 * 0.7f), WINSIZE.width, WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(secondlayer, 100, "secondlayer");
    secondlayer->setTag(100);
	secondlayer->setPosition(WINORIGIN);

	//secondlayer->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]() {
	//	secondlayer->removeFromParent();
	//}), nullptr));

	auto tipnode = CSLoader::createNode("animation/diamondtip_get.csb");
	secondlayer->addChild(tipnode);
	tipnode->setPosition(WINORIGIN);

	auto tipaction = CSLoader::createTimeline("animation/diamondtip_get.csb");
	tipnode->runAction(tipaction);
	tipaction->play("guangquan", true);

	auto Button_mfzs = dynamic_cast<ui::Button*>(tipnode->getChildByName("Button_mffh"));
	Button_mfzs->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
            cj::clickUMCount(2);
            
			cjMusic::playEffect("video/tap.mp3");
       //     cjMusic::pauseBackgroundMusic();
            
       //     auto cha=dynamic_cast<ui::Button*>(tipnode->getChildByName("cha"));
       //     cha->setEnabled(false);
            Button_mfzs->setEnabled(false);
            
            openTip();
            GameDataInstance()->d_iosad=false;
            
			vigame::ad::ADManager::openAd("home_mfzs", [=](vigame::ad::ADSourceItem* pADItem, int result) {
				if (result == vigame::ad::ADSourceItem::open_result::OpenSuccess)
				{
                    GameDataInstance()->d_iosad=true;
                    
                    PayScene::getInstance()->setPlayAd(true);
                    
					GameData::getSaveData()->_diamondNumber += 10;
					GameData::getSaveData()->_freediamondnum++;
					GameData::getInstance()->dataSave();
                   
                    closeTip();
                    dailyClickTip(10, 2);
                    
                    auto selay=Director::getInstance()->getRunningScene()->getChildByName("secondlayer");
                    if(selay)
                    {
                        selay->removeFromParent();

                        auto lay = Layer::create();
                        lay->setPosition(WINORIGIN);
                        Director::getInstance()->getRunningScene()->addChild(lay, 100);
                        addDiamondPlist(lay, 10);
                    }

				}
			});
		}
	});

	auto fntnum= dynamic_cast<ui::TextBMFont*>(tipnode->getChildByName("fntnum"));
	fntnum->setString(cjTTFLabel::getNameByInt("%d",MAX(1,10- GameData::getSaveData()->_freediamondnum)));

	auto hand = handNode::createHand(0.4f);
	secondlayer->addChild(hand);
	hand->setPosition(Vec2(340,250));

	auto cancel = dynamic_cast<ui::Button*>(tipnode->getChildByName("cha"));
	cancel->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
            closeTip();
            
			cjMusic::playEffect("video/tap.mp3");
			secondlayer->removeFromParent();
		}
	});

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
		return false;
	};
	touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
	};
	secondlayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, secondlayer);
}

//
void PayScene::paysuccess(int key)
{
	int id = key;
	if (key == 0 || key == 7)
		id = 0;
	else if (key >= 2 && key <= 5)
		id = key - 1;
	else if(key==1)
		id = 4;

	GameDataInstance()->d_xianshidalibao = false;
	if(key==6)
		GameDataInstance()->d_xianshidalibao = true;

	vigame::pay::PayManager::orderPay(id);

	cj::payUMCount(id, 1);



	return ;

	GameData::getSaveData()->_diamondNumber+=PAY_DIAMOND[key];

	char idname[10];
	sprintf(idname,"%d",key);

	if(key==6)
	{
		for(int i=0;i<4;i++)
			GameData::getSaveData()->_propnum[i]+=1;
	}
	else if((key==0)||(key==7))
	{
		GameData::getSaveData()->_xinshou=false;
	}

	GameData::getInstance()->dataSave();
	Toast::makeText("PAY_RESULT_SUCCESS")->show();
}

//
void PayScene::diamondRain(Layer *lay)
{
	auto rain = ParticleSystemQuad::create("lizi/zuanshiyu.plist");
	lay->addChild(rain, 100);
	rain->setPosition(Vec2(WINSIZE.width/2.0f,WINSIZE.height*1.0f));
	rain->setAutoRemoveOnFinish(true);
}

//
void PayScene::yanhua(Layer *lay)
{
	auto rain = ParticleSystemQuad::create("lizi/yanhua.plist");
	lay->addChild(rain, 100);
	rain->setPosition(Vec2(WINSIZE.width / 2.0f, WINSIZE.height/2.0f+20));
	rain->setAutoRemoveOnFinish(true);
}

//
void PayScene::parShow(Layer *lay, int level, Vec2 pos)
{
	auto csb = CSLoader::createNode("animation/prop_skill_paint.csb");
	lay->addChild(csb, level);
	csb->setScale(0.5f);
	csb->setPosition(pos);
	auto csbaction = CSLoader::createTimeline("animation/prop_skill_paint.csb");
	csb->runAction(csbaction);
	csbaction->play("change", false);
	csbaction->setLastFrameCallFunc([csb]() {
		csb->removeFromParent();
	});

	auto skill2_bai = csb->getChildByName("skill2_bai");
	skill2_bai->setVisible(false);
}

//
void PayScene::freeDiamond()
{
	auto colorlay = LayerColor::create(Color4B(0, 0, 0, 255 * OPA_BEIJING), WINSIZE.width, WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay, 100, 100);
	colorlay->setPosition(WINORIGIN);

	auto backcsb = CSLoader::createNode("animation/diamondtip.csb");
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f, 0.5f));
	backcsb->setPosition(WINSIZE / 2.0f);

	auto mfzs= dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_mfzs"));
	mfzs->addTouchEventListener([colorlay](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();

			PayScene::getInstance()->turnTip();
		}
	});

	auto pADItem = vigame::ad::ADManager::isAdReady("home_mfzs", "video");
	if (!pADItem||GameData::getSaveData()->_freediamondnum >= 10||!GameDataInstance()->d_iosad) {
		mfzs->setVisible(false);
	}
	else
	{
		auto hand = handNode::createHand(0.6f);
		backcsb->addChild(hand);
		hand->setPosition(mfzs->getPosition()+Vec2(100,0));
	}

	auto cancel = dynamic_cast<ui::Button*>(backcsb->getChildByName("cha"));
	cancel->addTouchEventListener([colorlay](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

	backcsb->setScale(0.0f);
	backcsb->runAction(ScaleTo::create(0.2f, 1.0f));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

void PayScene::openTip()
{
    auto downsp=Scale9Sprite::createWithSpriteFrameName("tsk.png");
    Director::getInstance()->getRunningScene()->addChild(downsp,100,"tipgg");
    downsp->setPreferredSize(CCSizeMake(300,60));
    downsp->setOpacity(200);
    downsp->setPosition(Vec2(WINSIZE.width/2.0f,400));
    
    auto tip=LabelTTF::create(cjTTFLabel::getStringByKey("shipingload").c_str(),TTF_PATH,25);
    downsp->addChild(tip);
    tip->setColor(Color3B::WHITE);
    tip->setPosition(downsp->getContentSize()/2.0f);

}

void PayScene::closeTip()
{
    Director::getInstance()->getRunningScene()->removeChildByName("tipgg");
}

//ŒÂ–«∫√∆¿ΩÁ√�?
void PayScene::starPage()
{
	if (GameData::getSaveData()->_starPraise || !NetWork::getInstance()->isConnectNet() || GameDataInstance()->d_gameEnter < GameDataInstance()->d_starNum)
		return;

	GameDataInstance()->d_gameEnter = 0;
    GameDataInstance()->d_starNum = 5;
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    auto lay=dynamic_cast<Layer*>(Director::getInstance()->getRunningScene()->getChildByTag(1));
    CommentLayer::showCommentLayer(lay, GameUrl);
#endif;
    
    return;

	auto colorlay = LayerColor::create(Color4B(0, 0, 0, 255 * OPA_BEIJING), WINSIZE.width, WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay, 100, 100);
	colorlay->setPosition(WINORIGIN);

	auto backcsb = CSLoader::createNode("animation/starPage.csb");
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f, 0.5f));
	backcsb->setPosition(WINSIZE / 2.0f);

	for (int i = 0; i < 3; i++)
	{
		auto button = dynamic_cast<ui::Button*>(backcsb->getChildByName(cjTTFLabel::getNameByInt("Button_%d",i+1)));
		button->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
			if (i == 0 || i == 1)
			{

			}
			else if (i == 2)
			{
				GameDataInstance()->d_starNum = 5;
			}
			colorlay->removeFromParent();
		});
	}

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//数字增加动画
void PayScene::fontAddMotion(LabelAtlas* font,int num,int round)
{
    int roundnum=round;
    int addnum=1;
    if(num<round)
        roundnum=num;
    else
        addnum=num/round;
    
    Vector<FiniteTimeAction*> action;
    for(int i=0;i<roundnum;i++)
    {
        action.pushBack(CallFunc::create([=](){
            if(i==(roundnum-1))
            {
                font->setString(cjTTFLabel::getNameByInt("%d", num));
            }
            else
            {
                font->setString(cjTTFLabel::getNameByInt("%d", i*addnum+addnum));
            }
        }));
        action.pushBack(DelayTime::create(0.05f));
    }
	font->runAction(Sequence::create(action));
}

//fillupprop
void PayScene::fillupProp()
{
	auto colorlay = LayerColor::create(Color4B(0, 0, 0, 255 * 0.95), WINSIZE.width, WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay, 100, "FILLUPPROP");
	colorlay->setTag(100);
	colorlay->setPosition(WINORIGIN);

	auto backcsb = CSLoader::createNode("animation/FillUpProp.csb");
	colorlay->addChild(backcsb);
	backcsb->setPosition(WINORIGIN);

	//zuanshi
	auto zs = backcsb->getChildByName("diamond");
	auto zuanshi = cjAtlasLabel::createWithInt(GameData::getSaveData()->_diamondNumber, ATLAS_PATH20);
	backcsb->addChild(zuanshi);
	zuanshi->setPosition(zs->getPosition()-Vec2(0,2));
	zuanshi->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(DelayTime::create(0.1f), CallFunc::create([zuanshi]() {
		zuanshi->setString(cjTTFLabel::getNameByInt("%d", GameData::getSaveData()->_diamondNumber));
	}))));
	//

	//����
	for (int i = 1; i <= 3; i++)
	{
		auto prop = backcsb->getChildByName(cjTTFLabel::getNameByInt("prop%d", i));
		auto buy = dynamic_cast<ui::Button*>(prop->getChildByName("buy"));
		buy->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::BEGAN)
			{
				if (!buy->getChildByName("huise")->isVisible())
				{
					if (GameData::getSaveData()->_diamondNumber >= FILLUPPPRICE[i - 1])
					{
						buy->getChildByName("huise")->setVisible(true);

						//
						GameData::getSaveData()->_diamondNumber -= FILLUPPPRICE[i - 1];
						GameData::getSaveData()->_fillupprop[i - 1] = true;
						GameData::getInstance()->dataSave();
					}
					else
					{
						if (GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo() && vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(4))
							PayScene::getInstance()->payPage1();
						else
							Toast::makeText("PAY_RESULT_FAIL")->show();
					}
				}
				else
				{
					GameData::getSaveData()->_diamondNumber += FILLUPPPRICE[i - 1];
					GameData::getSaveData()->_fillupprop[i - 1] = false;
					GameData::getInstance()->dataSave();
					buy->getChildByName("huise")->setVisible(false);
				}
			}
		});

		buy->getChildByName("huise")->setVisible(false);
		if (GameData::getSaveData()->_fillupprop[i - 1])
		{
			buy->getChildByName("huise")->setVisible(true);
		}

		auto zusi = prop->getChildByName("zuanshi");
		auto price = cjAtlasLabel::createWithInt(FILLUPPPRICE[i - 1], ATLAS_PATH20);
		prop->addChild(price);
		price->setAnchorPoint(Vec2(0.0f, 0.5f));
		price->setPosition(zusi->getPosition() + Vec2(zusi->getContentSize().width / 2.0f, 0));
	}

	RankScene::getInstance()->rankSort(103);
	//ͷ��
	auto user = RankScene::getInstance()->getUserData(103);
	for (int i = 0; i < 3; i++)
	{
		auto rank = backcsb->getChildByName(cjTTFLabel::getNameByInt("rank%d", i + 1));
		auto userinfo = RankScene::getInstance()->getUserData(i+1, 103);
		if (userinfo)
		{
			int picorder = 1;
			if (userinfo->order == user->order)
				picorder = 2;
			auto tex = RankScene::getInstance()->getIcon(picorder);
			if (userinfo->image)
				tex = userinfo->image;
			
			auto touxiang = RankScene::getInstance()->getTouXiang(tex);
			backcsb->addChild(touxiang, 1, "pic");
			touxiang->setPosition(rank->getPosition() + Vec2(55, -3));
		}
		else
		{
			rank->setVisible(false);
		}
	}
	if (user)
	{
		auto banner = RankScene::getInstance()->rankBanner(*user, 2, 103);
		backcsb->addChild(banner);
		banner->setPosition(Vec2(240,185));
		banner->removeChildByName("liqu");

		//ȥ���а�Ŀ�
		auto anniu = cjMenu::createWithImage2("animation/Rank/paihangbangzi.png", [=](Ref *rf) {
			auto ranklay= Director::getInstance()->getRunningScene()->getChildByName("ranklay");
			if (ranklay)
			{
				colorlay->removeFromParent();
			}
			else
			{
				RankScene::getInstance()->rankMainPage(103);
			}
		});
		colorlay->addChild(anniu, 10);
		anniu->setPosition(banner->getPosition());
		anniu->setOpacity(0);
		//
	}
	else
	{
		auto tip = LabelTTF::create(cjTTFLabel::getStringByKey("rankdataload").c_str(), TTF_PATH, 25);
		colorlay->addChild(tip, 1, "tip");
		tip->setPosition(Vec2(240, 200));
	}

	//�Ӻ�
	auto addmondy = dynamic_cast<ui::Button*>(backcsb->getChildByName("diamond")->getChildByName("add"));
	addmondy->addTouchEventListener([](Ref *, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			PayScene::getInstance()->payPage2();
		}
	});

	auto start = dynamic_cast<ui::Button*>(backcsb->getChildByName("start"));
	start->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			start->setScale(1.1f);
		}
		else if (type == ui::Widget::TouchEventType::MOVED)
			start->setScale(1.0f);
		else if (type == ui::Widget::TouchEventType::ENDED)
		{
			colorlay->removeFromParent();

			cj::clickUMCount(29);

			if (GameDataInstance()->d_restart)
			{
				Director::getInstance()->popToRootScene();

				Director::getInstance()->replaceScene(GameScene::createScene());
			}
			else
				Director::getInstance()->replaceScene(GameScene::createScene());
		}
	});

	start->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(ScaleTo::create(0.5f, 1.02f), ScaleTo::create(0.5f, 0.98f))));

	if (GameData::getInstance()->d_restart)
		start->setPosition(start->getPosition() + Vec2(0, 15));


	auto cancel = dynamic_cast<ui::Button*>(backcsb->getChildByName("cha"));
	cancel->addTouchEventListener([colorlay](Ref *, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cj::clickUMCount(28);

			cjMusic::playEffect("video/tap.mp3");
			auto ranklay = Director::getInstance()->getRunningScene()->getChildByName("ranklay");

			if (GameData::getInstance()->d_restart&&!ranklay)
				Director::getInstance()->replaceScene(MainScene::createScene());
			else
				colorlay->removeFromParent();
		}
	});

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);

}
