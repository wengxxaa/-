/*
*
* CallHelper.h
*
* Created by Juns [2014-01-20]
*/


#ifndef __CALLHELPER_H__
#define __CALLHELPER_H__
#include <map>

class DhmData {

public:
	DhmData()
		:state("-1") {}
	std::string price;//价格：分
	std::string message;//提示文字
	std::string state;//使用状态（-1未使用，0兑换码不存在，1成功，2不能重复使用，3参数错误）
};
//整点免费领取数据
class FreeGetData
{
public:
	FreeGetData()
		:state("-1") {}
	std::string segment;//领取成功的时间段
	std::string message;//提示文字
	std::string state;//使用状态：-1初始值，即无网络情况 0不在时间段内 1在时间段内 (暂时没用:2不能重复领奖 3参数错误)
protected:
private:
};

class WXShareData {
public:
	WXShareData() {}
	std::string resultCode;		//响应标志, 0表示成功, 其他表示失败
	std::string	resultMsg;		//响应内容 eg:成功
	std::string	url;            //分享链接
	std::string	title;          //分享标题
	std::string	content;        //分享内容
};

class ExchangeData {
public:
	std::string resultCode;
	std::string resultMsg;
	std::string content;//提示内容
	std::string state;//兑换标志：0表示成功兑换， -1表示奖品已送完，下次再来  -2表示非法请求
};

//typedef std::map<std::string,std::string> AdMap;

typedef enum _AdStatus {
	ADSTATUS_SUCCESS,//展示成功
	ADSTATUS_LOAD_FAILED,//广告加载失败（包括初始化失败，预加载失败等都归本类）
	ADSTATUS_SHOW_FAILED,//广告展示失败
}ADStatus;

class CallHelper
{

public:

	//获取用户ID
	static std::string fetchUserName(std::string lsn);
	//判断用户是否在线
	static int isUserOnline(std::string jid);
	//检查更新
	static int doUpdateCheck(std::string ver, std::string package, std::string prjid, std::string mmid);
	//提交反馈
	static int submitMessage(std::string jid, std::string msg);

	static int submitIosTransaction(std::string jid, std::string transaction, int price);
	//send mmid to server
	static int submitMmid();



	//************************************
	// Method:    useDhm-获取兑换结果
	// FullName:  CallHelper::useDhm
	// Access:    public static 
	// Returns:   DhmData
	// Qualifier:
	// Parameter: std::string dhm
	//************************************
	static DhmData useDhm(std::string dhm);


	//************************************
	// Method:    useFreeget - 使用整点领取
	// FullName:  CallHelper::useFreeget
	// Access:    public static 
	// Returns:   FreeGetData
	// Qualifier:
	// Parameter: std::string segment eg:11,13
	//************************************
	static FreeGetData useFreeget(std::string segment);

	//************************************
	// Method:    submitGameBonus 提交游戏排名数据
	// FullName:  CallHelper::submitGameBonus
	// Access:    public 
	// Returns:   int resultCode  0成功，其他表示失败
	// Qualifier:
	// Parameter: int lever 关卡
	// Parameter: int score 得分
	// Parameter: int usedCoins 消耗钻石
	// Parameter: int leftCoins 剩余钻石
	// Parameter: int topType 排行榜类型
	//************************************
	static std::string submitGameBonus(int level, int score, int usedCoins, int leftCoins, int topType = 0);

	//************************************
	// Method:    submitWXShare 请求微信分享数据
	// FullName:  CallHelper::submitWXShare
	// Access:    public 
	// Returns:   WXShareData
	// Qualifier:
	// Parameter: int lever 关卡
	// Parameter: int score 得分
	//************************************
	static WXShareData submitWXShare(int level, int score);
	//************************************
	// Method:    submitExchangeContact
	// FullName:  CallHelper::submitExchangeContact
	// Access:    public static 
	// Returns:   ExchangeData
	// Qualifier: 提交用户的兑换信息
	// Parameter: string name
	// Parameter: string tel
	// Parameter: string address
	//************************************
	static ExchangeData submitExchangeContact(std::string prizeName, std::string name, std::string tel, std::string address);
	//获取北京时间
	static std::string getBJTime();

	//   //获取需要展示的广告(异步方法)
	//   static void getAd(std::string name,OnHttpResponce responceCall); 
	//   //提交广告展示结果(异步方法)
	//   static void responceAd(std::string sid,ADStatus status);

	static std::string _getValue(const std::string& result, const char* startstr, const char* endstr);

	//打开用户协议
	static void openUserAgreement();
	//打开排行
	static void openRank();
	//打开公告
	static void openHeadlineDialog();
	//打开活动
	static void openActivityWeb(int level, int score, int usedCoins, int leftCoins);
	//提交关卡数据
	static void postLevelData(int level, int score, int usedCoins, int leftCoins);
protected:
	static std::string _submitGameParam(const std::string& name, const std::string& value);

};


#endif //__CALLHELPER_H__