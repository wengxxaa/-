#include "CallHelper.h"
#include <stdlib.h>
#include "Common.h"

#include "../cj/cjconfig.h"

#include <string>
using namespace std;

#include "vigame_core.h"
using namespace vigame;


//#include "views/AppInfoLayer.h"
std::string CallHelper::fetchUserName(std::string lsn)
{
	std::string  url = "http://zjh.345zjh.com:8998/PorkGetUserNameServlet?lsn=";
	url += lsn;

	std::string result = "";

	for (int i = 0; i<3; i++)
	{
		result = vigame::http::get((char*)url.c_str()).body;
		//result = HttpWorker::http_get();

		if (!result.empty())
		{
			break;
		}
	}

	return result;
}

int CallHelper::isUserOnline(std::string jid)
{
	int state = 0;

	std::string url = "http://pk.345zjh.com:9090/plugins/presence/status?jid=";
	url += jid;
	url += "&type=xml";

	std::string result = "";

	result = vigame::http::get((char*)url.c_str()).body;
	//result = HttpWorker::http_get((char*)url.c_str());

	if (result.size() > 0)
	{
		if (result.find("type=\"unavailable\"") != std::string::npos)
		{
			state = 2;
		}
		else if (result.find("type=\"error\"") != std::string::npos)
		{
			state = 0;
		}
		else if (result.find("priority") != std::string::npos || result.find("id=\"") != std::string::npos)
		{
			state = 1;
		}
	}

	return state;
}

int CallHelper::doUpdateCheck(std::string ver, std::string package, std::string prjid, std::string mmid)
{
	std::string url = "http://zjh.345zjh.com:8998/Updateapk?";
	url += "v=" + ver;
	url += "&p=" + package;
	url += "&i=" + prjid;
	url += "&m=" + mmid;

	std::string result = "";

	result = vigame::http::get((char*)url.c_str()).body;
	//result = HttpWorker::http_get((char*)url.c_str());

	return 0;
}

int CallHelper::submitIosTransaction(std::string jid, std::string transaction, int price)
{
	int ret = -1;
	char tmp[6] = { 0 };
	sprintf(tmp, "%d", price);

	//http://mj.syjhmm.com:6701/greios?
	std::string url = "http://zjh.345zjh.com:8998/greios?";
	url += "u=" + jid;
	url += "&v=";
	url += tmp;
	url += "&p=" + SysConfig::getInstance()->getPrjid();
	url += "&redata=" + transaction;

	std::string result = "";

	for (int i = 0; i<3; i++)
	{
		result = vigame::http::get((char*)url.c_str()).body;
		//result = HttpWorker::http_get((char*)url.c_str());
		if (!result.empty() && (result == "1" || result == "0" || result == "2"))
		{
			ret = atoi(result.c_str());
			break;
		}
	}

	return ret;

}

//return payType
int CallHelper::submitMmid()
{
	int ret = -1;
	std::string postString = "";
	std::string url = "http://zf.quanzhifu.net:6502/MmChnl";

	postString = "pid=" + SysConfig::getInstance()->getPrjid();
	postString += "&lsn=" + SysConfig::getInstance()->getLsn();
	postString += "&imsi=" + SysConfig::getInstance()->getImsi();
	postString += "&chlid=" + SysConfig::getInstance()->getMmid();
	postString += "&appid=" + SysConfig::getInstance()->getAppid();

	std::string base64string = base64_encode(postString);

	url += "?value=" + base64string;

	std::string result = "";

	result = vigame::http::get((char*)url.c_str()).body;
	//result = HttpWorker::http_get((char*)url.c_str());

	if (result.find("<qpay>1</qpay>") != std::string::npos)
	{
		//mm
		ret = 1;
	}

	return ret;

}

int CallHelper::submitMessage(std::string jid, std::string msg)
{
	//std::string message = encode(msg, "UTF-8");		
	std::string url = "http://zjh.345zjh.com:8998/Getuadvice?u=";
	url += jid;
	url += "&des=" + msg;

	std::string result = "";

	result = vigame::http::get((char*)url.c_str()).body;
	//result = HttpWorker::http_get((char*)url.c_str());

	if (result.find("ok") != std::string::npos)
	{
		return 1;
	}

	return 0;

}

