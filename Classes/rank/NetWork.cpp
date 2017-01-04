#include "NetWork.h"
#include "RankScene.h"
#include "../cj/cjconfig.h"
#include "vigame_social.h"
#include "../cj/GameData.h"
#include "Login.h"
#include "Common.h"
#include "vigame_core.h"

#include "json/rapidjson.h"

using namespace vigame;

using namespace vigame::social;

struct playerInfo {
	string appid;
	string child;
	string coins;
	string id;
	string imei;
	string imsi;
	string level;
	string lsn;
	string nickname;
	string phone;
	string pid;
	string score;
	string total;
};

//添加玩家排名数据
void NetWork::getUserIcon(std::string url, std::string imsi,int type)
{
    if(url.size()>0)
    {
        std::thread([=]() {
            HttpRequest *request = new HttpRequest();
            request->setUrl(url);
            request->setRequestType(HttpRequest::Type::GET);
            request->setTag("GET_IMAGE");
            request->setResponseCallback([=](HttpClient* client, HttpResponse* response) {
                if (response) {
                    std::vector<char>* responceData = response->getResponseData();
                    long size = responceData->size();
                    if (response->isSucceed())
                    {
						auto userdata = RankScene::getInstance()->getUserDataByImei(imsi,type);
						if (userdata)
						{
							if (size>0) {
								auto img = new Image;
								img->initWithImageData((unsigned char*)responceData->data(), responceData->size());

								auto texture = new Texture2D();
								texture->initWithImage(img);
								img->release();

								userdata->image = texture;
							}
						}
                    }
                }
            });
            
            HttpClient::getInstance()->send(request);
            request->release();
        }).detach();
    }

}

//请求排行榜数据
void NetWork::getRankData(int key)
{
	std::thread([=]() {
		cj::LogText("getRankData");

		for (int i = 0; i < RANKNUM; i++)
		{
			if (key != 2 || !RankScene::getInstance()->getUserData(101+i))
				sendRankData(101+i);
		}

		if (key == 1)
		{
			if (RankScene::getInstance()->getUserData(101) || RankScene::getInstance()->getUserData(102) || RankScene::getInstance()->getUserData(103))
			{
				if (!RankScene::getInstance()->getUserData(101) || !RankScene::getInstance()->getUserData(102) || !RankScene::getInstance()->getUserData(103))
					getRankData(2);
			}
		}

	}).detach();
}

void NetWork::sendRankData(int type)
{
	int grank = 0;
	if (RankScene::getInstance()->getUserData(type))
		grank = RankScene::getInstance()->getUserData(type)->order;

	std::string url = URL_RANK_DATA;
	std::string param = "action=";
	param += URL_ACTION_GETTOP;
	param += "&pid=" + SysConfig::getInstance()->getPrjid();
	param += "&chlid=" + SysConfig::getInstance()->getMmid();
	param += "&lsn=" + SysConfig::getInstance()->getLsn();
	param += "&appid=" + SysConfig::getInstance()->getAppid();
	param += "&phoneModel=" + SysConfig::getInstance()->getMoilemodel();
	param += "&version=" + SysConfig::getInstance()->getAppVersion();
	param += "&grank=" + StringUtils::format("%d", grank);//当前结算排行
	param += "&topType=" + StringUtils::format("%d", type);
	std::string encode = base64_encode(param);
	url += encode;
	cj::LogText("RUL:" + url);
	vigame::http::response  respon = vigame::http::get(url.c_str());
	string result = respon.body;
	//cj::LogText("RESULT" + result);
	if (result.size() > 0)
	{
		RankScene::getInstance()->cleanInfoData(type);
		parseRankData(result, type);

		GameDataInstance()->d_rankUpdateTime = time(NULL);
	}
	//else
	//{
	//	getRankData();
	//}
}

