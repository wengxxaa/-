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

	//跳转金立游戏大厅
	void launchGNGameHall();

	//是否由金立游戏大厅
	bool isSupportGamePrivilege();

private:
	OffersHelper() {};
};

#endif
