#include"OffersHelper.h"
#include "vigame_core.h"
#include "cjconfig.h"

#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)

#include "core/platform/android/JniHelper.h"
#define CLASS_NAME "org/cocos2dx/cpp/GNGameHallSDK"

#endif

//跳转金立游戏大厅
void OffersHelper::launchGNGameHall()
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	cj::LogText("launchGNGameHall begin");
	JniMethodInfo t;
	bool dd = JniHelper::getStaticMethodInfo(t, CLASS_NAME, "launchGNGameHall", "()V");
	if (JniHelper::getStaticMethodInfo(t,CLASS_NAME,"launchGNGameHall","()V"))
	{
		t.env->CallStaticVoidMethod(t.classID,t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}

//是否由金立游戏大厅
bool OffersHelper::isSupportGamePrivilege()
{
	std::string packagename= vigame::SysConfig::getInstance()->getPackage();
	bool res=true;
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	cj::LogText("isSupportGamePrivilege begin");

	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "isSupportGamePrivilege", "(Ljava/lang/String;)Z"))
	{
		auto jurl = t.env->NewStringUTF(packagename.c_str());
		res=t.env->CallStaticBooleanMethod(t.classID, t.methodID,jurl);
		t.env->DeleteLocalRef(t.classID);

		cj::LogText("isjinliSupport?");
	}
#endif

	return res;

}


