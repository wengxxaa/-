#ifndef GameData_H_
#define GameData_H_

#include"fstream"
#include"../gameconfig/ConfigState.h"
#include"cocos2d.h"
USING_NS_CC;

struct _tetrisdata{
	bool _istip;	//�Ƿ񱣴��¼

	int _chessInfo[CHESS_SIZE_W][CHESS_SIZE_H];	//������Ϣ
	int _level;	//�ؿ�
	int _score;	//����
	int _targetscore;
	int _recentbox;	//��ǰ����
	int _recentposx;
	int _recentposy;	//��ǰλ��
	int _nextbox[3];	//��һ������

	bool _guider;	//����ָ��

	_tetrisdata(){};
	_tetrisdata& operator=(const _tetrisdata &one)
	{
		_istip=one._istip;
		for(int i=0;i<CHESS_SIZE_W;i++)
			for(int j=0;j<CHESS_SIZE_H;j++)
				_chessInfo[i][j]=one._chessInfo[i][j];
		_level=one._level;
		_score=one._score;
		_targetscore=one._targetscore;
		_recentbox=one._recentbox;
		_recentposx=one._recentposx;
		_recentposy=one._recentposy;
		for(int i=0;i<3;i++)
			_nextbox[i]=one._nextbox[i];
		_guider=one._guider;
		return (*this);
	}
};

struct _onezerodata{
	bool _istip;	//�Ƿ񱣴��¼

	int _chessInfo[CHESS_SIZE_W_1010][CHESS_SIZE_H_1010];	//������Ϣ
	int _level;	//�ؿ�
	int _score;	//����
	int _target;	//Ŀ��
	int _boxshape[3];	//ѡ�񷽿����״
	int _boxangle[3];	//��ѡ����ĽǶ�

	bool _guider;	//����ָ��

	_onezerodata(){};
	_onezerodata& operator=(const _onezerodata& one){
		_istip=one._istip;
		for(int i=0;i<CHESS_SIZE_W_1010;i++)
			for(int j=0;j<CHESS_SIZE_H_1010;j++)
				_chessInfo[i][j]=one._chessInfo[i][j];
		_level=one._level;
		_score=one._score;
		_target=one._target;
		for(int i=0;i<3;i++)
		{
			_boxshape[i]=one._boxshape[i];
			_boxangle[i]=one._boxangle[i];
		}
		_guider=one._guider;
		return (*this);
	}
};

struct _startdata{
	bool _istip;	//�Ƿ񱣴��¼

	int _chessInfo[CHESS_SIZE_W_START][CHESS_SIZE_H_START];	//������Ϣ
	int _level;	//�ؿ�
	int _score;	//����
	int _target;	//Ŀ��
	int _lastlevelscore;	//��һ�ط���

	int _highestscore;	//��߷�
	bool _guider;	//����ָ��

	_startdata(){};
	_startdata& operator=(const _startdata &one)
	{
		_istip=one._istip;
		for(int i=0;i<CHESS_SIZE_W_START;i++)
			for(int j=0;j<CHESS_SIZE_H_START;j++)
				_chessInfo[i][j]=one._chessInfo[i][j];
		_level=one._level;
		_score=one._score;
		_target=one._target;
		_lastlevelscore=one._lastlevelscore;
		_highestscore=one._highestscore;
		_guider=one._guider;

		return (*this);
	}
};

struct _angledate{
	bool _istip;	//�Ƿ񱣴��¼
	int _chessInfo[CHESS_SIZE_W_ANGLE][CHESS_SIZE_H_ANGLE];	//������Ϣ
	int _level;	//�ؿ�
	int _score;	//����
	int _target;	//Ŀ��
	int _boxshape[3];	//ѡ�񷽿����״
	int _highestscore;	//��߷�

	bool _guider;	//����ָ��

	_angledate(){};
	_angledate& operator=(const _angledate& one){
		_istip=one._istip;
		for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
			for(int j=0;j<CHESS_SIZE_H_ANGLE;j++)
				_chessInfo[i][j]=one._chessInfo[i][j];
		_level=one._level;
		_score=one._score;
		_target=one._target;
		for(int i=0;i<3;i++)
		{
			_boxshape[i]=one._boxshape[i];
		}
		_highestscore = one._highestscore;
		_guider=one._guider;
		return (*this);
	}
};

struct _fillupdata {
	bool _istip;	//�Ƿ񱣴��¼

	int _chessInfo[175];	//������Ϣ
	int _score;	//����
	int _time;	//ʱ��
	int _propid;	//����id
	int _highestscore;	//��߷�
	int _level;	//���ߵȼ�
	int _propscore;	//���߷���
	int _multiple;	//�ӷֱ���
	int _multipletime;	//�ӷ�ʱ��
	int _eliminatetime;	//�������ʱ��
	bool _hightestlevel;	//��߼�
	bool _firstEnter;
	bool _specialProp[3];	//�������

