#ifndef RANKSCENE_H_
#define RANKSCENE_H_

#include"cocos2d.h"
using namespace cocos2d;

#include "cocos-ext.h"
using namespace extension;

struct InfoData {
	bool isuser;	//是否是自己
	std::string name;	//名字
	Texture2D* image;	//图像
	std::string urlname;	//头像名字
	std::string imsi;	//区别每个用户

	int order;	//排名
	int score;	//分数

	std::string bonus;	//奖金
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
	//排行榜主界面
	void rankMainPage(int type=101);

	//排行banner
	Node* rankBanner(InfoData data,int type=1,int rank=101);
	Node* rankBanner2(InfoData data, int type);

	//滑动框
	ScrollView* rankList(int key);

	//增加玩家
	void addUser(InfoData &user,int type);

	//玩家数据排行
	void rankFlush();

	//添加假数据
	void addVirtualUser();

	//添加玩家数据
	void addGameUserData();

	//提交玩家数据
	void sumbitUserScore(int score,int topType);

	//设置玩家头像和名字
	void setUserInfor(std::string name, std::string url);

	//获取当前玩家
	InfoData* getUserData(int type);
	InfoData* getUserData(int order, int type);
	InfoData* getUserDataByImei(std::string imsi,int type);

	//获取默认头像
	Texture2D* getIcon(int key);

	//设置服务器用户的头像
	void setServerIcon(int type);

	//玩家分数排序
	void rankSort(int type);
	void sortAlgorithm(std::vector<InfoData> &data, int left, int right);

	//清除排行榜数据
	void cleanInfoData(int type);

	//服务器信息
//	void setServerInfo()
	//
	void timeUpdate(float ft);

	//选择排行榜
	void setRank(int type);

	//用户头像显示更新
	void imgUpdate();

	//得到头像
	Node* getTouXiang(Texture2D* img);

public:
	unsigned int _time;
	int _second;
	Label *_timeLabel;

	int _type;

	//玩家数据
	std::vector<InfoData> _InfoData[3];

	//用户头像是否已经显示
	std::vector<bool> _userImg[3];

	bool _userselfImg[3];

	//服务器数据
	//InfoServer* _serverInfo;
};

#endif
