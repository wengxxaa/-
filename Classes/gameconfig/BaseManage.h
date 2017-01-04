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
	//����
	void setmScore(int score) { _score = score; }
	int getmScore() { return _score; }
	//�ȼ�
	void setmLevel(int level) { _level = level; }
	int getmLevel() { return _level; }
	//Ŀ��
	void setmTarget(int target) { _target = target; }
	int getmTarget() { return _target; }
	//��Ϸ״̬
	void setGameState(GameState state) { _gamestate = state; }
	GameState getGameState() { return _gamestate; }
	//����
	void setmStepNumber(int stepnumber) { _stepnumber = stepnumber; }
	int getmStepNumber() { return _stepnumber; }
	//���Ʒ���
	ShapeIndex getNextBox(int i) { return _nextBox[i]; }

	CC_SYNTHESIZE(int, _diamondnumber, Diamond);	//��ʯ��
	CC_SYNTHESIZE(TouchState, _touchState, TouchState);	//�Ƿ���ܵ��

	virtual void start() {}		//����
	virtual void pause() {}	//��ͣ
	virtual void run() {}; //��ʼ
	virtual void Resurrection() {}	//����
};