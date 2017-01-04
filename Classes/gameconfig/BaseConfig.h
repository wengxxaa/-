#ifndef BASE_CONGIF_H_
#define BASE_CONGIF_H_

#include"iostream"
#include"ConfigState.h"
#include"cocos2d.h"
#include"../cj/cjconfig.h"
#include "../cj/GameData.h"
using namespace std;
USING_NS_CC;

const Color3B particleColor[10]={
	Color3B(153,51,255),
	Color3B(76,85,255),
	Color3B(255,206,38),
	Color3B(43,217,153),
	Color3B(89,183,255),
	Color3B(255,76,175),
	Color3B(255,63,69),
	Color3B(0,201,80),
	Color3B(255,153,64),
	Color3B(220,64,255),
};

const Color3B angleColor[9]={
	Color3B(217,87,91),
	Color3B(217,147,87),
	Color3B(217,87,160),
	Color3B(217,191,87),
	Color3B(85,211,110),
	Color3B(100,87,217),
	Color3B(87,217,169),
	Color3B(87,160,217),
	Color3B(163,87,217)
};

const string startstr1[5]={
	//"fangkuai_5.png",
	//"fangkuai_8.png",
	//"fangkuai_1.png",
	//"fangkuai_7.png",
	//"fangkuai_3.png",
	"box1-01.png",
	"box2-01.png",
	"box3-01.png",
	"box4-01.png",
	"box5-01.png"
};

const string startstr2[5]={
	//"fangkuai_5_01.png",
	//"fangkuai_8_01.png",
	//"fangkuai_1_01.png",
	//"fangkuai_7_01.png",
	//"fangkuai_3_01.png",
	"box1-02.png",
	"box2-02.png",
	"box3-02.png",
	"box4-02.png",
	"box5-02.png"
};

const float scorevideotime=2.0f;//得分音效容许的间断时间
const float sprintshowtime=1.5f;	//精灵动画时间
static int levelupmusic=0;	//升级音效
const float boxgraytime=0.03f;	//方块变灰速度

const float startnexttime=0.1f;	//填充方块的移动时间
const int nextboxoffest=150;
const int nextboxbegin=80;	
const int boxtofiger=60;	//方块距离手指的距离
const float propsheight=650;
const float liuxingtime=0.6f;	//流星雨下落时间
const float levelupscale = 1.0f;	//关数字体大小
const float chessmovetime = 0.2f;	//方块向下滑动的时间
const float boxeliminatetime = 0.5f;	//方块消失和动画的时间
//class Base

const Vec2 ignoreps = Vec2(180, 200);
const string ignorestr = "animation/Mainpage/ignore.png";

typedef enum _BoxState_
{
	STATE_01,
	STATE_02,
}BoxState;

//星星棋子
class StartBox:public cocos2d::Node
{
public:
	StartBox(){};
	~StartBox(){};

	static StartBox* createBox(int color)
	{
		StartBox *box=new (std::nothrow)StartBox();
		if(box&&box->init(color))
		{
			box->autorelease();
			return box;
		}
		else
		{
			CC_SAFE_DELETE(box);
			return nullptr;
		}
	}

	virtual bool init(int color);
	void setBox2();
	void setBox1();

public:
	int _color;

	BoxState _state;
};



#endif