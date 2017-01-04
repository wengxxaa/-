#ifndef PayScene_H_
#define PayScene_H_

#include"stdio.h"
#include"cocos2d.h"
USING_NS_CC;
#include"cocos-ext.h"
USING_NS_CC_EXT;

#include "vigame_ad.h"
#include "ui/UITextBMFont.h"

const int ADTIME=1;

class PayScene
{
public:
	static PayScene* getInstance()
	{
		static PayScene one;
		return &one;
	}

public:

	//xianshi 
	void payPage1();

	//shop
	void payPage2();

	//œﬁ ±¥Û¿Ò∞¸
	void payPage3();

	//–¬ ÷¿Ò∞¸
	void payPage4();

	//√ø»’«©µΩ
	void dailyClick();

	//∞Ô÷˙
	void help();

	void helpFillUp();

	//πÿ”⁄Œ“√«
	void aboutus();

	//√ø»’«©µΩÃ· æ
	void dailyClickTip(int number,int key=1);

	//π˝πÿΩ±¿¯
	void levelReward();

	//√‚∑—◊Í ØÃ· æ
	void freeDiamond();

	//¥Û◊™≈Ã
	void turnTable();

	void tableCallBack(Node* csb);

	void tableReward(int key);


	void menuchange();


	void turnTip();
    
	//◊Í ØœÚ…œ∑…
	void turnAddDiamond(Layer *colorlay, int num, ui::TextBMFont *diamondnum, Vec2 position);
	void turnAddDiamond(Layer *colorlay, int num,Vec2 position);

	//√‚∑—÷ß∏∂
	void paysuccess(int key);

	//◊Í Ø”Í
	void diamondRain(Layer *lay);

	//—Ãª®
	void yanhua(Layer *lay);

	//µ√µΩŒÔ∆∑µƒ–ßπ˚
	void parShow(Layer *lay, int level, Vec2 pos);
    
    void addDiamondPlist(Layer *lay,int level);
    
    void openTip();
    
    void closeTip();

	//ŒÂ–«∫√∆¿ΩÁ√Ê
	void starPage();
    
    void fontAddMotion(LabelAtlas* font,int num,int round=5);

	//viplb
	void vipPage();
	void vipRewardPage(bool first = false);
    
    bool canPlayAd();
    void setPlayAd(bool bo);

	//fillupprop
	void fillupProp();

private:
    
    Menu* zuanshi;

	int _libakey;

	bool _canTouch;

	int _rewardid;

	Sprite *_sp[8];		//◊™≈Ãµƒ8∏ˆsp
    static bool _adVideo;
};

#endif
