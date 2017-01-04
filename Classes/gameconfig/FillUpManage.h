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
	void start() { setGameState(STATE_RUN); _timeDecrease = true;}		//����
	void pause() { setGameState(STATE_PAUSE); _timeDecrease = false; }	//��ͣ
	void run() {  }	//��ʼ
	void Resurrection();	//����

private:
	//��ʼ������
	void initChess();

	//��ʼ������
	void initData();

	//��ʼ����
	void startDrop(bool key=false);

	//λ��ת��
	Vec2 posToposition(pair<int, int> pos);
	Vec2 posToposition2(pair<int, int> pos);
	pair<int, int> positionTopos(Vec2 vec);

	//�õ���Χ��ͬ��ɫ�ķ���
	vector<pair<int, int>> boxGet(pair<int, int> pos);
	void boxAdd(pair<int, int> pos, vector<pair<int, int>> &vec);

	//��������
	void boxEliminate(vector<pair<int, int>> &vec,float discount=1.0f);
	//�������
	void boxFillUp();

	//�ӷ�
	void addScore(vector<pair<int, int>> &vec,float discount=1.0f);

	//���̿ɵ���ж�
	bool canTouchJudge();

	//����ʱ
	void timeUpdate(float ft);

	inline int getChessInfo(pair<int, int> pos) { return _chesscolor[pos.first*_Height+pos.second]; }

	//��������
	void scoreMotion(int score, pair<int, int> pt,int multiple);

	//���߽���������
	void barHandle();
	void propHandle();
	void propEliminate(int x,int y);

//	void useProp2(pair<int, int> pos);
//	void useProp3(pair<int, int> pos);
	void useProp4(pair<int, int> pos);

	//����ͳһ����
	void useProp2();
	//����
	void useProp3(pair<int, int> pos);
	//ʱ�䶳��
	void useProp5();
	//���
	void useProp6();

	void dataSave();

	void addParticle(int x, int y, int color,int model=1);
	void removeBox(pair<int,int> pos);

	//���ʧ�ܺ�ʱ�����
	void touchFaild();

	//
	void barRestart();

	//test
	void test();
	void testupdate(float ft);

	//���·���
	void reDrop();

	//��ɫ
	void changeBoxColor();
	void changeBoxColor1(vector<pair<int,int>> &vec);

	//�ı���ɫ
	void chaneBoxId(pair<int, int> pos, int id);

	//���ģʽ
	void crazyAdd(vector<pair<int, int>> &vec);

	//����ʱ
	CC_SYNTHESIZE(int, _timenumber, timeNumber);

	//�õ������ɫ��λ��
	pair<int,int> getMostestBox();

	//��ʾ������
	void showLiJi(Vec2 pos);

	//ʱ����������5�붯��
	void timeSliderMotion();

	//���ģʽ������ر�
	void toolSliderMotion(bool open);

	//����ģ��
	int getResetId(int x, int y,int key);

	//����Ļ
	void showText(TEXT text,Vec2 pos=Vec2(240,400));

	//��ʼ��Ϸ
	void beganGame();
	//��ʼ��Ϸǰ�ĵ���
	void specialPropPage();
	//��Ϸ����
	void endGame();

	//����ָ��
	void startGuide();
	void endGuide();
	void setGuide1();
	void setGuide2();
	void setGuideText(Node *node);
	void guideTitle();

	void ignoreGuide();

	bool _guide;
	int _guidenumber;

	//��������
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

	vector<int> _chesscolor;	//���ӵ���ɫ

	ClippingNode *_clip;	//�ü�����
	NodeGrid *_waveNode;
//	Layer *_clip;	//�ü�����

	LabelAtlas *_timelabel;	//ʱ�䵹��ʱ
	LabelAtlas *_timelabel2;

	//csb�ļ�
	Node *_csbNode;

	//���߽�����
	ProgressTimer *_propbar[3];
	//ʱ�������
	ProgressTimer *_timebar;

	bool _timeDecrease;

	//��������Ϣ
	PropBar _propdata[3];
	ParticleSystemQuad* _proppar[3];
	
	int _level;
	int _propid;
	int _propscore;
	bool _propbool;
	
	//�ӷֱ���
	int _multiple;
	int _multipletime;

	//������ʱ��
	int _eliminatetime;

	bool _hightestLevel;

	//�Ƿ��ɫ
	bool _changeColor;

	//��ͣʱ��
	int _stoptime;

	int _lianji;	//������

	bool _daduanshow;

	bool _gameFaild;

	//��Ϸǰ�������������
	bool _specialProp[3];

	//��Ҫ����������
	vector<lizivec> _lizivec;

	//�Ƿ��ʱʱ��
	bool _jiashi;
};

#endif