	bool _guider;	//����ָ��

	_fillupdata& operator=(const _fillupdata& one)
	{
		_istip = one._istip;
		for (int i = 0; i < 175; i++)
			_chessInfo[i] = one._chessInfo[i];
		_score = one._score;
		_time = one._time;
		_propid = one._propid;
		_highestscore = one._highestscore;
		_level = one._level;
		_propscore = one._propscore;
		_guider = one._guider;
		_multiple = one._multiple;
		_multipletime = one._multipletime;
		_eliminatetime = one._eliminatetime;
		_hightestlevel = one._hightestlevel;
		_firstEnter = one._firstEnter;

		for (int i = 0; i < 3; i++)
			_specialProp[i] = one._specialProp[i];

		return (*this);
	}
};

struct _data{
	_tetrisdata _challengesstetris;
	_onezerodata _onezero;
	_startdata _start;
	_angledate _angle;
	_fillupdata _fillup;

	bool _xinshou;
	int _diamondNumber;		//��ʯ����
	int _propnum[4];	//���߸���
	int _hongbaotime;	//���ʱ��
	int _hongbaonum;	//��ȡ�������
	int _starthongbaonum;	//����ģʽ�������
	int _anglehongbaonum;	//����ģʽ�������
	bool _music;	//����
	bool _xieyi;	//�û�Э��
	int _qd_day;	//ǩ��������
	int _qd_number;	//����ǩ��������

	int _gg_guanshu;	//������صĹ���

	int _levelreward_num;	//���ؽ�������ȡ����
	int _day_num;	//��һ�ε�¼������ʱ��
	bool _freeround;	//�������ת��
	int _freeroundday;	//�ϴ��������ת�̵�����
	int _freeroundnum;	//���ת����Ĵ���
	int _freediamondnum;	//�����ʯ����Ĵ���
	//
	int _huhightnum;	//����������߸���
	int _hunum;	//�������ĵ�ǰ����
	int _hupropid;	//�����ĵ���

	int _bonusTime[3];	//��һ����ȡ������ʱ��

	bool _starPraise;	//�Ƿ����Ǻ���
    
    bool _isWxLoad; //΢���Ƿ��Ѿ���¼

	bool _isMonthPay;	//�Ƿ����˰������
	int _monthpay_day;	//�������������
	int _monthpay_month;	//�������������

	//fillupprop
	bool _fillupprop[3];
};

class SceneManage;

class GameData:public Ref
{
public:
	GameData():_saving(false){
	};
	~GameData(){
	};

	static GameData* getInstance(){
		static GameData one;
		return &one;
	}

private:
	void dataRead();

public:
	void dataSave();

	void loadSaveFile();

	void dataReset();

	inline static _data* getSaveData()
	{
		return &getInstance()->d_saveData;
	}

	//�����ļ���ʼ��
	void configInit();

public:
	GameProject d_Project;

	//����˹����
	GameModel d_Model;

	//umeng��ʱ
	time_t d_time;

	//ָ��ģʽ
	bool _guide;

	//
	int BANER_HEIGHT;

	//���ڵ�ʱ������
	int d_daytime;
	int d_monthtime;
	int d_monthday;

	//���ؽ���
	bool d_levelReward;

	int d_PayType;

	//����main�Ĵ���
	int main_num;

	//�Ƿ��ɹ��ؽ�������ת��
	bool d_jointurn;

	//��ҳ�������
	bool d_giftfist;

	//ת������
	map<int, vector<int>> d_turndata;

	//�����а�
	bool d_rank;

	//���а����ʱ��
	int d_rankUpdateTime;

	//fillupģʽ����1
	map<pair<int,char>, string> d_fillup1;
	//fillupģʽ����2
	map<pair<int, char>, string> d_fillup2;
	//fillupģʽ����3
	map<pair<int, char>, string> d_fillup3;

	//reset shuju
	map<int, vector<int>> d_fillupreset;

	//�ӷ�����ȡ������ȡʱ��
	int d_bonusTime[3];

	//������Ϸ����
	int d_gameEnter;
	//���Ǻ���Ҫ�����
	int d_starNum;

	//ios AD
	bool d_iosad;

	//haoping
	bool d_haoping;

	//restat
	bool d_restart;

	//��ʱ�����
	bool d_xianshidalibao;

private:
	_data d_saveData;

	bool _saving;
};

#define GameDataInstance() GameData::getInstance()

#endif
