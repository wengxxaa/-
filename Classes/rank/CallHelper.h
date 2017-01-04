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
	std::string price;//�۸񣺷�
	std::string message;//��ʾ����
	std::string state;//ʹ��״̬��-1δʹ�ã�0�һ��벻���ڣ�1�ɹ���2�����ظ�ʹ�ã�3��������
};
//���������ȡ����
class FreeGetData
{
public:
	FreeGetData()
		:state("-1") {}
	std::string segment;//��ȡ�ɹ���ʱ���
	std::string message;//��ʾ����
	std::string state;//ʹ��״̬��-1��ʼֵ������������� 0����ʱ����� 1��ʱ����� (��ʱû��:2�����ظ��콱 3��������)
protected:
private:
};

class WXShareData {
public:
	WXShareData() {}
	std::string resultCode;		//��Ӧ��־, 0��ʾ�ɹ�, ������ʾʧ��
	std::string	resultMsg;		//��Ӧ���� eg:�ɹ�
	std::string	url;            //��������
	std::string	title;          //�������
	std::string	content;        //��������
};

class ExchangeData {
public:
	std::string resultCode;
	std::string resultMsg;
	std::string content;//��ʾ����
	std::string state;//�һ���־��0��ʾ�ɹ��һ��� -1��ʾ��Ʒ�����꣬�´�����  -2��ʾ�Ƿ�����
};

//typedef std::map<std::string,std::string> AdMap;

typedef enum _AdStatus {
	ADSTATUS_SUCCESS,//չʾ�ɹ�
	ADSTATUS_LOAD_FAILED,//������ʧ�ܣ�������ʼ��ʧ�ܣ�Ԥ����ʧ�ܵȶ��鱾�ࣩ
	ADSTATUS_SHOW_FAILED,//���չʾʧ��
}ADStatus;

class CallHelper
{

public:

	//��ȡ�û�ID
	static std::string fetchUserName(std::string lsn);
	//�ж��û��Ƿ�����
	static int isUserOnline(std::string jid);
	//������
	static int doUpdateCheck(std::string ver, std::string package, std::string prjid, std::string mmid);
	//�ύ����
	static int submitMessage(std::string jid, std::string msg);

	static int submitIosTransaction(std::string jid, std::string transaction, int price);
	//send mmid to server
	static int submitMmid();



	//************************************
	// Method:    useDhm-��ȡ�һ����
	// FullName:  CallHelper::useDhm
	// Access:    public static 
	// Returns:   DhmData
	// Qualifier:
	// Parameter: std::string dhm
	//************************************
	static DhmData useDhm(std::string dhm);


	//************************************
	// Method:    useFreeget - ʹ��������ȡ
	// FullName:  CallHelper::useFreeget
	// Access:    public static 
	// Returns:   FreeGetData
	// Qualifier:
	// Parameter: std::string segment eg:11,13
	//************************************
	static FreeGetData useFreeget(std::string segment);

	//************************************
	// Method:    submitGameBonus �ύ��Ϸ��������
	// FullName:  CallHelper::submitGameBonus
	// Access:    public 
	// Returns:   int resultCode  0�ɹ���������ʾʧ��
	// Qualifier:
	// Parameter: int lever �ؿ�
	// Parameter: int score �÷�
	// Parameter: int usedCoins ������ʯ
	// Parameter: int leftCoins ʣ����ʯ
	// Parameter: int topType ���а�����
	//************************************
	static std::string submitGameBonus(int level, int score, int usedCoins, int leftCoins, int topType = 0);

	//************************************
	// Method:    submitWXShare ����΢�ŷ�������
	// FullName:  CallHelper::submitWXShare
	// Access:    public 
	// Returns:   WXShareData
	// Qualifier:
	// Parameter: int lever �ؿ�
	// Parameter: int score �÷�
	//************************************
	static WXShareData submitWXShare(int level, int score);
	//************************************
	// Method:    submitExchangeContact
	// FullName:  CallHelper::submitExchangeContact
	// Access:    public static 
	// Returns:   ExchangeData
	// Qualifier: �ύ�û��Ķһ���Ϣ
	// Parameter: string name
	// Parameter: string tel
	// Parameter: string address
	//************************************
	static ExchangeData submitExchangeContact(std::string prizeName, std::string name, std::string tel, std::string address);
	//��ȡ����ʱ��
	static std::string getBJTime();

	//   //��ȡ��Ҫչʾ�Ĺ��(�첽����)
	//   static void getAd(std::string name,OnHttpResponce responceCall); 
	//   //�ύ���չʾ���(�첽����)
	//   static void responceAd(std::string sid,ADStatus status);

	static std::string _getValue(const std::string& result, const char* startstr, const char* endstr);

	//���û�Э��
	static void openUserAgreement();
	//������
	static void openRank();
	//�򿪹���
	static void openHeadlineDialog();
	//�򿪻
	static void openActivityWeb(int level, int score, int usedCoins, int leftCoins);
	//�ύ�ؿ�����
	static void postLevelData(int level, int score, int usedCoins, int leftCoins);
protected:
	static std::string _submitGameParam(const std::string& name, const std::string& value);

};


#endif //__CALLHELPER_H__