//解析从服务器取到的数据
bool NetWork::parseRankData(string &str,int type)
{
	int zijinum = 0;
	bool ziji = false;

	rapidjson::Document data;
	data.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	if (data.HasParseError())
	{
		cj::LogText("json parse error!");
		return false;
	}
	if (data.IsObject())
	{
		rapidjson::Value &rankData = data;

		//解析奖励
		cj::LogText("began jiexi jiangli");
		if ((rankData.HasMember("award")) && (rankData.HasMember("awardCount"))) {
			std::string strRew = rankData["award"].GetString();
			std::string strRewCount = rankData["awardCount"].GetString();
			if (strRew.size() > 0 && strRewCount.size() > 0) {
				std::vector<std::string> vecStrAward;
				std::vector<std::string> vecStrCount;
				Common::split(strRew, ",", vecStrAward);
				Common::split(strRewCount, ",", vecStrCount);
				for (unsigned int i = 0; (i < MIN(vecStrAward.size(), vecStrCount.size())); i++)
				{
					int rewId = atoi(vecStrAward.at(i).c_str());
					int rewCount = atoi(vecStrCount.at(i).c_str());
					_bonusInfo[type-101].reward.push_back(make_pair(rewId, rewCount));
					char name[50];
					sprintf(name, "RankData::TYPR=%d:_reward_info==%d,%d", type, rewId, rewCount);
					cj::LogText(name);
				}
			}
		}
		if (rankData.HasMember("topDt")) {
			std::string strTopDt = rankData["topDt"].GetString();
			cj::LogText(strTopDt.c_str());
			
			int bonustime = atoi(strTopDt.c_str());
			GameDataInstance()->d_bonusTime[type - 101] = bonustime;
			if (GameData::getSaveData()->_bonusTime[type-101] == bonustime)
			{
				_bonusInfo[type-101].liqu = false;
				cj::LogText("Reward:false");
			}
			else if(_bonusInfo[type - 101].reward.size()>0)
			{
				_bonusInfo[type-101].liqu = true;
				cj::LogText("Reward:true");
			}
		}

		std::string imsistr;
		if (rankData.HasMember("currList")) {
			rapidjson::Value& playerArry = rankData["currList"];
			if (playerArry.IsArray()) {
				cj::LogText(cjTTFLabel::getNameByInt("currList:%d", playerArry.Size()));
				for (unsigned int i = 0; i < playerArry.Size(); i++)
				{
					InfoData playerData;
					rapidjson::Value& player = playerArry[i];
					if (player.IsObject())
					{
						if (player.HasMember("id")) {
							playerData.order = atoi(player["id"].GetString());
						}
						if (player.HasMember("nickname")) {
							playerData.name = URL_DECODE(player["nickname"].GetString());
						}
						if (player.HasMember("total")) {
							std::string bonus = player["total"].GetString();
							if (bonus.size() > 0) {
								playerData.bonus = URL_DECODE(bonus);
							}
							else {
								playerData.bonus = bonus;
							}
						}
						if (player.HasMember("score")) {
							playerData.score = atoi(player["score"].GetString());
						}
						if (player.HasMember("coins")) {
							string _icon_url = (player["coins"].GetString());
							playerData.image = nullptr;
							playerData.urlname = "";
							if (_icon_url.size() > 0) {
								//if (playerData._icon_url.find("http") != std::string::npos) {
								//	log("RankData==headUrl===%s", playerData._icon_url.c_str());
								//	playerData._head_type = IMG_HEAD_NETWORK;
								//}
								//else {
								//	playerData._head_type = atoi(playerData._icon_url.c_str());
								//}

								if (_icon_url.find("http") != std::string::npos) {
									playerData.urlname = _icon_url;
								}
							}
						}

						if (player.HasMember("imsi"))
						{
							imsistr = player["imsi"].GetString();
							playerData.imsi= player["imsi"].GetString();
						}

					}

					playerData.isuser = true;
					RankScene::getInstance()->addUser(playerData, type);
				}
			}
		}

		if (rankData.HasMember("list")) {
			rapidjson::Value& playerArry = rankData["list"];

			//imsi
			bool imsiadd = false;
			vector<std::string> imsivector;
			//

			if (playerArry.IsArray()) {
				for (unsigned int i = 0; i < playerArry.Size(); i++)
				{
					InfoData playerData;
					rapidjson::Value& player = playerArry[i];
					imsiadd = false;
					if (player.IsObject())
					{
						zijinum++;
						if (player.HasMember("id")) {
							playerData.order = atoi(player["id"].GetString());
						}
						if (player.HasMember("nickname")) {
							playerData.name = URL_DECODE(player["nickname"].GetString());
						}
						if (player.HasMember("total")) {
							std::string bonus = player["total"].GetString();
							if (bonus.size() > 0) {
								playerData.bonus = URL_DECODE(bonus);
							}
							else {
								playerData.bonus = bonus;
							}
						}
						if (player.HasMember("score")) {
							playerData.score = atoi(player["score"].GetString());
						}
						if (player.HasMember("coins")) {
							string _icon_url = (player["coins"].GetString());
							playerData.image = nullptr;
							playerData.urlname = "";
							if (_icon_url.size() > 0) {
								if (_icon_url.find("http") != std::string::npos) {
									playerData.urlname = _icon_url;
								}
							}
						}

						if (player.HasMember("imsi"))
						{
							std::string imsi = player["imsi"].GetString();
							if (imsi.size() > 0)
							{
								if (imsi != imsistr)
								{
									if (std::find(imsivector.begin(), imsivector.end(), imsi) == imsivector.end())
									{
										imsiadd = true;
										imsivector.push_back(imsi);
									}
								}
							}
							else
								imsiadd = true;

							playerData.imsi = imsi;
						}
						else
							imsiadd = true;

					}

					if(imsiadd&&(playerData.name!=cjTTFLabel::getStringByKey("ziji"))&&(playerData.score>0))
					{
						playerData.isuser = false;
						RankScene::getInstance()->addUser(playerData, type);
					}
					//}
				}
			}
		}

#if(CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
		RankScene::getInstance()->setServerIcon(type);
#endif
	}
	return true;
}

//判断是否联网
bool NetWork::isConnectNet() 
{
	return true;
	//auto host = gethostbyname("www.baidu.com");
	//if (host)
	//	return true;
	//else
	//	return false;
}