DhmData CallHelper::useDhm(std::string dhm)
{
	DhmData ret;
	// 	if(dhm=="10021002"){
	// 		AppInfoLayer::create()->show();
	// 		return ret;
	// 	}
	std::string url = "http://dhm.51apppay.com/dhvm?value=";
	std::string param;
	param += SysConfig::getInstance()->getLsn();
	param += "#" + SysConfig::getInstance()->getImsi();
	param += "#" + SysConfig::getInstance()->getAppid();
	param += "#" + SysConfig::getInstance()->getPrjid();
	param += "#" + SysConfig::getInstance()->getMmid();
	param += "#" + dhm;

	std::string encode = base64_encode(param);
	url += encode;

	std::string result = vigame::http::get((char*)url.c_str()).body;
	//std::string result =  HttpWorker::http_get((char*)url.c_str());
	if (result.length()>0) {
		std::string::size_type start, end;

		std::string LTAG = "<price>";
		std::string RTAG = "</price>";
		std::string* tmpString = &ret.price;
		for (int i = 0; i<3; i++)
		{

			if (i == 1) {
				LTAG = "<msg>";
				RTAG = "</msg>";
				tmpString = &ret.message;
			}
			else if (i == 2) {
				LTAG = "<state>";
				RTAG = "</state>";
				tmpString = &ret.state;
			}
			start = result.find(LTAG);
			end = result.find(RTAG);
			if (start != std::string::npos && end != std::string::npos && end>start) {
				*tmpString = result.substr(start + LTAG.length(), end - start - LTAG.length());
			}

		}

	}
	return ret;
}


FreeGetData CallHelper::useFreeget(std::string segment)
{
	FreeGetData ret;
	std::string url = "http://zf.quanzhifu.net:6502/redPoint?value=";
	std::string param;
	param += SysConfig::getInstance()->getLsn();
	param += "#" + SysConfig::getInstance()->getImsi();
	param += "#" + SysConfig::getInstance()->getAppid();
	param += "#" + SysConfig::getInstance()->getPrjid();
	param += "#" + SysConfig::getInstance()->getMmid();
	param += "#" + segment;

	std::string encode = base64_encode(param);
	url += encode;

	std::string result = vigame::http::get((char*)url.c_str()).body;
	//std::string result =  HttpWorker::http_get((char*)url.c_str());
	if (result.length()>0) {
		std::string::size_type start, end;

		std::string LTAG = "<segment>";
		std::string RTAG = "</segment>";
		std::string* tmpString = &ret.segment;
		for (int i = 0; i<3; i++)
		{

			if (i == 1) {
				LTAG = "<msg>";
				RTAG = "</msg>";
				tmpString = &ret.message;
			}
			else if (i == 2) {
				LTAG = "<state>";
				RTAG = "</state>";
				tmpString = &ret.state;
			}
			start = result.find(LTAG);
			end = result.find(RTAG);
			if (start != std::string::npos && end != std::string::npos && end>start) {
				*tmpString = result.substr(start + LTAG.length(), end - start - LTAG.length());
			}

		}

	}
	return ret;
}


std::string CallHelper::_submitGameParam(const std::string& name, const std::string& value)
{
	string encode = vigame::base64_encode(value);
	//string url = "http://192.168.1.27:8080/dnwx-zhpay/"+name+"?value="+encode;
	string url = "http://ad.vimedia.cn:8998/" + name + "?value=" + encode;

	cj::LogText("Submit:" + url);

	std::string result = vigame::http::get((char*)url.c_str()).body;
	return result;
	//return HttpWorker::http_get(url.c_str());
}

std::string CallHelper::_getValue(const std::string& result, const char* startstr, const char* endstr)
{
	string::size_type start;
	string::size_type end;
	start = result.find(startstr);
	if (start != string::npos) {
		start += strlen(startstr);
		end = result.find(endstr, start);
		if (end != string::npos) {
			return result.substr(start, end - start);
		}
	}
	return "";
}

