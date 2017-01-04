#include"Login.h"
#include"../cj/Toast.h"
#include"../cj/cjconfig.h"
#include"../cj/GameData.h"
#include"NetWork.h"

//΢�ŵ�¼
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

//�Ƿ���Ե�¼
bool Login::if_Login()
{
	if (SocialManager::getSocialAgent(SocialType::WeChat))
		return true;
	return false;
}

//΢����Ϣˢ��
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
		/*action �û�����
		pid   ��ĿID
		appid ��ƷID
		chlid �ƶ�MM channel id
		lsn   �豸ʶ����
		imsi  �ֻ�IMSI��
		imei  �ֻ�ʶ����

		userid �û�ID��LSN��
		username �û�����
		hiscore ��ʷ��߷�
		hilevel ��ʷ����
		pkwins  ȫ��PK�ھ���
		avatar  ͷ��
		coin	��ʯ
		ticket	�һ�ȯ*/

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
