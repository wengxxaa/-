#ifndef OFFERSHELPER_H_
#define OFFERSHELPER_H_

#include "cocos2d.h"
using namespace cocos2d;

class OffersHelper {
public:
	static OffersHelper* getInstance()
	{
		static OffersHelper* one=new OffersHelper();
		if (one)
		{
			return one;
		}
	}

public:

	//��ת������Ϸ����
	void launchGNGameHall();

	//�Ƿ��ɽ�����Ϸ����
	bool isSupportGamePrivilege();

private:
	OffersHelper() {};
};

#endif
