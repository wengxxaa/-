#ifndef RANKSCENE_H_
#define RANKSCENE_H_

#include"cocos2d.h"
using namespace cocos2d;

#include "cocos-ext.h"
using namespace extension;

struct InfoData {
	bool isuser;	//�Ƿ����Լ�
	std::string name;	//����
	Texture2D* image;	//ͼ��
	std::string urlname;	//ͷ������
	std::string imsi;	//����ÿ���û�

	int order;	//����
	int score;	//����

	std::string bonus;	//����
};

//struct InfoServer {
//	int count;
//	string res;
//	string msg;
//	string notice;
//	string award;
//	string awardCount;
//};

class RankScene
{
public:
	static RankScene* getInstance()
	{
		static RankScene one;
		return &one;
	}

public:
	//���а�������
	void rankMainPage(int type=101);

	//����banner
	Node* rankBanner(InfoData data,int type=1,int rank=101);
	Node* rankBanner2(InfoData data, int type);

	//������
	ScrollView* rankList(int key);

	//�������
	void addUser(InfoData &user,int type);

	//�����������
	void rankFlush();

	//��Ӽ�����
	void addVirtualUser();

	//����������
	void addGameUserData();

	//�ύ�������
	void sumbitUserScore(int score,int topType);

	//�������ͷ�������
	void setUserInfor(std::string name, std::string url);

	//��ȡ��ǰ���
	InfoData* getUserData(int type);
	InfoData* getUserData(int order, int type);
	InfoData* getUserDataByImei(std::string imsi,int type);

	//��ȡĬ��ͷ��
	Texture2D* getIcon(int key);

	//���÷������û���ͷ��
	void setServerIcon(int type);

	//��ҷ�������
	void rankSort(int type);
	void sortAlgorithm(std::vector<InfoData> &data, int left, int right);

	//������а�����
	void cleanInfoData(int type);

	//��������Ϣ
//	void setServerInfo()
	//
	void timeUpdate(float ft);

	//ѡ�����а�
	void setRank(int type);

	//�û�ͷ����ʾ����
	void imgUpdate();

	//�õ�ͷ��
	Node* getTouXiang(Texture2D* img);

public:
	unsigned int _time;
	int _second;
	Label *_timeLabel;

	int _type;

	//�������
	std::vector<InfoData> _InfoData[3];

	//�û�ͷ���Ƿ��Ѿ���ʾ
	std::vector<bool> _userImg[3];

	bool _userselfImg[3];

	//����������
	//InfoServer* _serverInfo;
};

#endif
