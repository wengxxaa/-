#ifndef NETWORK_H_
#define NETWORK_H_

#include "cocos2d.h"
#include "Common.h"
#include "RankScene.h"
#include "cocos/network/HttpRequest.h"
#include "cocos/network/HttpResponse.h"
#include "cocos/network/HttpClient.h"

using namespace cocos2d;
using namespace cocos2d::network;

#define RANK 1

static const int RANKNUM = 3;

#if 1
#define URL_RANK_DATA "http://zjh.345zjh.com:8998/LiuJiaoTopServlet?value="
#else
#define URL_RANK_DATA "http://192.168.1.27:8080/dnwx-zhpay/LiuJiaoTopServlet?value="
#endif

#define URL_ICON "http://pic.to8to.com/attch/day_160218/20160218_d968438a2434b62ba59dH7q5KEzTS6OH.png"

#define URL_ACTION_GETTOP "getTop"
#define URL_ACTION_SETUSER "setUser"

#define RANK_UPDATE_TIME 600

enum RankType {
	Model_Start=1,
	Model_LiuJiao,
};

struct BonusInfo {
	std::vector<std::pair<int,int>> reward;
	bool liqu;	//�Ƿ���ȡ
};

class NetWork
{
public:
	static NetWork* getInstance() {
		static NetWork one;
		return &one;
	}

	//�õ����ͼ��
	void getUserIcon(std::string url, std::string imsi,int type);

	//�������а�����
	void getRankData(int key=1);

	//�����ӷ�����ȡ��������

	void sendRankData(int type);

	bool parseRankData(std::string &data, int type);

	//��Ʒ
	void setBonusInfo(int type, bool liqu) {
		_bonusInfo[type].liqu = liqu;
	}

	BonusInfo* getBonusInfo(int type) {
		return &_bonusInfo[type];
	};

	//�ж��Ƿ�����
	bool isConnectNet();

private:
	BonusInfo _bonusInfo[3];

};

#endif // !NETWORK_H_