std::string CallHelper::submitGameBonus(int level, int score, int usedCoins, int leftCoins, int topType)
{
	SysConfig* pConfig = SysConfig::getInstance();
	char buffer[64] = { 0 };
	sprintf(buffer, "%d", level);
	string strLevel = buffer;
	sprintf(buffer, "%d", score);
	string strScore = buffer;
	sprintf(buffer, "%d", usedCoins);
	string strUsed = buffer;
	sprintf(buffer, "%d", leftCoins);
	string strLeft = buffer;
	sprintf(buffer, "%d", topType);
	string strTopTypet = buffer;
	string value = "pid=" + pConfig->getPrjid()
		+ "&lsn=" + pConfig->getLsn()
		+ "&imsi=" + pConfig->getImsi()
		+ "&chlid=" + pConfig->getMmid()
		+ "&appid=" + pConfig->getAppid()
		+ "&imei=" + pConfig->getImei()
		+ "&model=" + pConfig->getMoilemodel()
		+ "&topType=" + strTopTypet
		+ "&level=" + strLevel
		+ "&score=" + strScore
		+ "&coins=" + strUsed
		+ "&total=" + strLeft;
	string result = _submitGameParam(string("AlonePostServlet"), value);
	string resultCode = _getValue(result, "<resultCode>", "</resultCode>");
	return resultCode.c_str();
}

WXShareData CallHelper::submitWXShare(int level, int score)
{
	SysConfig* pConfig = SysConfig::getInstance();
	char buffer[64] = { 0 };
	sprintf(buffer, "%d", level);
	string strLevel = buffer;
	sprintf(buffer, "%d", score);
	string strScore = buffer;

	string value = "pid=" + pConfig->getPrjid()
		+ "&lsn=" + pConfig->getLsn()
		+ "&imsi=" + pConfig->getImsi()
		+ "&chlid=" + pConfig->getMmid()
		+ "&appid=" + pConfig->getAppid()
		+ "&imei=" + pConfig->getImei()
		+ "&level=" + strLevel
		+ "&score=" + strScore;

	WXShareData ret;
	string result = _submitGameParam(string("AloneShareServlet"), value);
	ret.resultCode = _getValue(result, "<resultCode>", "</resultCode>");
	ret.resultMsg = _getValue(result, "<resultMsg>", "</resultMsg>");
	ret.url = _getValue(result, "<url>", "</url>");
	ret.title = _getValue(result, "<title>", "</title>");
	ret.content = _getValue(result, "<content>", "</content>");
	return ret;
}

ExchangeData CallHelper::submitExchangeContact(string prizeName, string name, string tel, string address)
{
	SysConfig* pConfig = SysConfig::getInstance();
	string value =
		"pid=" + pConfig->getPrjid()
		+ "&lsn=" + pConfig->getLsn()
		+ "&imsi=" + pConfig->getImsi()
		+ "&chlid=" + pConfig->getMmid()
		+ "&appid=" + pConfig->getAppid()
		+ "&imei=" + pConfig->getImei()
		+ "&name=" + name
		+ "&tel=" + tel
		+ "&address=" + address
		+ "&prizename=" + prizeName;

	string result = _submitGameParam("exchange", value);
	ExchangeData ret;
	ret.content = _getValue(result, "<content>", "</content>");
	ret.resultCode = _getValue(result, "<resultCode>", "</resultCode>");
	ret.resultMsg = _getValue(result, "<resultMsg>", "</resultMsg>");
	ret.state = _getValue(result, "<state>", "</state>");
	return ret;
}

string CallHelper::getBJTime()
{
	string url = "http://wx.vigame.cn:8998/getTime";
	return vigame::http::get((char*)url.c_str()).body;
	//return HttpWorker::http_get((char*)url.c_str());
}

