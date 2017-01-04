#pragma once

#include"ConfigState.h"

class BaseManage :public Layer
{
public:
	virtual bool init() { return true; };

	CREATE_FUNC(BaseManage);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event) { return false; };
	virtual void onTouchMoved(Touch *touch, Event *unused_event) {};
	virtual void onTouchEnded(Touch *touch, Event *unused_event) {};

public:
	GameState _gamestate;
	int _score;
	int _target;
	int _level;
	int _stepnumber;
	ShapeIndex _nextBox[3];

public:
	//分数
	void setmScore(int score) { _score = score; }
	int getmScore() { return _score; }
	//等级
	void setmLevel(int level) { _level = level; }
	int getmLevel() { return _level; }
	//目标
	void setmTarget(int target) { _target = target; }
	int getmTarget() { return _target; }
	//游戏状态
	void setGameState(GameState state) { _gamestate = state; }
	GameState getGameState() { return _gamestate; }
	//步数
	void setmStepNumber(int stepnumber) { _stepnumber = stepnumber; }
	int getmStepNumber() { return _stepnumber; }
	//下移方块
	ShapeIndex getNextBox(int i) { return _nextBox[i]; }

	CC_SYNTHESIZE(int, _diamondnumber, Diamond);	//钻石数
	CC_SYNTHESIZE(TouchState, _touchState, TouchState);	//是否接受点击

	virtual void start() {}		//继续
	virtual void pause() {}	//暂停
	virtual void run() {}; //开始
	virtual void Resurrection() {}	//复活
};