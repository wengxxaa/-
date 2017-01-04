#include"Login.h"
#include"../cj/Toast.h"
#include"../cj/cjconfig.h"
#include"../cj/GameData.h"
#include"NetWork.h"

//微信登录
void Login::wx_Login()
{
	cj::LogText("wx_Login");
	SocialManager::login(SocialType::WeChat, [=](SocialRetCode retCode, std::string reason) {
		if (retCode == SocialRetCode::Success)
		{
			cj::LogText("wx_Login_success");

			wx_updateUserInfo();

			//cj::LogText("wx_Update_success");
		}
		else
			cj::LogText("wx_Login_faild");
	});

}

//是否可以登录
bool Login::if_Login()
{
	if (SocialManager::getSocialAgent(SocialType::WeChat))
		return true;
	return false;
}

//微信信息刷新
void Login::wx_updateUserInfo()
{
	SocialManager::updateUserInfo(SocialType::WeChat, [=](SocialRetCode retCode, std::string reason) {
		if (retCode == SocialRetCode::Success)
		{
			_userInfo = (WeChatUserInfo*)SocialManager::getUserInfo(SocialType::WeChat);

			cj::LogText(_userInfo->nickname);
			cj::LogText(_userInfo->headimgUrl);

			RankScene::getInstance()->setUserInfor(_userInfo->nickname, _userInfo->headimgUrl);
			submitWeChatData();
            
            GameData::getSaveData()->_isWxLoad=true;
            GameData::getInstance()->dataSave();
            
			cj::LogText("wx_Update_success");
		}
		else
			cj::LogText("wx_Update_faild");
	});
}

void Login::submitWeChatData()
{
#if( CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	WeChatUserInfo *wxInfo = getUserInfo();
	std::thread _thread([this, wxInfo]() {
		/*action 用户命令
		pid   项目ID
		appid 产品ID
		chlid 移动MM channel id
		lsn   设备识别码
		imsi  手机IMSI号
		imei  手机识别码

		userid 用户ID（LSN）
		username 用户名称
		hiscore 历史最高分
		hilevel 历史关数
		pkwins  全民PK冠军数
		avatar  头像
		coin	钻石
		ticket	兑换券*/

		//SocialUserInfo wxInfo = SocialManager::getInstance()->getUserInfo();
		std::string url = URL_RANK_DATA;
		std::string param = "action=";
		param += URL_ACTION_SETUSER;
		param += "&pid=" + vigame::SysConfig::getInstance()->getPrjid();
		param += "&appid=" + vigame::SysConfig::getInstance()->getAppid();
		param += "&chlid=" + vigame::SysConfig::getInstance()->getMmid();
		param += "&lsn=" + vigame::SysConfig::getInstance()->getLsn();
		param += "&imsi=" + vigame::SysConfig::getInstance()->getImsi();
		param += "&imei=" + vigame::SysConfig::getInstance()->getImei();
		param += "&userid=" + wxInfo->openid;
		param += "&username=" + URL_ENCODE(wxInfo->nickname);
		param += "&avatar=" + wxInfo->headimgUrl;
		//param+="&coin="+std::string("200");
		//param+="&ticket="+std::string("20");

		std::string encode = vigame::base64_encode(param);
		url += encode;
		vigame::http::response  respon = vigame::http::get(url.c_str());
		string result = respon.body;
		log("WxControl===%s", param.c_str());
		log("WxControl===%s", result.c_str());
	});
	_thread.detach();
#endif
}