// void CallHelper::getAd( std::string name,OnHttpResponce responceCall )
// {
// 	SysConfig* pConfig = SysConfig::getInstance();
// 	string value = 
// 		"pid="		+	pConfig->getPrjid()
// 		+"&lsn="	+	pConfig->getLsn()
// 		+"&imsi="	+	pConfig->getImsi()
// 		+"&chlid="	+	pConfig->getMmid()
// 		+"&appid="	+	pConfig->getAppid()
// 		+"&imei="	+	pConfig->getImei()
// 		+"&adname="	+	name;
// 	string encode = base64_encode(value);
// 	string domain = get_ctrl_value("apiGet");
// 	if(domain.length()>0){
// 		std::string url = domain+"?value="+encode;
// 		AysnHttpWorker::http_get(url.c_str(),responceCall);
// 	}
// }
// 
// void CallHelper::responceAd( std::string sid,ADStatus status )
// {
// 	char buffStatus[32] = {0};
// 	sprintf(buffStatus,"%d",(int)status);
// 	SysConfig* pConfig = SysConfig::getInstance();
// 	string value = 
// 		"pid="		+	pConfig->getPrjid()
// 		+"&lsn="	+	pConfig->getLsn()
// 		+"&imsi="	+	pConfig->getImsi()
// 		+"&chlid="	+	pConfig->getMmid()
// 		+"&appid="	+	pConfig->getAppid()
// 		+"&imei="	+	pConfig->getImei()
// 		+"&sid="	+	sid
// 		+"&status=" +   buffStatus;
// 	string encode = base64_encode(value);
// 	string domain = get_ctrl_value("apiResp");
// 	if(domain.length()>0){
// 		std::string url = domain + "?value="+encode;
// 		AysnHttpWorker::http_get(url.c_str(),NULL);
// 	}
// 
// }


void CallHelper::openUserAgreement()
{
#if( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, EXTRAL_CLASS_JAVA, "openUserAgreement", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
	}
#endif
}
void CallHelper::openRank()
{
	if (MMChnlManager::getInstance()->getMMChnl()->bonus.size() == 0)
	{
		return;
	}
#if( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, EXTRAL_CLASS_JAVA, "openRank", "(Ljava/lang/String;)V"))
	{
		jstring stringUrl = t.env->NewStringUTF(MMChnlManager::getInstance()->getMMChnl()->bonus.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringUrl);
		t.env->DeleteLocalRef(t.classID);
		t.env->DeleteLocalRef(stringUrl);
	}
#endif
}
void CallHelper::openHeadlineDialog()
{
	if (MMChnlManager::getInstance()->getMMChnl()->headline.size() == 0)
	{
		return;
	}
#if( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, EXTRAL_CLASS_JAVA, "openHeadlineDialog", "(Ljava/lang/String;)V"))
	{
		jstring stringUrl = t.env->NewStringUTF(MMChnlManager::getInstance()->getMMChnl()->headline.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringUrl);
		t.env->DeleteLocalRef(t.classID);
		t.env->DeleteLocalRef(stringUrl);
	}
#endif
}
void CallHelper::openActivityWeb(int level, int score, int usedCoins, int leftCoins)
{
	if (MMChnlManager::getInstance()->getMMChnl()->activity.size() == 0)
	{
		return;
	}
#if( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, EXTRAL_CLASS_JAVA, "openActivityWeb", "(Ljava/lang/String;IIII)V"))
	{
		jstring stringUrl = t.env->NewStringUTF(MMChnlManager::getInstance()->getMMChnl()->activity.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringUrl, level, score, usedCoins, leftCoins);
		t.env->DeleteLocalRef(t.classID);
		t.env->DeleteLocalRef(stringUrl);
	}
#endif
}

void CallHelper::postLevelData(int level, int score, int usedCoins, int leftCoins)
{
	if (MMChnlManager::getInstance()->getMMChnl()->activity.size() == 0 &&
		MMChnlManager::getInstance()->getMMChnl()->bonus.size() == 0)
	{
		return;
	}
#if( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, EXTRAL_CLASS_JAVA, "postLevelData", "(IIII)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID, level, score, usedCoins, leftCoins);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}
