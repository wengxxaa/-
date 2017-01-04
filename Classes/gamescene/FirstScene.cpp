#include "FirstScene.h"
#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "vigame_pay.h"
using namespace cocostudio;

bool FirstScene::init()
{
	if(!Layer::init())
		return false;

	cj::LogText("FirstScene");

	string str="animation/firstpage.csb";
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
	str = "animation/firstpage_ios.csb";
#endif

	auto backcsb=CSLoader::createNode(str);
	this->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f, 0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	float time = 3.0f;
	if (cjTTFLabel::getStringByKey("MMLOGE") == "true")
	{
		auto mmlogo = Sprite::create("logo/logo.png");
		this->addChild(mmlogo, 2);
		mmlogo->setPosition(WINSIZE / 2.0f);

		time = 5.0f;

		runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]() {
			mmlogo->removeFromParent();
		}), nullptr));
	}


	runAction(Sequence::create(DelayTime::create(time),CallFunc::create([=](){
		cj::preLoad();
		GameDataInstance()->d_giftfist = true;
		Director::getInstance()->replaceScene(MainScene::createScene());

		//公告
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
		vigame::notice::open();
#endif
	}),nullptr));

	return true;
}