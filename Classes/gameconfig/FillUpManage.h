#ifndef FILLUPMANAGE_H_
#define FILLUPMANAGE_H_

#include"cocos2d.h"
#include"../cj/cjconfig.h"
#include"../cj/GameData.h"
#include "ui/CocosGUI.h"
#include "BaseManage.h"

struct PropBar
{
	int score;
	int multiple;
	float eliminateTime;
	int multipletime;
	vector<int> prop;
	int propNumber;
	float eliminateScore;
	bool interrupt;
	float scoreReduce;
};

struct lizivec
{
	int x;
	int y;
	int color;
	lizivec(int _x, int _y, int _color)
	{
		x = _x;
		y = _y;
		color = _color;
	}
};

enum TEXT
{
	daduan,
	kaishi,
	shijiandao,
	yubei,
	fengkuang,
	jxxc

};

class FillUpManage :public BaseManage
{
public:
	virtual bool init();

	CREATE_FUNC(FillUpManage);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event) {}
	virtual void onTouchEnded(Touch *touch, Event *unused_event) {}

	inline int getmTime() { return _Time; }

public:
	void start() { setGameState(STATE_RUN); _timeDecrease = true;}		//继续
	void pause() { setGameState(STATE_PAUSE); _timeDecrease = false; }	//暂停
	void run() {  }	//开始
	void Resurrection();	//复活

private:
	//初始化棋盘
	void initChess();

	//初始化数据
	void initData();

	//开始下落
	void startDrop(bool key=false);

	//位置转换
	Vec2 posToposition(pair<int, int> pos);
	Vec2 posToposition2(pair<int, int> pos);
	pair<int, int> positionTopos(Vec2 vec);

	//得到周围相同颜色的方块
	vector<pair<int, int>> boxGet(pair<int, int> pos);
	void boxAdd(pair<int, int> pos, vector<pair<int, int>> &vec);

	//方块消除
	void boxEliminate(vector<pair<int, int>> &vec,float discount=1.0f);
	//方块填充
	void boxFillUp();

	//加分
	void addScore(vector<pair<int, int>> &vec,float discount=1.0f);

	//棋盘可点击判断
	bool canTouchJudge();

	//倒计时
	void timeUpdate(float ft);

	inline int getChessInfo(pair<int, int> pos) { return _chesscolor[pos.first*_Height+pos.second]; }

	//分数动画
	void scoreMotion(int score, pair<int, int> pt,int multiple);

	//道具进度条处理
	void barHandle();
	void propHandle();
	void propEliminate(int x,int y);

//	void useProp2(pair<int, int> pos);
//	void useProp3(pair<int, int> pos);
	void useProp4(pair<int, int> pos);

	//棋盘统一消除
	void useProp2();
	//重排
	void useProp3(pair<int, int> pos);
	//时间冻结
	void useProp5();
	//疯狂
	void useProp6();

	void dataSave();

	void addParticle(int x, int y, int color,int model=1);
	void removeBox(pair<int,int> pos);

	//点击失败和时间过长
	void touchFaild();

	//
	void barRestart();

	//test
	void test();
	void testupdate(float ft);

	//重新分配
	void reDrop();

	//变色
	void changeBoxColor();
	void changeBoxColor1(vector<pair<int,int>> &vec);

	//改变颜色
	void chaneBoxId(pair<int, int> pos, int id);

	//疯狂模式
	void crazyAdd(vector<pair<int, int>> &vec);

	//倒计时
	CC_SYNTHESIZE(int, _timenumber, timeNumber);

	//得到最多颜色的位置
	pair<int,int> getMostestBox();

	//显示连击数
	void showLiJi(Vec2 pos);

	//时间进度条最后5秒动画
	void timeSliderMotion();

	//疯狂模式开启与关闭
	void toolSliderMotion(bool open);

	//重排模板
	int getResetId(int x, int y,int key);

	//弹字幕
	void showText(TEXT text,Vec2 pos=Vec2(240,400));

	//开始游戏
	void beganGame();
	//开始游戏前的道具
	void specialPropPage();
	//游戏结束
	void endGame();

	//新手指引
	void startGuide();
	void endGuide();
	void setGuide1();
	void setGuide2();
	void setGuideText(Node *node);
	void guideTitle();

	void ignoreGuide();

	bool _guide;
	int _guidenumber;

	//消除粒子
	void eliminateLizi(float ft);

private:
	//csv
	int _Width;
	int _Height;
	int _Time;
	float _DropTime;
	int _BaseScore;
	int _ChessSize;
	float _ChessScale;
	int _BoxIndex;
	int _BoxChangeNum;

	map<int, int> _ScoreConfig;

	vector<int> _chesscolor;	//棋子的颜色

	ClippingNode *_clip;	//裁剪区域
	NodeGrid *_waveNode;
//	Layer *_clip;	//裁剪区域

	LabelAtlas *_timelabel;	//时间倒计时
	LabelAtlas *_timelabel2;

	//csb文件
	Node *_csbNode;

	//道具进度条
	ProgressTimer *_propbar[3];
	//时间进度条
	ProgressTimer *_timebar;

	bool _timeDecrease;

	//道具条信息
	PropBar _propdata[3];
	ParticleSystemQuad* _proppar[3];
	
	int _level;
	int _propid;
	int _propscore;
	bool _propbool;
	
	//加分倍数
	int _multiple;
	int _multipletime;

	//点击间隔时间
	int _eliminatetime;

	bool _hightestLevel;

	//是否变色
	bool _changeColor;

	//暂停时间
	int _stoptime;

	int _lianji;	//连击数

	bool _daduanshow;

	bool _gameFaild;

	//游戏前的三个特殊道具
	bool _specialProp[3];

	//将要消除的粒子
	vector<lizivec> _lizivec;

	//是否加时时间
	bool _jiashi;
};

#endif