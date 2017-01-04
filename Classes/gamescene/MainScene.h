#ifndef MAINSCENE_H_
#define MAINSCENE_H_

#include"GameScene.h"
#include "../cj/cjconfig.h"
#include"../cj/GameData.h"
#include "vigame_ad.h"

class MainScene:public Layer
{
public:
	
	virtual bool init();

	virtual void onEnter();

	CREATE_FUNC(MainScene);
	CREATE_SCENE(MainScene);

	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchesMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	void text(int k);

private:
	void set();

	void xinshouUpdate(float ft);

	//弹包月礼包奖励
	void openVipGift();
	//弹每日签到
	void openDayClick();
    void adSoundUpdate(float ft);

	//排行榜数据刷新
	//void 

	//jinli退出
	void jinliBack();

	int _setstate;

	bool _xieyi;
    
    Menu* zuanshi;

	NodeGrid *_gridnode;

	int _wavenum;

	bool _wxloadclick;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS|| IOSMODE)
    void checkVideoState(float dt);
#endif
};

#endif